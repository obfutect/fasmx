
; fasmx - flat assembler IDE for Linux/X11
; X11 protocol client (core protocol over a local socket)

X11_OUT_BUFFER_SIZE = 10000h
X11_IN_BUFFER_SIZE  = 10000h
X11_PACKET_SIZE     = 10000h
X11_QUEUE_LENGTH    = 64

X11_KeyPress	     = 2
X11_KeyRelease	     = 3
X11_ButtonPress      = 4
X11_ButtonRelease    = 5
X11_MotionNotify     = 6
X11_FocusIn	     = 9
X11_FocusOut	     = 10
X11_Expose	     = 12
X11_ConfigureNotify  = 22
X11_ClientMessage    = 33
X11_MappingNotify    = 34

X11_KeyPressMask	= 1
X11_ButtonPressMask	= 4
X11_ButtonReleaseMask	= 8
X11_Button1MotionMask	= 100h
X11_ExposureMask	= 8000h
X11_StructureNotifyMask = 20000h
X11_FocusChangeMask	= 200000h

X11_Button1Mask = 100h

X11_ATOM	= 4
X11_STRING	= 31
X11_WM_HINTS	= 35
X11_WM_NAME	= 39
X11_WM_CLASS	= 67

X11_GXinvert = 0Ah

; x11_connect: connects to the display named by the DISPLAY variable
; returns CF=1 and esi = error message on failure

x11_connect:
	mov	esi,_display_variable
	call	find_environment_variable
	mov	esi,_error_no_display
	test	eax,eax
	jz	x11_connect_failed
	mov	esi,eax
	mov	edi,eax
    find_display_colon:
	mov	al,[esi]
	inc	esi
	cmp	al,':'
	je	display_colon_found
	test	al,al
	jnz	find_display_colon
    bad_display_name:
	mov	esi,_error_bad_display
	jmp	x11_connect_failed
    display_colon_found:
	lea	ecx,[esi-1]
	sub	ecx,edi
	jz	local_display
	cmp	ecx,4
	jne	remote_display
	cmp	dword [edi],'unix'
	je	local_display
    remote_display:
	mov	esi,_error_remote_display
	jmp	x11_connect_failed
    local_display:
	mov	[x11_display_digits],esi
	xor	eax,eax
	xor	ecx,ecx
    parse_display_number:
	movzx	edx,byte [esi]
	sub	edx,'0'
	cmp	edx,9
	ja	display_number_parsed
	imul	eax,10
	add	eax,edx
	inc	esi
	inc	ecx
	jmp	parse_display_number
    display_number_parsed:
	jecxz	bad_display_name
	mov	[x11_display_digits_length],ecx
	mov	[x11_display_number],eax
	mov	[x11_screen_number],0
	cmp	byte [esi],'.'
	jne	display_name_parsed
	inc	esi
	xor	eax,eax
    parse_screen_number:
	movzx	edx,byte [esi]
	sub	edx,'0'
	cmp	edx,9
	ja	screen_number_parsed
	imul	eax,10
	add	eax,edx
	inc	esi
	jmp	parse_screen_number
    screen_number_parsed:
	mov	[x11_screen_number],eax
    display_name_parsed:
	mov	edi,x11_socket_address
	mov	word [edi],AF_UNIX
	add	edi,2
	mov	esi,_x11_socket_path
    copy_socket_path:
	lodsb
	stosb
	test	al,al
	jnz	copy_socket_path
	dec	edi
	mov	esi,[x11_display_digits]
	mov	ecx,[x11_display_digits_length]
	rep	movsb
	mov	byte [edi],0
	sub	edi,x11_socket_address
	mov	[x11_socket_address_length],edi
	call	x11_connect_socket
	jnc	x11_socket_connected
	mov	esi,x11_socket_address+2+106
	mov	edi,x11_socket_address+2+107
	mov	ecx,107
	std
	rep	movsb
	cld
	mov	byte [x11_socket_address+2],0
	inc	[x11_socket_address_length]
	call	x11_connect_socket
	mov	esi,_error_connect
	jc	x11_connect_failed
    x11_socket_connected:
	call	x11_find_authorization
	mov	edi,x11_out_buffer
	mov	byte [edi],'l'
	mov	byte [edi+1],0
	mov	word [edi+2],11
	mov	word [edi+4],0
	mov	eax,[x11_auth_name_length]
	mov	[edi+6],ax
	mov	eax,[x11_auth_data_length]
	mov	[edi+8],ax
	mov	word [edi+10],0
	add	edi,12
	mov	esi,x11_auth_name
	mov	ecx,[x11_auth_name_length]
	add	ecx,3
	and	ecx,not 3
	rep	movsb
	mov	esi,x11_auth_data
	mov	ecx,[x11_auth_data_length]
	add	ecx,3
	and	ecx,not 3
	rep	movsb
	sub	edi,x11_out_buffer
	mov	[x11_out_length],edi
	call	x11_flush
	mov	edi,x11_packet
	mov	ecx,8
	call	x11_consume
	movzx	ecx,word [x11_packet+6]
	shl	ecx,2
	push	ecx
	add	ecx,8
	call	get_memory
	pop	ecx
	mov	esi,_error_memory
	test	eax,eax
	jz	x11_connect_failed
	mov	[x11_setup_memory],ebx
	mov	edi,eax
	mov	esi,x11_packet
	movsd
	movsd
	call	x11_consume
	mov	esi,[x11_setup_memory]
	cmp	byte [esi],1
	je	x11_setup_accepted
	mov	esi,_error_setup
	jmp	x11_connect_failed
    x11_setup_accepted:
	mov	eax,[esi+12]
	mov	[x11_id_base],eax
	mov	eax,[esi+16]
	mov	[x11_id_mask],eax
	mov	[x11_id_next],0
	movzx	eax,word [esi+26]
	shl	eax,2
	mov	[x11_max_request_size],eax
	mov	al,[esi+34]
	mov	[x11_min_keycode],al
	mov	al,[esi+35]
	mov	[x11_max_keycode],al
	movzx	eax,word [esi+24]
	add	eax,3
	and	eax,not 3
	movzx	ecx,byte [esi+29]
	lea	ebx,[esi+40+eax]
	lea	ebx,[ebx+ecx*8]
	movzx	ecx,byte [esi+28]
	mov	edx,[x11_screen_number]
	cmp	edx,ecx
	jb	find_screen
	xor	edx,edx
    find_screen:
	test	edx,edx
	jz	screen_found
	movzx	ecx,byte [ebx+39]
	add	ebx,40
    skip_screen_depths:
	jecxz	screen_skipped
	movzx	eax,word [ebx+2]
	lea	eax,[eax*3]
	lea	ebx,[ebx+8+eax*8]
	dec	ecx
	jmp	skip_screen_depths
    screen_skipped:
	dec	edx
	jmp	find_screen
    screen_found:
	movzx	eax,word [ebx+20]
	mov	[x11_screen_width],eax
	movzx	eax,word [ebx+22]
	mov	[x11_screen_height],eax
	mov	eax,[ebx]
	mov	[x11_root],eax
	mov	eax,[ebx+4]
	mov	[x11_colormap],eax
	mov	eax,[ebx+8]
	mov	[x11_white_pixel],eax
	mov	eax,[ebx+12]
	mov	[x11_black_pixel],eax
	mov	eax,[ebx+32]
	mov	[x11_root_visual],eax
	mov	al,[ebx+38]
	mov	[x11_root_depth],al
	mov	[x11_truecolor],0
	movzx	ecx,byte [ebx+39]
	add	ebx,40
    find_root_visual:
	jecxz	root_visual_done
	movzx	edx,word [ebx+2]
	add	ebx,8
    check_visual:
	test	edx,edx
	jz	next_depth
	mov	eax,[ebx]
	cmp	eax,[x11_root_visual]
	je	root_visual_found
	add	ebx,24
	dec	edx
	jmp	check_visual
    next_depth:
	dec	ecx
	jmp	find_root_visual
    root_visual_found:
	cmp	byte [ebx+4],4
	jne	root_visual_done
	mov	[x11_truecolor],1
	mov	eax,[ebx+8]
	mov	edi,x11_red_shift
	call	x11_analyze_mask
	mov	eax,[ebx+12]
	mov	edi,x11_green_shift
	call	x11_analyze_mask
	mov	eax,[ebx+16]
	mov	edi,x11_blue_shift
	call	x11_analyze_mask
    root_visual_done:
	mov	ebx,[x11_setup_memory]
	call	release_memory
	clc
	retn
    x11_connect_failed:
	stc
	retn

x11_connect_socket:
	mov	dword [socketcall_arguments],AF_UNIX
	mov	dword [socketcall_arguments+4],SOCK_STREAM
	mov	dword [socketcall_arguments+8],0
	mov	eax,SYS_SOCKETCALL
	mov	ebx,SYS_SOCKET
	mov	ecx,socketcall_arguments
	int	80h
	test	eax,eax
	js	x11_socket_connect_failed
	mov	[x11_socket],eax
	mov	[socketcall_arguments],eax
	mov	dword [socketcall_arguments+4],x11_socket_address
	mov	eax,[x11_socket_address_length]
	mov	[socketcall_arguments+8],eax
	mov	eax,SYS_SOCKETCALL
	mov	ebx,SYS_CONNECT
	mov	ecx,socketcall_arguments
	int	80h
	test	eax,eax
	js	x11_socket_not_connected
	mov	eax,SYS_FCNTL
	mov	ebx,[x11_socket]
	mov	ecx,F_SETFD
	mov	edx,FD_CLOEXEC
	int	80h
	clc
	retn
    x11_socket_not_connected:
	mov	eax,SYS_CLOSE
	mov	ebx,[x11_socket]
	int	80h
    x11_socket_connect_failed:
	stc
	retn

x11_analyze_mask:
	xor	ecx,ecx
	xor	edx,edx
	test	eax,eax
	jz	mask_analyzed
	bsf	ecx,eax
	bsr	edx,eax
	sub	edx,ecx
	inc	edx
    mask_analyzed:
	mov	[edi],ecx
	mov	[edi+4],edx
	retn

; x11_find_authorization: looks up the MIT-MAGIC-COOKIE-1 for the display

x11_find_authorization:
	mov	[x11_auth_name_length],0
	mov	[x11_auth_data_length],0
	mov	esi,_xauthority_variable
	call	find_environment_variable
	test	eax,eax
	jnz	authority_file_known
	mov	esi,_home_variable
	call	find_environment_variable
	test	eax,eax
	jz	no_authorization
	mov	esi,eax
	mov	edi,path_buffer
	mov	ecx,1000h-20
    copy_home_path:
	lodsb
	test	al,al
	jz	home_path_copied
	stosb
	loop	copy_home_path
	jmp	no_authorization
    home_path_copied:
	mov	esi,_xauthority_file
    append_authority_file:
	lodsb
	stosb
	test	al,al
	jnz	append_authority_file
	mov	eax,path_buffer
    authority_file_known:
	mov	ebx,eax
	call	load_file
	jc	no_authorization
	mov	[x11_auth_file],ebx
	lea	ebp,[eax+ecx]
	mov	esi,eax
	mov	eax,SYS_UNAME
	mov	ebx,utsname
	int	80h
	mov	edi,utsname+65
	xor	al,al
	mov	ecx,65
	repne	scasb
	sub	edi,utsname+65+1
	mov	[x11_host_name_length],edi
    next_authority_entry:
	lea	eax,[esi+2]
	cmp	eax,ebp
	ja	authorization_done
	movzx	eax,word [esi]
	xchg	al,ah
	mov	[authority_family],eax
	add	esi,2
	call	authority_field
	jc	authorization_done
	mov	[authority_address],edx
	mov	[authority_address_length],ecx
	call	authority_field
	jc	authorization_done
	mov	[authority_number],edx
	mov	[authority_number_length],ecx
	call	authority_field
	jc	authorization_done
	mov	[authority_name],edx
	mov	[authority_name_length],ecx
	call	authority_field
	jc	authorization_done
	mov	[authority_data],edx
	mov	[authority_data_length],ecx
	cmp	[authority_family],0FFFFh
	je	authority_address_matches
	cmp	[authority_family],256
	jne	next_authority_entry
	mov	ecx,[authority_address_length]
	cmp	ecx,[x11_host_name_length]
	jne	next_authority_entry
	push	esi
	mov	esi,[authority_address]
	mov	edi,utsname+65
	repe	cmpsb
	pop	esi
	jne	next_authority_entry
    authority_address_matches:
	mov	ecx,[authority_number_length]
	jecxz	authority_number_matches
	cmp	ecx,[x11_display_digits_length]
	jne	next_authority_entry
	push	esi
	mov	esi,[authority_number]
	mov	edi,[x11_display_digits]
	repe	cmpsb
	pop	esi
	jne	next_authority_entry
    authority_number_matches:
	cmp	[authority_name_length],18
	jne	next_authority_entry
	push	esi
	mov	esi,[authority_name]
	mov	edi,_mit_magic_cookie
	mov	ecx,18
	repe	cmpsb
	pop	esi
	jne	next_authority_entry
	mov	ecx,[authority_data_length]
	cmp	ecx,100h
	ja	next_authority_entry
	mov	[x11_auth_data_length],ecx
	push	esi
	mov	esi,[authority_data]
	mov	edi,x11_auth_data
	rep	movsb
	mov	esi,_mit_magic_cookie
	mov	edi,x11_auth_name
	mov	ecx,18
	mov	[x11_auth_name_length],ecx
	rep	movsb
	pop	esi
    authorization_done:
	mov	ebx,[x11_auth_file]
	call	release_memory
    no_authorization:
	retn
    authority_field:
	lea	eax,[esi+2]
	cmp	eax,ebp
	ja	authority_field_invalid
	movzx	ecx,word [esi]
	xchg	cl,ch
	add	esi,2
	mov	edx,esi
	add	esi,ecx
	cmp	esi,ebp
	ja	authority_field_invalid
	clc
	retn
    authority_field_invalid:
	stc
	retn

; x11_new_id: returns eax = new resource identifier

x11_new_id:
	mov	eax,[x11_id_next]
	inc	[x11_id_next]
	push	ecx
	mov	ecx,[x11_id_mask]
	bsf	ecx,ecx
	shl	eax,cl
	pop	ecx
	and	eax,[x11_id_mask]
	or	eax,[x11_id_base]
	retn

; x11_begin_request: ecx = size of request in bytes (multiple of 4)
; returns edi = place for request data, eax = sequence number

x11_begin_request:
	mov	eax,[x11_out_length]
	add	eax,ecx
	cmp	eax,X11_OUT_BUFFER_SIZE
	jbe	request_space_ok
	push	ecx
	call	x11_flush
	pop	ecx
    request_space_ok:
	mov	edi,[x11_out_length]
	add	[x11_out_length],ecx
	add	edi,x11_out_buffer
	inc	[x11_sequence]
	mov	eax,[x11_sequence]
	retn

x11_flush:
	push	eax ebx ecx edx esi
	mov	esi,x11_out_buffer
	mov	edx,[x11_out_length]
	mov	ebx,[x11_socket]
    flush_output:
	test	edx,edx
	jz	output_flushed
	mov	eax,SYS_WRITE
	mov	ecx,esi
	int	80h
	cmp	eax,-EINTR
	je	flush_output
	test	eax,eax
	js	x11_connection_lost
	add	esi,eax
	sub	edx,eax
	jmp	flush_output
    output_flushed:
	mov	[x11_out_length],0
	pop	esi edx ecx ebx eax
	retn

x11_connection_lost:
	mov	ebx,STDERR
	mov	esi,_error_connection_lost
	call	write_string
	mov	al,1
	jmp	exit_program

; x11_fill: reads available data from the server into the input buffer

x11_fill:
	push	eax ebx ecx edx
	mov	eax,[x11_in_start]
	cmp	eax,[x11_in_end]
	jne	input_buffer_not_empty
	xor	eax,eax
	mov	[x11_in_start],eax
	mov	[x11_in_end],eax
	jmp	read_input
    input_buffer_not_empty:
	cmp	[x11_in_end],X11_IN_BUFFER_SIZE
	jb	read_input
	push	esi edi
	mov	esi,[x11_in_start]
	mov	ecx,[x11_in_end]
	sub	ecx,esi
	add	esi,x11_in_buffer
	mov	edi,x11_in_buffer
	mov	[x11_in_end],ecx
	mov	[x11_in_start],0
	rep	movsb
	pop	edi esi
    read_input:
	mov	eax,SYS_READ
	mov	ebx,[x11_socket]
	mov	ecx,[x11_in_end]
	add	ecx,x11_in_buffer
	mov	edx,X11_IN_BUFFER_SIZE
	sub	edx,[x11_in_end]
	int	80h
	cmp	eax,-EINTR
	je	read_input
	test	eax,eax
	jle	x11_connection_lost
	add	[x11_in_end],eax
	pop	edx ecx ebx eax
	retn

; x11_consume: edi = destination (0 to discard), ecx = number of bytes

x11_consume:
	push	eax esi
    consume_input:
	jecxz	input_consumed
	mov	eax,[x11_in_end]
	sub	eax,[x11_in_start]
	jnz	input_available
	call	x11_fill
	jmp	consume_input
    input_available:
	cmp	eax,ecx
	jbe	consumed_length_ok
	mov	eax,ecx
    consumed_length_ok:
	mov	esi,[x11_in_start]
	add	[x11_in_start],eax
	sub	ecx,eax
	test	edi,edi
	jz	consume_input
	add	esi,x11_in_buffer
	push	ecx
	mov	ecx,eax
	rep	movsb
	pop	ecx
	jmp	consume_input
    input_consumed:
	pop	esi eax
	retn

; x11_read_packet: reads the next event, reply or error
; returns esi = x11_packet, eax = packet type

x11_read_packet:
	push	ebx ecx edi
	mov	edi,x11_packet
	mov	ecx,32
	call	x11_consume
	mov	al,[x11_packet]
	cmp	al,1
	je	long_packet
	and	al,7Fh
	cmp	al,35
	jne	packet_read
    long_packet:
	mov	ebx,dword [x11_packet+4]
	shl	ebx,2
	mov	ecx,ebx
	cmp	ecx,X11_PACKET_SIZE-32
	jbe	packet_fits
	mov	ecx,X11_PACKET_SIZE-32
    packet_fits:
	sub	ebx,ecx
	mov	edi,x11_packet+32
	call	x11_consume
	mov	ecx,ebx
	xor	edi,edi
	call	x11_consume
    packet_read:
	movzx	eax,byte [x11_packet]
	mov	esi,x11_packet
	pop	edi ecx ebx
	retn

; x11_wait_reply: eax = sequence number of request
; returns CF=0 and esi = reply, or CF=1 and esi = error

x11_wait_reply:
	push	ebx
	mov	ebx,eax
	call	x11_flush
    wait_for_reply:
	call	x11_read_packet
	cmp	al,1
	je	check_reply
	test	al,al
	jz	check_error
	call	x11_queue_event
	jmp	wait_for_reply
    check_reply:
	cmp	bx,[esi+2]
	jne	wait_for_reply
	pop	ebx
	clc
	retn
    check_error:
	cmp	bx,[esi+2]
	je	reply_failed
	call	x11_report_error
	jmp	wait_for_reply
    reply_failed:
	pop	ebx
	stc
	retn

x11_queue_event:
	push	ecx esi edi
	mov	edi,[x11_queue_count]
	cmp	edi,X11_QUEUE_LENGTH
	jae	event_queue_full
	add	edi,[x11_queue_head]
	and	edi,X11_QUEUE_LENGTH-1
	shl	edi,5
	add	edi,x11_event_queue
	mov	ecx,8
	rep	movsd
	inc	[x11_queue_count]
    event_queue_full:
	pop	edi esi ecx
	retn

; x11_dequeue_event: returns CF=0 and esi = event, or CF=1 if queue is empty

x11_dequeue_event:
	cmp	[x11_queue_count],0
	je	event_queue_empty
	push	ecx edi
	mov	esi,[x11_queue_head]
	shl	esi,5
	add	esi,x11_event_queue
	mov	edi,x11_event
	mov	ecx,8
	rep	movsd
	inc	[x11_queue_head]
	and	[x11_queue_head],X11_QUEUE_LENGTH-1
	dec	[x11_queue_count]
	pop	edi ecx
	mov	esi,x11_event
	clc
	retn
    event_queue_empty:
	stc
	retn

; x11_report_error: esi = error packet

x11_report_error:
	push	eax ebx esi
	cmp	byte [esi+10],45
	je	error_reported
	mov	ebx,STDERR
	push	esi
	mov	esi,_error_x11_code
	call	write_string
	pop	esi
	movzx	eax,byte [esi+1]
	call	write_number
	push	esi
	mov	esi,_error_x11_request
	call	write_string
	pop	esi
	movzx	eax,byte [esi+10]
	call	write_number
	mov	esi,_new_line
	call	write_string
    error_reported:
	pop	esi ebx eax
	retn

; x11_rgb_to_pixel: eax = 0RRGGBBh, returns eax = pixel value

x11_rgb_to_pixel:
	cmp	[x11_truecolor],0
	je	rgb_without_truecolor
	push	ebx ecx edx
	mov	edx,eax
	xor	ebx,ebx
	shr	eax,16
	and	eax,0FFh
	mov	ecx,[x11_red_bits]
	call	scale_color_component
	mov	ecx,[x11_red_shift]
	shl	eax,cl
	or	ebx,eax
	movzx	eax,dh
	mov	ecx,[x11_green_bits]
	call	scale_color_component
	mov	ecx,[x11_green_shift]
	shl	eax,cl
	or	ebx,eax
	movzx	eax,dl
	mov	ecx,[x11_blue_bits]
	call	scale_color_component
	mov	ecx,[x11_blue_shift]
	shl	eax,cl
	or	eax,ebx
	pop	edx ecx ebx
	retn
    scale_color_component:
	cmp	ecx,8
	jae	enlarge_color_component
	neg	ecx
	add	ecx,8
	shr	eax,cl
	retn
    enlarge_color_component:
	sub	ecx,8
	shl	eax,cl
	retn
    rgb_without_truecolor:
	push	ecx edx
	mov	edx,eax
	movzx	ecx,dl
	movzx	eax,dh
	add	ecx,eax
	shr	edx,16
	movzx	eax,dl
	add	ecx,eax
	mov	eax,[x11_black_pixel]
	cmp	ecx,3*80h
	jb	rgb_converted
	mov	eax,[x11_white_pixel]
    rgb_converted:
	pop	edx ecx
	retn

; x11_intern_atom: esi = zero-terminated name, returns eax = atom (0 on failure)

x11_intern_atom:
	push	ebx ecx edx esi edi
	mov	edi,esi
	xor	al,al
	or	ecx,-1
	repne	scasb
	not	ecx
	dec	ecx
	mov	edx,ecx
	add	ecx,3
	and	ecx,not 3
	push	ecx
	add	ecx,8
	call	x11_begin_request
	pop	ecx
	mov	ebx,ecx
	shr	ebx,2
	add	ebx,2
	mov	byte [edi],16
	mov	byte [edi+1],0
	mov	[edi+2],bx
	mov	[edi+4],dx
	mov	word [edi+6],0
	add	edi,8
	push	ecx
	mov	ecx,edx
	rep	movsb
	pop	ecx
	sub	ecx,edx
	push	eax
	xor	al,al
	rep	stosb
	pop	eax
	call	x11_wait_reply
	jc	atom_not_interned
	mov	eax,[esi+8]
	pop	edi esi edx ecx ebx
	retn
    atom_not_interned:
	xor	eax,eax
	pop	edi esi edx ecx ebx
	retn

; x11_open_font: esi = zero-terminated name, returns CF=0 and eax = font

x11_open_font:
	push	ebx ecx edx esi edi
	call	x11_new_id
	mov	[x11_opened_font],eax
	mov	edi,esi
	xor	al,al
	or	ecx,-1
	repne	scasb
	not	ecx
	dec	ecx
	mov	edx,ecx
	add	ecx,3
	and	ecx,not 3
	push	ecx
	add	ecx,12
	call	x11_begin_request
	pop	ecx
	mov	ebx,ecx
	shr	ebx,2
	add	ebx,3
	mov	byte [edi],45
	mov	byte [edi+1],0
	mov	[edi+2],bx
	mov	eax,[x11_opened_font]
	mov	[edi+4],eax
	mov	[edi+8],dx
	mov	word [edi+10],0
	add	edi,12
	push	ecx
	mov	ecx,edx
	rep	movsb
	pop	ecx
	sub	ecx,edx
	xor	al,al
	rep	stosb
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],47 + 2 shl 16
	mov	edx,[x11_opened_font]
	mov	[edi+4],edx
	call	x11_wait_reply
	jc	font_not_opened
	mov	eax,[x11_opened_font]
	pop	edi esi edx ecx ebx
	clc
	retn
    font_not_opened:
	pop	edi esi edx ecx ebx
	stc
	retn

; x11_map_window: eax = window

x11_map_window:
	push	eax ecx edi
	push	eax
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],8 + 2 shl 16
	pop	eax
	mov	[edi+4],eax
	pop	edi ecx eax
	retn

; x11_resize_window: eax = window, ecx = width, edx = height

x11_resize_window:
	push	eax ecx edi
	push	eax ecx
	mov	ecx,20
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],12 + 5 shl 16
	mov	[edi+4],eax
	mov	dword [edi+8],4+8
	mov	[edi+12],ecx
	mov	[edi+16],edx
	pop	edi ecx eax
	retn

; x11_set_gc_colors: eax = gc, ecx = foreground pixel, edx = background pixel

x11_set_gc_colors:
	push	eax ecx edi
	push	eax ecx
	mov	ecx,20
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],56 + 5 shl 16
	mov	[edi+4],eax
	mov	dword [edi+8],4+8
	mov	[edi+12],ecx
	mov	[edi+16],edx
	pop	edi ecx eax
	retn

; x11_fill_rectangle: eax = drawable, ebx = gc, ecx = x, edx = y,
; esi = width, edi = height

x11_fill_rectangle:
	push	eax ecx edx esi edi
	push	edi eax ecx
	mov	ecx,20
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],70 + 5 shl 16
	mov	[edi+4],eax
	mov	[edi+8],ebx
	mov	[edi+12],cx
	mov	[edi+14],dx
	mov	[edi+16],si
	pop	eax
	mov	[edi+18],ax
	pop	edi esi edx ecx eax
	retn

; x11_image_text16: eax = drawable, ebx = gc, ecx = x, edx = baseline y,
; esi = characters (CHAR2B), edi = count (1-255)

x11_image_text16:
	push	eax ecx edx esi edi
	push	eax ecx edi
	lea	ecx,[edi*2+3]
	and	ecx,not 3
	push	ecx
	add	ecx,16
	call	x11_begin_request
	pop	ecx
	mov	eax,ecx
	shr	eax,2
	add	eax,4
	shl	eax,16
	mov	al,77
	pop	ecx
	mov	ah,cl
	mov	[edi],eax
	mov	eax,ecx
	pop	ecx
	mov	[edi+12],cx
	mov	[edi+14],dx
	pop	edx
	mov	[edi+4],edx
	mov	[edi+8],ebx
	add	edi,16
	mov	ecx,eax
	shl	ecx,1
	push	ecx
	rep	movsb
	pop	ecx
	neg	ecx
	and	ecx,3
	xor	al,al
	rep	stosb
	pop	edi esi edx ecx eax
	retn

; x11_change_property: eax = window, ebx = property, ecx = type,
; edx = format (8 or 32), esi = data, edi = number of elements

x11_change_property:
	push	eax ebx ecx edx esi edi
	mov	[property_window],eax
	mov	[property_type],ecx
	mov	[property_format],edx
	mov	[property_count],edi
	mov	eax,edi
	cmp	edx,32
	jne	property_size_ok
	shl	eax,2
    property_size_ok:
	mov	[property_size],eax
	lea	ecx,[eax+3]
	and	ecx,not 3
	push	ecx
	add	ecx,24
	call	x11_begin_request
	pop	ecx
	mov	eax,ecx
	shr	eax,2
	add	eax,6
	shl	eax,16
	mov	al,18
	mov	[edi],eax
	mov	eax,[property_window]
	mov	[edi+4],eax
	mov	[edi+8],ebx
	mov	eax,[property_type]
	mov	[edi+12],eax
	mov	eax,[property_format]
	mov	[edi+16],eax
	mov	eax,[property_count]
	mov	[edi+20],eax
	add	edi,24
	push	ecx
	mov	ecx,[property_size]
	rep	movsb
	pop	ecx
	sub	ecx,[property_size]
	xor	al,al
	rep	stosb
	pop	edi esi edx ecx ebx eax
	retn

; x11_create_window: eax = parent, ecx = x, edx = y, esi = width, edi = height,
; [window_value_mask] and [window_values] give attributes
; returns eax = new window

x11_create_window:
	push	ebx ecx edx esi edi
	push	eax ecx edx esi edi
	call	x11_new_id
	mov	[created_window],eax
	xor	ebx,ebx
	mov	edx,[window_value_mask]
    count_window_values:
	test	edx,edx
	jz	window_values_counted
	shr	edx,1
	adc	ebx,0
	jmp	count_window_values
    window_values_counted:
	lea	ecx,[32+ebx*4]
	call	x11_begin_request
	lea	eax,[8+ebx]
	shl	eax,16
	mov	al,1
	mov	[edi],eax
	mov	eax,[created_window]
	mov	[edi+4],eax
	pop	eax
	shl	eax,16
	mov	ax,word [esp]
	add	esp,4
	mov	[edi+16],eax
	pop	edx ecx
	mov	[edi+12],cx
	mov	[edi+14],dx
	pop	eax
	mov	[edi+8],eax
	mov	dword [edi+20],1 shl 16
	mov	dword [edi+24],0
	mov	eax,[window_value_mask]
	mov	[edi+28],eax
	add	edi,32
	mov	esi,window_values
	mov	ecx,ebx
	rep	movsd
	mov	eax,[created_window]
	pop	edi esi edx ecx ebx
	retn

; x11_destroy_window: eax = window

x11_destroy_window:
	push	eax ecx edi
	push	eax
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],4 + 2 shl 16
	pop	eax
	mov	[edi+4],eax
	pop	edi ecx eax
	retn

; x11_unmap_window: eax = window

x11_unmap_window:
	push	eax ecx edi
	push	eax
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],10 + 2 shl 16
	pop	eax
	mov	[edi+4],eax
	pop	edi ecx eax
	retn

; x11_map_subwindows: eax = window

x11_map_subwindows:
	push	eax ecx edi
	push	eax
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],9 + 2 shl 16
	pop	eax
	mov	[edi+4],eax
	pop	edi ecx eax
	retn

; x11_move_resize_window: eax = window, ecx = x, edx = y, esi = width,
; edi = height

x11_move_resize_window:
	push	eax ecx edx edi
	push	edi eax ecx
	mov	ecx,28
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],12 + 7 shl 16
	mov	[edi+4],eax
	mov	dword [edi+8],1+2+4+8
	movsx	ecx,cx
	mov	[edi+12],ecx
	movsx	edx,dx
	mov	[edi+16],edx
	mov	[edi+20],esi
	pop	eax
	mov	[edi+24],eax
	pop	edi edx ecx eax
	retn

; x11_raise_window: eax = window

x11_raise_window:
	push	eax ecx edi
	push	eax
	mov	ecx,16
	call	x11_begin_request
	mov	dword [edi],12 + 4 shl 16
	pop	eax
	mov	[edi+4],eax
	mov	dword [edi+8],40h
	mov	dword [edi+12],0
	pop	edi ecx eax
	retn

; x11_select_input: eax = window, ecx = event mask

x11_select_input:
	push	eax ecx edi
	push	eax ecx
	mov	ecx,16
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],2 + 4 shl 16
	mov	[edi+4],eax
	mov	dword [edi+8],800h
	mov	[edi+12],ecx
	pop	edi ecx eax
	retn

; x11_set_input_focus: eax = window, ecx = time

x11_set_input_focus:
	push	eax ecx edi
	push	eax ecx
	mov	ecx,12
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],42 + 2 shl 8 + 3 shl 16
	mov	[edi+4],eax
	mov	[edi+8],ecx
	pop	edi ecx eax
	retn

; x11_grab_pointer: eax = window, ecx = event mask, edx = time

x11_grab_pointer:
	push	eax ecx edi
	push	eax ecx
	mov	ecx,24
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],26 + 1 shl 8 + 6 shl 16
	mov	[edi+4],eax
	mov	[edi+8],cx
	mov	word [edi+10],1 + 1 shl 8
	mov	dword [edi+12],0
	mov	dword [edi+16],0
	mov	[edi+20],edx
	pop	edi ecx eax
	retn

; x11_ungrab_pointer: eax = time

x11_ungrab_pointer:
	push	eax ecx edi
	push	eax
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],27 + 2 shl 16
	pop	eax
	mov	[edi+4],eax
	pop	edi ecx eax
	retn

; x11_set_gc_font: eax = gc, ecx = font

x11_set_gc_font:
	push	eax ecx edi
	push	eax ecx
	mov	ecx,16
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],56 + 4 shl 16
	mov	[edi+4],eax
	mov	dword [edi+8],4000h
	mov	[edi+12],ecx
	pop	edi ecx eax
	retn

; x11_create_gc: eax = drawable, ecx = value mask, esi = values
; returns eax = gc

x11_create_gc:
	push	ebx ecx edx esi edi
	push	eax
	call	x11_new_id
	mov	edx,eax
	xor	ebx,ebx
	mov	eax,ecx
    count_gc_values:
	test	eax,eax
	jz	gc_values_counted
	shr	eax,1
	adc	ebx,0
	jmp	count_gc_values
    gc_values_counted:
	push	ecx
	lea	ecx,[16+ebx*4]
	call	x11_begin_request
	pop	ecx
	lea	eax,[4+ebx]
	shl	eax,16
	mov	al,55
	mov	[edi],eax
	mov	[edi+4],edx
	pop	eax
	mov	[edi+8],eax
	mov	[edi+12],ecx
	add	edi,16
	mov	ecx,ebx
	rep	movsd
	mov	eax,edx
	pop	edi esi edx ecx ebx
	retn

; x11_close_font: eax = font

x11_close_font:
	push	eax ecx edi
	push	eax
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],46 + 2 shl 16
	pop	eax
	mov	[edi+4],eax
	pop	edi ecx eax
	retn

; x11_draw_line: eax = drawable, ebx = gc, ecx = x1, edx = y1, esi = x2,
; edi = y2

x11_draw_line:
	push	eax ecx edx esi edi
	push	edi eax ecx
	mov	ecx,20
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],66 + 5 shl 16
	mov	[edi+4],eax
	mov	[edi+8],ebx
	mov	[edi+12],cx
	mov	[edi+14],dx
	mov	[edi+16],si
	pop	eax
	mov	[edi+18],ax
	pop	edi esi edx ecx eax
	retn

; x11_fill_arc: eax = drawable, ebx = gc, ecx = x, edx = y, esi = width,
; edi = height (full ellipse); x11_draw_arc draws the outline only

x11_draw_arc:
	push	eax
	mov	al,68
	jmp	arc_request
x11_fill_arc:
	push	eax
	mov	al,71
    arc_request:
	mov	[arc_opcode],al
	pop	eax
	push	eax ecx edx esi edi
	push	edi eax ecx
	mov	ecx,24
	call	x11_begin_request
	pop	ecx eax
	movzx	esi,[arc_opcode]
	or	esi,6 shl 16
	mov	[edi],esi
	mov	[edi+4],eax
	mov	[edi+8],ebx
	mov	[edi+12],cx
	mov	[edi+14],dx
	mov	esi,[esp+8]
	mov	[edi+16],si
	pop	eax
	mov	[edi+18],ax
	mov	word [edi+20],0
	mov	word [edi+22],360*64
	pop	edi esi edx ecx eax
	retn

; x11_get_property: eax = window, ecx = property, edx = delete flag,
; returns CF=0 and esi = reply (format at [esi+1], type at [esi+8],
; bytes after at [esi+12], length in format units at [esi+16], data at [esi+32])

x11_get_property:
	push	eax ebx ecx edx edi
	push	eax ecx edx
	mov	ecx,24
	call	x11_begin_request
	pop	edx ecx ebx
	shl	edx,8
	or	edx,20 + 6 shl 16
	mov	[edi],edx
	mov	[edi+4],ebx
	mov	[edi+8],ecx
	mov	dword [edi+12],0
	mov	edx,[property_offset]
	mov	[edi+16],edx
	mov	dword [edi+20],(X11_PACKET_SIZE-32)/4
	call	x11_wait_reply
	pop	edi edx ecx ebx eax
	retn

; x11_set_selection_owner: eax = owner, ecx = selection, edx = time

x11_set_selection_owner:
	push	eax ecx edi
	push	eax ecx
	mov	ecx,16
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],22 + 4 shl 16
	mov	[edi+4],eax
	mov	[edi+8],ecx
	mov	[edi+12],edx
	pop	edi ecx eax
	retn

; x11_get_selection_owner: ecx = selection, returns eax = owner window

x11_get_selection_owner:
	push	ecx esi edi
	push	ecx
	mov	ecx,8
	call	x11_begin_request
	pop	ecx
	mov	dword [edi],23 + 2 shl 16
	mov	[edi+4],ecx
	call	x11_wait_reply
	mov	eax,0
	jc	selection_owner_known
	mov	eax,[esi+8]
    selection_owner_known:
	pop	edi esi ecx
	retn

; x11_convert_selection: eax = requestor, ecx = selection, edx = target,
; esi = property, edi = time

x11_convert_selection:
	push	eax ecx edi
	push	edi eax ecx
	mov	ecx,24
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],24 + 6 shl 16
	mov	[edi+4],eax
	mov	[edi+8],ecx
	mov	[edi+12],edx
	mov	[edi+16],esi
	pop	eax
	mov	[edi+20],eax
	pop	edi ecx eax
	retn

; x11_send_event: eax = destination, ecx = event mask, esi = event (32 bytes)

x11_send_event:
	push	eax ecx esi edi
	push	eax ecx
	mov	ecx,44
	call	x11_begin_request
	pop	ecx eax
	mov	dword [edi],25 + 11 shl 16
	mov	[edi+4],eax
	mov	[edi+8],ecx
	add	edi,12
	mov	ecx,8
	rep	movsd
	pop	edi esi ecx eax
	retn

; x11_translate_coordinates: eax = source window, ecx = x, edx = y
; returns ecx, edx = coordinates relative to the root window

x11_translate_coordinates:
	push	eax ebx esi edi
	push	eax ecx edx
	mov	ecx,16
	call	x11_begin_request
	mov	ebx,eax
	pop	edx ecx eax
	mov	dword [edi],40 + 4 shl 16
	mov	[edi+4],eax
	mov	eax,[x11_root]
	mov	[edi+8],eax
	mov	[edi+12],cx
	mov	[edi+14],dx
	mov	eax,ebx
	call	x11_wait_reply
	jc	coordinates_translated
	movsx	ecx,word [esi+12]
	movsx	edx,word [esi+14]
    coordinates_translated:
	pop	edi esi ebx eax
	retn

; x11_bell

x11_bell:
	push	eax ecx edi
	mov	ecx,4
	call	x11_begin_request
	mov	dword [edi],104 + 1 shl 16
	pop	edi ecx eax
	retn

; x11_list_fonts: esi = zero-terminated pattern
; returns CF=0 and esi = reply (number of names at [esi+8], names at [esi+32])

x11_list_fonts:
	push	eax ebx ecx edx edi
	call	string_length
	mov	edx,ecx
	add	ecx,3
	and	ecx,not 3
	push	ecx
	add	ecx,8
	call	x11_begin_request
	pop	ecx
	mov	ebx,ecx
	shr	ebx,2
	add	ebx,2
	shl	ebx,16
	mov	bl,49
	mov	[edi],ebx
	mov	word [edi+4],4000
	mov	[edi+6],dx
	add	edi,8
	push	ecx
	mov	ecx,edx
	rep	movsb
	pop	ecx
	sub	ecx,edx
	push	eax
	xor	al,al
	rep	stosb
	pop	eax
	call	x11_wait_reply
	pop	edi edx ecx ebx eax
	retn
