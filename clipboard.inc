
; fasmx - flat assembler IDE for Linux/X11
; clipboard (CLIPBOARD selection), drag and drop (XDND) and single instance

CLIPBOARD_TIMEOUT = 2000

; x11_wait_event: al = event type, edx = window ([esi+4] or requestor)
; returns CF=0 and esi = copy of event, CF=1 on timeout
; other events are queued for later processing

x11_wait_event:
	push	ebx ecx edx edi
	mov	[waited_event_type],al
	mov	[waited_event_window],edx
	call	get_time
	add	eax,CLIPBOARD_TIMEOUT
	mov	[wait_deadline],eax
	call	x11_flush
    wait_for_event:
	mov	eax,[x11_in_end]
	sub	eax,[x11_in_start]
	cmp	eax,32
	jae	read_waited_packet
	call	get_time
	mov	edx,[wait_deadline]
	sub	edx,eax
	js	event_wait_timeout
	mov	eax,[x11_socket]
	mov	[wait_poll],eax
	mov	dword [wait_poll+4],POLLIN
	mov	eax,SYS_POLL
	mov	ebx,wait_poll
	mov	ecx,1
	int	80h
	test	eax,eax
	js	wait_for_event
	jz	event_wait_timeout
	call	x11_fill
	jmp	wait_for_event
    read_waited_packet:
	call	x11_read_packet
	cmp	al,1
	je	wait_for_event
	test	al,al
	jnz	check_waited_event
	call	x11_report_error
	jmp	wait_for_event
    check_waited_event:
	and	al,7Fh
	cmp	al,[waited_event_type]
	jne	queue_unwaited_event
	mov	eax,[waited_event_window]
	cmp	[waited_event_type],X11_SelectionNotify
	je	check_requestor_window
	cmp	[esi+4],eax
	jne	queue_unwaited_event
	jmp	waited_event_found
    check_requestor_window:
	cmp	[esi+8],eax
	jne	queue_unwaited_event
    waited_event_found:
	mov	edi,waited_event
	mov	ecx,8
	rep	movsd
	mov	esi,waited_event
	pop	edi edx ecx ebx
	clc
	retn
    queue_unwaited_event:
	call	x11_queue_event
	jmp	wait_for_event
    event_wait_timeout:
	pop	edi edx ecx ebx
	stc
	retn

; clipboard_set_cells: esi = cells, ecx = count; CF=1 on allocation failure

clipboard_set_cells:
	; Keep the previous clipboard intact until its replacement is encoded.
	cmp	ecx,3FFFF7FFh
	ja	clipboard_set_failed
	push	esi ecx
	lea	ecx,[ecx*4+4]
	call	get_memory
	pop	ecx esi
	test	eax,eax
	jz	clipboard_set_failed
	push	eax
	mov	edi,eax
	jecxz	clipboard_encoded
    encode_clipboard_cell:
	lodsd
	call	utf8_encode
	loop	encode_clipboard_cell
    clipboard_encoded:
	mov	byte [edi],0
	pop	eax
	sub	edi,eax
	mov	ebx,[clipboard_data]
	mov	[clipboard_data],eax
	mov	[clipboard_length],edi
	call	release_memory
	mov	eax,[main_window]
	mov	ecx,[atom_clipboard]
	mov	edx,[last_event_time]
	call	x11_set_selection_owner
	mov	[clipboard_owned],1
	clc
	retn
    clipboard_set_failed:
	stc
	retn

; clipboard_available: returns eax = nonzero if clipboard has some owner

clipboard_available:
	mov	eax,1
	cmp	[clipboard_owned],0
	jne	clipboard_availability_known
	mov	ecx,[atom_clipboard]
	call	x11_get_selection_owner
    clipboard_availability_known:
	retn

; clipboard_get_cells: returns eax = zero-terminated cells (0 if none)

clipboard_get_cells:
	cmp	[clipboard_owned],0
	je	request_clipboard
	mov	esi,[clipboard_data]
	mov	ecx,[clipboard_length]
	xor	edx,edx
	jmp	decode_clipboard_text
    request_clipboard:
	mov	eax,[atom_utf8_string]
	mov	[clipboard_target],eax
    convert_clipboard:
	mov	eax,[main_window]
	mov	ecx,[atom_clipboard]
	mov	edx,[clipboard_target]
	mov	esi,[atom_fasmx_selection]
	mov	edi,[last_event_time]
	call	x11_convert_selection
	mov	al,X11_SelectionNotify
	mov	edx,[main_window]
	call	x11_wait_event
	jc	no_clipboard_text
	cmp	dword [esi+20],0
	jne	clipboard_converted
	mov	eax,[clipboard_target]
	cmp	eax,X11_STRING
	je	no_clipboard_text
	mov	[clipboard_target],X11_STRING
	jmp	convert_clipboard
    clipboard_converted:
	mov	eax,[atom_fasmx_selection]
	call	read_selection_property
	jc	no_clipboard_text
	mov	esi,[received_data]
	mov	ecx,[received_length]
	xor	edx,edx
	mov	eax,[clipboard_target]
	cmp	eax,X11_STRING
	jne	decode_clipboard_text
	inc	edx
    decode_clipboard_text:
	mov	[clipboard_latin1],edx
	push	esi ecx
	lea	ecx,[ecx*4+4]
	call	get_memory
	pop	ecx esi
	test	eax,eax
	jz	clipboard_text_decoded
	push	eax
	mov	edi,eax
	cmp	[clipboard_latin1],0
	jne	decode_latin1_text
	call	utf8_to_cells
	jmp	clipboard_cells_ready
    decode_latin1_text:
	jecxz	latin1_decoded
    decode_latin1_byte:
	movzx	eax,byte [esi]
	inc	esi
	test	eax,eax
	jz	latin1_byte_skipped
	stosd
    latin1_byte_skipped:
	loop	decode_latin1_byte
    latin1_decoded:
	xor	eax,eax
	stosd
    clipboard_cells_ready:
	pop	eax
    clipboard_text_decoded:
	push	eax
	cmp	[clipboard_owned],0
	jne	clipboard_received_kept
	mov	ebx,[received_data]
	call	release_memory
    clipboard_received_kept:
	pop	eax
	retn
    no_clipboard_text:
	xor	eax,eax
	retn

; read_selection_property: eax = property of main window
; returns CF=0 and received_data, received_length (memory to release)

read_selection_property:
	mov	[received_property],eax
	mov	[received_data],0
	mov	[received_length],0
	call	read_property_value
	jc	property_read_failed
	mov	eax,[received_type]
	cmp	eax,[atom_incr]
	je	incremental_transfer
	cmp	[received_data],0
	je	property_read_failed
	clc
	retn
    incremental_transfer:
	mov	ebx,[received_data]
	call	release_memory
	mov	[received_data],0
	mov	[received_length],0
    next_incremental_part:
	mov	al,X11_PropertyNotify
	mov	edx,[main_window]
	call	x11_wait_event
	jc	property_read_failed
	mov	eax,[esi+8]
	cmp	eax,[received_property]
	jne	next_incremental_part
	cmp	byte [esi+16],0
	jne	next_incremental_part
	mov	eax,[received_length]
	push	eax
	call	read_property_value
	pop	eax
	jc	property_read_failed
	cmp	eax,[received_length]
	jne	next_incremental_part
	cmp	[received_data],0
	je	property_read_failed
	clc
	retn
    property_read_failed:
	mov	[property_offset],0
	mov	ebx,[received_data]
	call	release_memory
	mov	[received_data],0
	stc
	retn

; read_property_value: appends value of [received_property] to received
; data, deleting the property; sets [received_type]

read_property_value:
	mov	[property_offset],0
    read_property_part:
	mov	eax,[main_window]
	mov	ecx,[received_property]
	mov	edx,1
	call	x11_get_property
	jc	property_value_failed
	mov	eax,[esi+8]
	mov	[received_type],eax
	test	eax,eax
	jz	property_value_read
	mov	eax,[esi+12]
	mov	[received_bytes_after],eax
	movzx	eax,byte [esi+1]
	shr	eax,3
	imul	eax,[esi+16]
	shr	eax,2
	add	[property_offset],eax
	call	append_property_data
	jc	property_value_failed
	cmp	[received_bytes_after],0
	jne	read_property_part
    property_value_read:
	mov	[property_offset],0
	clc
	retn
    property_value_failed:
	mov	[property_offset],0
	stc
	retn

; append_property_data: esi = GetProperty reply

append_property_data:
	movzx	ecx,byte [esi+1]
	shr	ecx,3
	imul	ecx,[esi+16]
	push	esi ecx
	mov	eax,[received_length]
	add	eax,ecx
	jc	property_data_failed
	add	eax,1
	jc	property_data_failed
	; Leave room for growth and the allocator's header/rounding.
	cmp	eax,7FFF0000h
	ja	property_data_failed
	cmp	[received_data],0
	jne	enlarge_received_data
	lea	ecx,[eax+10000h]
	call	get_memory
	test	eax,eax
	jz	property_data_failed
	mov	[received_data],eax
	sub	ecx,1000h
	mov	[received_size],ecx
	jmp	received_space_ok
    enlarge_received_data:
	cmp	eax,[received_size]
	jbe	received_space_ok
	lea	ecx,[eax*2]
	push	ecx
	mov	ebx,[received_data]
	call	resize_memory
	pop	ecx
	test	eax,eax
	jz	property_data_failed
	mov	[received_data],eax
	mov	[received_size],ecx
    received_space_ok:
	pop	ecx esi
	mov	edi,[received_data]
	add	edi,[received_length]
	add	[received_length],ecx
	add	esi,32
	rep	movsb
	mov	byte [edi],0
	clc
	retn
    property_data_failed:
	pop	ecx esi
	stc
	retn

; selection_request_event: esi = event

selection_request_event:
	mov	edi,selection_notify
	mov	byte [edi],X11_SelectionNotify
	mov	eax,[esi+4]
	mov	[edi+4],eax
	mov	eax,[esi+12]
	mov	[edi+8],eax
	mov	eax,[esi+16]
	mov	[edi+12],eax
	mov	eax,[esi+20]
	mov	[edi+16],eax
	mov	eax,[esi+24]
	test	eax,eax
	jnz	selection_property_ok
	mov	eax,[esi+20]
    selection_property_ok:
	mov	[edi+20],eax
	mov	eax,[esi+16]
	cmp	eax,[atom_clipboard]
	jne	refuse_selection
	cmp	[clipboard_owned],0
	je	refuse_selection
	mov	eax,[esi+20]
	cmp	eax,[atom_targets]
	je	send_selection_targets
	cmp	eax,[atom_utf8_string]
	je	send_selection_text
	cmp	eax,[atom_text]
	je	send_selection_text
	cmp	eax,X11_STRING
	je	send_selection_text
    refuse_selection:
	mov	dword [selection_notify+20],0
	jmp	send_selection_notify
    send_selection_targets:
	mov	eax,[atom_targets]
	mov	[selection_targets],eax
	mov	eax,[atom_utf8_string]
	mov	[selection_targets+4],eax
	mov	[selection_targets+8],X11_STRING
	mov	eax,[atom_text]
	mov	[selection_targets+12],eax
	mov	eax,[selection_notify+8]
	mov	ebx,[selection_notify+20]
	mov	ecx,X11_ATOM
	mov	edx,32
	mov	esi,selection_targets
	mov	edi,4
	call	x11_change_property
	jmp	send_selection_notify
    send_selection_text:
	mov	ecx,[atom_utf8_string]
	cmp	dword [selection_notify+16],X11_STRING
	jne	selection_text_type_ok
	mov	ecx,X11_STRING
    selection_text_type_ok:
	mov	[incr_type],ecx
	mov	eax,[x11_max_request_size]
	sub	eax,64
	mov	[incr_chunk],eax
	mov	edi,[clipboard_length]
	cmp	edi,eax
	ja	start_incremental_send
	mov	eax,[selection_notify+8]
	mov	ebx,[selection_notify+20]
	mov	edx,8
	mov	esi,[clipboard_data]
	call	x11_change_property
	jmp	send_selection_notify
    start_incremental_send:
	cmp	[incr_window],0
	jne	refuse_selection
	mov	eax,[selection_notify+8]
	mov	[incr_window],eax
	mov	ebx,[selection_notify+20]
	mov	[incr_property],ebx
	mov	[incr_offset],0
	mov	ecx,X11_PropertyChangeMask
	call	x11_select_input
	mov	ecx,[atom_incr]
	mov	edx,32
	mov	esi,clipboard_length
	mov	edi,1
	call	x11_change_property
    send_selection_notify:
	mov	eax,[selection_notify+8]
	xor	ecx,ecx
	mov	esi,selection_notify
	jmp	x11_send_event

; selection_clear_event: esi = event

selection_clear_event:
	mov	eax,[esi+8]
	cmp	eax,[atom_clipboard]
	jne	selection_cleared
	mov	[clipboard_owned],0
    selection_cleared:
	retn

; property_event: esi = event

property_event:
	mov	eax,[esi+12]
	mov	[last_event_time],eax
	mov	eax,[esi+4]
	cmp	eax,[incr_window]
	jne	property_not_incremental
	mov	eax,[esi+8]
	cmp	eax,[incr_property]
	jne	property_handled
	cmp	byte [esi+16],1
	jne	property_handled
	mov	esi,[clipboard_data]
	add	esi,[incr_offset]
	mov	edi,[clipboard_length]
	sub	edi,[incr_offset]
	cmp	edi,[incr_chunk]
	jbe	incremental_chunk_ok
	mov	edi,[incr_chunk]
    incremental_chunk_ok:
	add	[incr_offset],edi
	mov	eax,[incr_window]
	mov	ebx,[incr_property]
	mov	ecx,[incr_type]
	mov	edx,8
	push	edi
	call	x11_change_property
	pop	edi
	test	edi,edi
	jnz	property_handled
	xor	ecx,ecx
	call	x11_select_input
	mov	[incr_window],0
    property_handled:
	retn
    property_not_incremental:
	cmp	eax,[main_window]
	jne	property_handled
	mov	eax,[esi+8]
	cmp	eax,[atom_fasmx_command]
	jne	property_handled
	cmp	byte [esi+16],0
	jne	property_handled
	jmp	instance_command

; drag and drop

dnd_message:
	cmp	eax,[atom_xdnd_enter]
	je	dnd_enter
	cmp	eax,[atom_xdnd_position]
	je	dnd_position
	cmp	eax,[atom_xdnd_drop]
	je	dnd_drop
	cmp	eax,[atom_xdnd_leave]
	je	dnd_leave
	retn
    dnd_enter:
	mov	eax,[esi+12]
	mov	[dnd_source],eax
	mov	[dnd_accept],0
	test	byte [esi+16],1
	jnz	dnd_type_list
	mov	ecx,3
	lea	edx,[esi+20]
    check_dnd_type:
	mov	eax,[edx]
	cmp	eax,[atom_uri_list]
	je	dnd_type_accepted
	add	edx,4
	loop	check_dnd_type
	retn
    dnd_type_list:
	mov	eax,[dnd_source]
	mov	ecx,[atom_xdnd_type_list]
	xor	edx,edx
	call	x11_get_property
	jc	dnd_done
	mov	ecx,[esi+16]
	lea	edx,[esi+32]
	jecxz	dnd_done
	jmp	check_dnd_type
    dnd_type_accepted:
	mov	[dnd_accept],1
    dnd_done:
	retn
    dnd_position:
	mov	eax,[esi+12]
	mov	edi,dnd_event
	call	prepare_dnd_message
	mov	eax,[atom_xdnd_status]
	mov	[edi+8],eax
	xor	eax,eax
	cmp	[current_dialog],0
	jne	dnd_status_ready
	mov	eax,[dnd_accept]
    dnd_status_ready:
	mov	[edi+16],eax
	neg	eax
	and	eax,[atom_xdnd_action_copy]
	mov	[edi+28],eax
	jmp	send_dnd_message
    dnd_leave:
	mov	[dnd_accept],0
	retn
    dnd_drop:
	mov	eax,[esi+12]
	mov	[dnd_source],eax
	mov	edi,[esi+20]
	cmp	[current_dialog],0
	jne	dnd_rejected
	cmp	[dnd_accept],0
	je	dnd_rejected
	mov	eax,[main_window]
	mov	ecx,[atom_xdnd_selection]
	mov	edx,[atom_uri_list]
	mov	esi,[atom_fasmx_drop]
	call	x11_convert_selection
	mov	[dnd_pending],1
	retn
    dnd_rejected:
	xor	eax,eax
	jmp	finish_dnd

; prepare_dnd_message: eax = destination, edi = event buffer

prepare_dnd_message:
	push	eax
	push	edi
	xor	eax,eax
	mov	ecx,8
	rep	stosd
	pop	edi
	pop	eax
	mov	byte [edi],X11_ClientMessage
	mov	byte [edi+1],32
	mov	[edi+4],eax
	mov	eax,[main_window]
	mov	[edi+12],eax
	retn

send_dnd_message:
	mov	eax,[edi+4]
	xor	ecx,ecx
	mov	esi,edi
	jmp	x11_send_event

; finish_dnd: eax = 1 if drop was accepted

finish_dnd:
	push	eax
	mov	eax,[dnd_source]
	mov	edi,dnd_event
	call	prepare_dnd_message
	mov	eax,[atom_xdnd_finished]
	mov	[edi+8],eax
	pop	eax
	mov	[edi+16],eax
	neg	eax
	and	eax,[atom_xdnd_action_copy]
	mov	[edi+20],eax
	mov	[dnd_accept],0
	jmp	send_dnd_message

; selection_notify_event: esi = event (asynchronous conversions)

selection_notify_event:
	cmp	[dnd_pending],0
	je	selection_notify_done
	mov	eax,[esi+12]
	cmp	eax,[atom_xdnd_selection]
	jne	selection_notify_done
	mov	[dnd_pending],0
	cmp	dword [esi+20],0
	je	dnd_rejected
	mov	eax,[atom_fasmx_drop]
	call	read_selection_property
	jc	dnd_rejected
	mov	esi,[received_data]
    next_dropped_uri:
	cmp	byte [esi],0
	je	dropped_files_opened
	mov	edi,esi
	mov	ebx,_uri_file_prefix
    compare_uri_prefix:
	mov	al,[ebx]
	test	al,al
	jz	uri_prefix_matches
	cmp	al,[edi]
	jne	skip_dropped_uri
	inc	ebx
	inc	edi
	jmp	compare_uri_prefix
    uri_prefix_matches:
	cmp	byte [edi],'/'
	je	uri_path_found
	cmp	byte [edi],0Dh
	je	skip_dropped_uri
	cmp	byte [edi],0Ah
	je	skip_dropped_uri
	cmp	byte [edi],0
	je	skip_dropped_uri
	inc	edi
	jmp	uri_prefix_matches
    uri_path_found:
	mov	esi,edi
	mov	edi,dropped_path
	lea	edx,[edi+PATH_SIZE-1]
    decode_uri_character:
	lodsb
	cmp	al,0Dh
	je	uri_decoded
	cmp	al,0Ah
	je	uri_decoded
	test	al,al
	jz	uri_decoded_at_end
	cmp	al,'%'
	jne	store_uri_character
	call	uri_hex_digit
	jc	store_uri_character
	mov	ah,al
	call	uri_hex_digit
	jc	store_uri_character
	shl	ah,4
	or	al,ah
    store_uri_character:
	cmp	edi,edx
	jae	decode_uri_character
	stosb
	jmp	decode_uri_character
    uri_decoded_at_end:
	dec	esi
    uri_decoded:
	mov	byte [edi],0
	push	esi
	mov	esi,dropped_path
	xor	ecx,ecx
	call	fm_open
	cmp	eax,-1
	jne	dropped_file_opened
	mov	esi,dropped_path
	call	show_loading_error
    dropped_file_opened:
	pop	esi
	jmp	next_dropped_uri
    skip_dropped_uri:
	lodsb
	test	al,al
	jz	last_uri_skipped
	cmp	al,0Ah
	jne	skip_dropped_uri
	jmp	next_dropped_uri
    last_uri_skipped:
	dec	esi
	jmp	next_dropped_uri
    dropped_files_opened:
	mov	ebx,[received_data]
	call	release_memory
	mov	eax,1
	jmp	finish_dnd
    selection_notify_done:
	retn

; uri_hex_digit: esi = text, returns CF=0 and al = value of hexadecimal digit

uri_hex_digit:
	mov	al,[esi]
	sub	al,'0'
	cmp	al,9
	jbe	uri_digit_ok
	mov	al,[esi]
	or	al,20h
	sub	al,'a'
	cmp	al,5
	ja	uri_digit_invalid
	add	al,10
    uri_digit_ok:
	inc	esi
	clc
	retn
    uri_digit_invalid:
	stc
	retn

; single instance

claim_instance:
	mov	eax,[main_window]
	mov	ecx,[atom_fasmx_instance]
	xor	edx,edx
	jmp	x11_set_selection_owner

; pass_to_running_instance: returns CF=0 if command was passed to the
; running instance

pass_to_running_instance:
	mov	ecx,[atom_fasmx_instance]
	call	x11_get_selection_owner
	test	eax,eax
	jz	no_running_instance
	mov	[instance_owner],eax
	mov	edi,instance_buffer
	call	get_current_directory
	mov	esi,edi
	call	string_length
	lea	edi,[edi+ecx+1]
	mov	ebx,[arguments]
	add	ebx,4
    pass_next_argument:
	mov	esi,[ebx]
	test	esi,esi
	jz	instance_arguments_ready
	call	string_length
	lea	eax,[edi+ecx+2]
	cmp	eax,instance_buffer+INSTANCE_BUFFER_SIZE
	ja	instance_arguments_ready
	call	copy_text
	inc	edi
	add	ebx,4
	jmp	pass_next_argument
    instance_arguments_ready:
	mov	byte [edi],0
	inc	edi
	sub	edi,instance_buffer
	mov	eax,[instance_owner]
	mov	ebx,[atom_fasmx_command]
	mov	ecx,[atom_utf8_string]
	mov	edx,8
	mov	esi,instance_buffer
	call	x11_change_property
	mov	ecx,[atom_fasmx_instance]
	call	x11_get_selection_owner
	clc
	retn
    no_running_instance:
	stc
	retn

; instance_command: another instance passed its arguments

instance_command:
	mov	eax,[esi+12]
	mov	[instance_time],eax
	mov	eax,[main_window]
	mov	ecx,[atom_fasmx_command]
	mov	edx,1
	call	x11_get_property
	jc	instance_command_done
	mov	ecx,[esi+16]
	cmp	ecx,INSTANCE_BUFFER_SIZE-2
	jbe	instance_data_size_ok
	mov	ecx,INSTANCE_BUFFER_SIZE-2
    instance_data_size_ok:
	add	esi,32
	mov	edi,instance_buffer
	rep	movsb
	xor	eax,eax
	stosw
	mov	esi,instance_buffer
	call	string_length
	lea	esi,[esi+ecx+1]
	mov	edi,instance_arguments
	xor	edx,edx
    collect_instance_argument:
	cmp	byte [esi],0
	je	instance_arguments_collected
	cmp	edx,INSTANCE_MAX_ARGUMENTS
	jae	instance_arguments_collected
	mov	[edi+edx*4],esi
	inc	edx
	call	string_length
	lea	esi,[esi+ecx+1]
	jmp	collect_instance_argument
    instance_arguments_collected:
	mov	dword [edi+edx*4],0
	cmp	[current_dialog],0
	jne	instance_activate
	mov	esi,instance_arguments
	mov	ebx,instance_buffer
	call	process_arguments
    instance_activate:
	mov	eax,[main_window]
	mov	edi,dnd_event
	call	prepare_dnd_message
	mov	eax,[main_window]
	mov	[edi+4],eax
	mov	eax,[atom_net_active_window]
	mov	[edi+8],eax
	mov	dword [edi+12],1
	mov	eax,[instance_time]
	mov	[edi+16],eax
	mov	eax,[x11_root]
	mov	ecx,180000h
	mov	esi,edi
	call	x11_send_event
    instance_command_done:
	retn

INSTANCE_BUFFER_SIZE = 8000h
INSTANCE_MAX_ARGUMENTS = 256
