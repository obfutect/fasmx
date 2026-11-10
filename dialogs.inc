
; fasmx - flat assembler IDE for Linux/X11
; dialogs (ports of FASMW dialog procedures and replacements of common dialogs)

; helpers for dialog procedures, working on [current_dialog]

; dlg_set_text: eax = id, esi = UTF-8 text

dlg_set_text:
	push	ebx
	call	dialog_item
	test	ebx,ebx
	jz	dlg_text_set
	call	set_control_text
    dlg_text_set:
	pop	ebx
	retn

; dlg_get_text: eax = id, edi = buffer, ecx = size, returns eax = length

dlg_get_text:
	push	ebx
	call	dialog_item
	test	ebx,ebx
	jz	dlg_no_text
	call	get_control_text
	pop	ebx
	retn
    dlg_no_text:
	mov	byte [edi],0
	xor	eax,eax
	pop	ebx
	retn

; dlg_set_int: eax = id, edx = value

dlg_set_int:
	push	edi esi
	mov	edi,number_text
	push	eax
	mov	eax,edx
	call	format_number
	pop	eax
	mov	esi,number_text
	call	dlg_set_text
	pop	esi edi
	retn

; dlg_get_int: eax = id, returns eax = value (0 if not a number)

dlg_get_int:
	push	ecx esi edi
	mov	edi,number_text
	mov	ecx,32
	call	dlg_get_text
	mov	esi,number_text
	call	parse_number
	jnc	dlg_int_ok
	xor	eax,eax
    dlg_int_ok:
	pop	edi esi ecx
	retn

; dlg_check: eax = id, edx = state

dlg_check:
	push	ebx
	call	dialog_item
	test	ebx,ebx
	jz	dlg_checked
	and	dword [ebx+C_STATE],not CS_CHECKED
	test	edx,edx
	jz	dlg_check_redraw
	cmp	dword [ebx+C_TYPE],CT_RADIO
	jne	dlg_check_box
	call	select_radio
	jmp	dlg_checked
    dlg_check_box:
	or	dword [ebx+C_STATE],CS_CHECKED
    dlg_check_redraw:
	call	control_redraw
    dlg_checked:
	pop	ebx
	retn

; dlg_is_checked: eax = id, returns eax = 1 if checked

dlg_is_checked:
	push	ebx
	call	dialog_item
	xor	eax,eax
	test	ebx,ebx
	jz	dlg_check_state
	test	dword [ebx+C_STATE],CS_CHECKED
	jz	dlg_check_state
	inc	eax
    dlg_check_state:
	pop	ebx
	retn

; dlg_enable: eax = id, edx = state

dlg_enable:
	push	ebx
	call	dialog_item
	test	ebx,ebx
	jz	dlg_enabled
	mov	eax,[ebx+C_STATE]
	and	dword [ebx+C_STATE],not CS_DISABLED
	test	edx,edx
	jnz	dlg_enable_changed
	or	dword [ebx+C_STATE],CS_DISABLED
    dlg_enable_changed:
	cmp	eax,[ebx+C_STATE]
	je	dlg_enabled
	call	control_redraw
    dlg_enabled:
	pop	ebx
	retn

; dlg_set_focus: eax = id

dlg_set_focus:
	push	ebx ebp
	call	dialog_item
	test	ebx,ebx
	jz	dlg_focus_set
	mov	ebp,[current_dialog]
	mov	[keyboard_focus_change],1
	call	set_dialog_focus
    dlg_focus_set:
	pop	ebp ebx
	retn

; dlg_set_icon: eax = id, edx = icon kind

dlg_set_icon:
	push	ebx
	call	dialog_item
	test	ebx,ebx
	jz	dlg_icon_set
	and	dword [ebx+C_STYLE],0FFFFFFh
	shl	edx,24
	or	[ebx+C_STYLE],edx
	call	control_redraw
    dlg_icon_set:
	pop	ebx
	retn

; dlg_combo_add: eax = id, esi = string

dlg_combo_add:
	push	ebx
	call	dialog_item
	test	ebx,ebx
	jz	dlg_combo_done
	call	combo_add_string
    dlg_combo_done:
	pop	ebx
	retn

; dlg_combo_add_strings: eax = id, esi = list of strings (ends with empty one)

dlg_combo_add_strings:
	push	esi
    add_combo_string:
	cmp	byte [esi],0
	je	combo_strings_added
	push	eax
	call	dlg_combo_add
	pop	eax
    skip_combo_string:
	lodsb
	test	al,al
	jnz	skip_combo_string
	jmp	add_combo_string
    combo_strings_added:
	pop	esi
	retn

; dlg_combo_set_selection: eax = id, edx = index

dlg_combo_set_selection:
	push	ebx
	call	dialog_item
	test	ebx,ebx
	jz	dlg_combo_done
	mov	eax,edx
	call	combo_set_selection
	pop	ebx
	retn

; dlg_combo_get_selection: eax = id, returns eax = index

dlg_combo_get_selection:
	push	ebx
	call	dialog_item
	or	eax,-1
	test	ebx,ebx
	jz	dlg_combo_done
	call	combo_get_selection
	pop	ebx
	retn

; dlg_combo_find: eax = id, esi = string, returns eax = index (-1 if none)

dlg_combo_find:
	push	ebx
	call	dialog_item
	or	eax,-1
	test	ebx,ebx
	jz	dlg_combo_done
	call	combo_find_string
	pop	ebx
	retn

; history of searched texts

HISTORY_LENGTH = 32

init_history:
	mov	ebx,history_list
	cmp	dword [ebx+L_STRINGS],0
	jne	history_ready
	call	listbox_init
    history_ready:
	retn

; history_to_combo: eax = id of combo box

history_to_combo:
	push	ebx ecx esi
	push	eax
	call	init_history
	pop	eax
	xor	ecx,ecx
    add_history_string:
	cmp	ecx,[history_list+L_COUNT]
	jae	history_added
	push	eax ecx
	mov	ebx,history_list
	mov	eax,ecx
	call	list_get_string
	pop	ecx eax
	push	eax ecx
	call	dlg_combo_add
	pop	ecx eax
	inc	ecx
	jmp	add_history_string
    history_added:
	pop	esi ecx ebx
	retn

; add_to_history: esi = string

add_to_history:
	cmp	byte [esi],0
	je	history_updated
	push	esi
	call	init_history
	pop	esi
	mov	ebx,history_list
	call	list_find_string
	cmp	eax,-1
	je	insert_history_string
	call	list_delete_string
    insert_history_string:
	xor	eax,eax
	call	list_insert_string
	cmp	dword [ebx+L_COUNT],HISTORY_LENGTH
	jbe	history_updated
	mov	eax,[ebx+L_COUNT]
	dec	eax
	call	list_delete_string
    history_updated:
	retn

; cells_to_utf8: esi = zero-terminated cells, edi = buffer, ecx = size

cells_to_utf8:
	push	eax edx esi edi
	lea	edx,[edi+ecx-5]
    convert_cell_to_utf8:
	cmp	edi,edx
	jae	cells_converted
	lodsd
	test	eax,eax
	jz	cells_converted
	call	utf8_encode
	jmp	convert_cell_to_utf8
    cells_converted:
	mov	byte [edi],0
	pop	edi esi edx eax
	retn

; utf8_string_to_cells: esi = zero-terminated UTF-8, edi = buffer for cells

utf8_string_to_cells:
	push	ecx esi edi
	call	string_length
	call	utf8_to_cells
	pop	edi esi ecx
	retn

; Position dialog

position_procedure:
	cmp	eax,DM_INIT
	je	position_init
	cmp	eax,DM_COMMAND
	je	position_command_message
	cmp	eax,DM_CLOSE
	je	position_close
	retn
    position_init:
	mov	[fe_wparam],fepos
	mov	edx,FM_GETPOS
	call	send_current_editor
	mov	eax,ID_ROW
	mov	edx,[fepos+FEPOS_CARET_LINE]
	call	dlg_set_int
	mov	eax,ID_COLUMN
	mov	edx,[fepos+FEPOS_CARET_POSITION]
	jmp	dlg_set_int
    position_command_message:
	cmp	edx,IDCANCEL
	je	position_close
	cmp	edx,IDOK
	jne	position_done
	mov	eax,ID_ROW
	call	dlg_get_int
	mov	[fepos+FEPOS_CARET_LINE],eax
	mov	[fepos+FEPOS_SELECTION_LINE],eax
	mov	eax,ID_COLUMN
	call	dlg_get_int
	mov	[fepos+FEPOS_CARET_POSITION],eax
	mov	[fepos+FEPOS_SELECTION_POSITION],eax
	mov	eax,ID_SELECT
	call	dlg_is_checked
	test	eax,eax
	jz	position_set
	mov	[fepos+FEPOS_SELECTION_LINE],0
	mov	[fepos+FEPOS_SELECTION_POSITION],0
    position_set:
	mov	[fe_wparam],fepos
	mov	edx,FM_SETPOS
	call	send_current_editor
	mov	eax,1
	jmp	end_dialog
    position_close:
	xor	eax,eax
	jmp	end_dialog
    position_done:
	retn

position_command:
	mov	esi,position_dialog
	mov	edi,position_procedure
	xor	eax,eax
	jmp	dialog_box

; Find dialog

find_procedure:
	cmp	eax,DM_INIT
	je	find_init
	cmp	eax,DM_COMMAND
	je	find_command_message
	cmp	eax,DM_CLOSE
	je	find_close
	retn
    find_init:
	mov	[fe_wparam],1000h
	mov	[fe_lparam],search_cells
	mov	edx,FM_GETWORDATCARET
	call	send_current_editor
	mov	esi,search_cells
	mov	edi,search_string
	mov	ecx,1000h
	call	cells_to_utf8
	mov	eax,ID_TEXT
	call	history_to_combo
	mov	eax,ID_TEXT
	mov	esi,search_string
	call	dlg_set_text
	mov	eax,ID_CASESENSITIVE
	mov	edx,[search_settings]
	and	edx,FEFIND_CASESENSITIVE
	call	dlg_check
	mov	eax,ID_WHOLEWORDS
	mov	edx,[search_settings]
	and	edx,FEFIND_WHOLEWORDS
	call	dlg_check
	mov	eax,ID_BACKWARD
	mov	edx,[search_settings]
	and	edx,FEFIND_BACKWARD
	call	dlg_check
	mov	eax,ID_INWHOLETEXT
	mov	edx,[search_settings]
	and	edx,FEFIND_INWHOLETEXT
	call	dlg_check
	jmp	find_update
    find_command_message:
	cmp	edx,ID_TEXT
	je	find_update
	cmp	edx,IDCANCEL
	je	find_close
	cmp	edx,IDOK
	jne	find_done
	xor	ebx,ebx
	mov	eax,ID_CASESENSITIVE
	call	dlg_is_checked
	test	eax,eax
	jz	find_casesensitive_ok
	or	ebx,FEFIND_CASESENSITIVE
    find_casesensitive_ok:
	mov	eax,ID_WHOLEWORDS
	call	dlg_is_checked
	test	eax,eax
	jz	find_wholewords_ok
	or	ebx,FEFIND_WHOLEWORDS
    find_wholewords_ok:
	mov	eax,ID_BACKWARD
	call	dlg_is_checked
	test	eax,eax
	jz	find_backward_ok
	or	ebx,FEFIND_BACKWARD
    find_backward_ok:
	mov	eax,ID_INWHOLETEXT
	call	dlg_is_checked
	test	eax,eax
	jz	find_inwholetext_ok
	or	ebx,FEFIND_INWHOLETEXT
    find_inwholetext_ok:
	mov	[search_settings],ebx
	mov	eax,ID_TEXT
	mov	edi,search_string
	mov	ecx,1000h
	call	dlg_get_text
	mov	esi,search_string
	call	add_to_history
	mov	eax,1
	jmp	end_dialog
    find_update:
	mov	eax,ID_TEXT
	mov	edi,search_string
	mov	ecx,1000h
	call	dlg_get_text
	xor	edx,edx
	cmp	[search_string],0
	setnz	dl
	mov	eax,IDOK
	jmp	dlg_enable
    find_close:
	xor	eax,eax
	jmp	end_dialog
    find_done:
	retn

; Replace dialog

replace_procedure:
	cmp	eax,DM_INIT
	je	replace_init
	cmp	eax,DM_COMMAND
	jne	find_procedure
	cmp	edx,IDOK
	jne	find_procedure
	push	eax ecx edx
	mov	eax,ID_NEWTEXT
	mov	edi,replace_string
	mov	ecx,1000h
	call	dlg_get_text
	xor	ebx,ebx
	mov	eax,ID_PROMPT
	call	dlg_is_checked
	test	eax,eax
	jz	replace_prompt_ok
	or	ebx,1
    replace_prompt_ok:
	mov	[command_flags],ebx
	mov	esi,replace_string
	call	add_to_history
	pop	edx ecx eax
	jmp	find_procedure
    replace_init:
	mov	eax,ID_NEWTEXT
	call	history_to_combo
	mov	eax,ID_NEWTEXT
	mov	esi,replace_string
	call	dlg_set_text
	mov	eax,ID_PROMPT
	mov	edx,[command_flags]
	and	edx,1
	call	dlg_check
	mov	eax,DM_INIT
	jmp	find_procedure

; find_command: Search > Find

find_command:
	mov	esi,find_dialog
	mov	edi,find_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,1
	jne	search_done
	mov	esi,search_string
	mov	edi,search_cells
	call	utf8_string_to_cells
	mov	eax,[search_settings]
	mov	[fe_wparam],eax
	mov	[fe_lparam],search_cells
	mov	edx,FM_FINDFIRST
	call	send_current_editor
	test	eax,eax
	jnz	search_done
    text_not_found_message:
	mov	[search_text_size],1000h
	mov	[fe_wparam],search_cells
	mov	[fe_lparam],search_text_size
	mov	edx,FM_GETSEARCHTEXT
	call	send_current_editor
	mov	esi,search_cells
	mov	edi,search_string
	mov	ecx,1000h
	call	cells_to_utf8
	mov	edx,FM_GETSEARCHFLAGS
	call	send_current_editor
	mov	ebx,_not_found
	test	eax,FEFIND_INWHOLETEXT
	jnz	make_not_found_message
	mov	ebx,_not_found_after
	test	eax,FEFIND_BACKWARD
	jz	make_not_found_message
	mov	ebx,_not_found_before
    make_not_found_message:
	mov	edi,string_buffer
	mov	esi,_text_prefix
	call	copy_text
	mov	esi,search_string
	call	copy_text
	mov	esi,ebx
	call	copy_text
	mov	edx,FM_RELEASESEARCH
	call	send_current_editor
	mov	esi,string_buffer
	mov	edi,_find
	mov	eax,MB_ICONINFORMATION+MB_OK
	call	message_box
    search_done:
	retn

findnext_command:
	mov	edx,FM_FINDNEXT
	call	send_current_editor
	test	eax,eax
	jz	text_not_found_message
	retn

; replace_command: Search > Replace

replace_command:
	mov	esi,replace_dialog
	mov	edi,replace_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,1
	jne	search_done
	call	current_editor
	mov	[replace_editor],eax
	mov	[replaces_count],0
	mov	esi,replace_string
	mov	edi,replace_cells
	call	utf8_string_to_cells
	mov	esi,search_string
	mov	edi,search_cells
	call	utf8_string_to_cells
	mov	eax,[replace_editor]
	mov	edx,FM_BEGINOPERATION
	call	send_editor
	test	[command_flags],1
	jz	start_replacing
	mov	eax,[replace_editor]
	mov	edx,FM_ENDOPERATION
	call	send_editor
    start_replacing:
	mov	eax,[search_settings]
	mov	[fe_wparam],eax
	mov	[fe_lparam],search_cells
	mov	eax,[replace_editor]
	mov	edx,FM_FINDFIRST
	call	send_editor
	test	eax,eax
	jz	replace_not_found
	mov	eax,[replace_editor]
	mov	edx,FM_GETMODE
	call	send_editor
	mov	[replace_mode],eax
	and	eax,not (FEMODE_VERTICALSEL + FEMODE_OVERWRITE)
	mov	[fe_wparam],eax
	mov	eax,[replace_editor]
	mov	edx,FM_SETMODE
	call	send_editor
    confirm_replace:
	test	[command_flags],1
	jz	do_replace
	call	x11_flush
	mov	esi,_replace_prompt
	mov	edi,_replace
	mov	eax,MB_ICONQUESTION+MB_YESNOCANCEL
	call	message_box
	cmp	eax,IDCANCEL
	je	replace_finish
	cmp	eax,IDNO
	je	replace_next
    do_replace:
	mov	[fe_wparam],0
	mov	[fe_lparam],replace_cells
	mov	eax,[replace_editor]
	mov	edx,FM_REPLACESEL
	call	send_editor
	inc	[replaces_count]
    replace_next:
	mov	eax,[replace_editor]
	mov	edx,FM_FINDNEXT
	call	send_editor
	test	eax,eax
	jnz	confirm_replace
    replace_finish:
	mov	eax,[replace_mode]
	mov	[fe_wparam],eax
	mov	eax,[replace_editor]
	mov	edx,FM_SETMODE
	call	send_editor
	test	[command_flags],1
	jnz	replace_summary
	mov	eax,[replace_editor]
	mov	edx,FM_ENDOPERATION
	call	send_editor
    replace_summary:
	mov	eax,[replace_editor]
	mov	edx,FM_RELEASESEARCH
	call	send_editor
	mov	edi,string_buffer
	mov	eax,[replaces_count]
	call	format_number
	mov	esi,_replaces_made
	call	copy_text
	mov	esi,string_buffer
	mov	edi,_find
	mov	eax,MB_ICONINFORMATION+MB_OK
	jmp	message_box
    replace_not_found:
	mov	eax,[replace_editor]
	mov	edx,FM_ENDOPERATION
	call	send_editor
	jmp	text_not_found_message

; Summary dialogs (after compilation)

summary_procedure:
	cmp	eax,DM_INIT
	je	summary_init
	cmp	eax,DM_COMMAND
	je	summary_command
	cmp	eax,DM_CLOSE
	je	summary_close
	retn
    summary_init:
	mov	esi,[compile_results]
	add	esi,[esi+CR_DISPLAY]
	mov	eax,ID_DISPLAY
	call	dlg_set_text
	mov	ebp,[current_dialog]
	mov	eax,[ebp+D_PARAM]
	cmp	eax,2
	je	summary_error_details
	test	eax,eax
	jnz	summary_error_message
	mov	edi,string_buffer
	mov	esi,[compile_results]
	mov	eax,[esi+CR_PASSES]
	call	format_number
	mov	esi,_passes_suffix
	call	copy_text
	mov	esi,[compile_results]
	mov	eax,[esi+CR_TIME]
	xor	edx,edx
	mov	ecx,100
	div	ecx
	test	eax,eax
	jz	summary_time_done
	xor	edx,edx
	mov	ecx,10
	div	ecx
	call	format_number
	mov	byte [edi],'.'
	inc	edi
	mov	eax,edx
	call	format_number
	mov	esi,_seconds_suffix
	call	copy_text
    summary_time_done:
	mov	esi,[compile_results]
	mov	eax,[esi+CR_WRITTEN]
	call	format_number
	mov	esi,_bytes_suffix
	call	copy_text
    summary_message_ready:
	mov	eax,ID_MESSAGE
	mov	esi,string_buffer
	jmp	dlg_set_text
    summary_error_details:
	mov	esi,[compile_results]
	mov	ecx,[esi+CR_LINES_COUNT]
	xor	ebx,ebx
    add_error_line:
	cmp	ebx,ecx
	jae	error_lines_added
	push	ebx ecx
	mov	esi,[compile_results]
	mov	edx,[esi+CR_LINES+ebx*8]
	add	edx,esi
	mov	eax,[esi+CR_LINES+ebx*8+4]
	mov	edi,string_buffer
	push	eax
	mov	esi,edx
	call	copy_text
	mov	byte [edi],' '
	mov	byte [edi+1],'['
	add	edi,2
	pop	eax
	call	format_number
	mov	byte [edi],']'
	mov	byte [edi+1],0
	mov	eax,ID_LINES
	call	dialog_item
	mov	esi,string_buffer
	call	list_add_string
	pop	ecx ebx
	inc	ebx
	jmp	add_error_line
    error_lines_added:
	mov	esi,[compile_results]
	add	esi,[esi+CR_INSTRUCTION]
	mov	eax,ID_INSTRUCTION
	call	dlg_set_text
	mov	eax,ID_LINES
	call	dialog_item
	xor	eax,eax
	call	list_set_selection
    summary_error_message:
	mov	eax,ID_ICON
	mov	edx,ICON_ERROR
	call	dlg_set_icon
	mov	edi,string_buffer
	mov	esi,_error_message_prefix
	call	copy_text
	mov	esi,[compile_results]
	mov	esi,[esi+CR_MESSAGE]
	test	esi,esi
	jz	summary_message_ready
	call	copy_text
	mov	byte [edi],'.'
	mov	byte [edi+1],0
	jmp	summary_message_ready
    summary_command:
	cmp	edx,ID_LINES
	je	summary_show_line
	cmp	edx,IDCANCEL
	je	summary_close
	cmp	edx,IDOK
	jne	summary_done
	mov	eax,1
	jmp	end_dialog
    summary_show_line:
	cmp	ecx,LBN_SELCHANGE
	jne	summary_done
	mov	eax,ID_LINES
	call	dialog_item
	mov	eax,[ebx+L_SELECTION]
	cmp	eax,0
	jl	summary_done
	mov	esi,[compile_results]
	cmp	eax,[esi+CR_LINES_COUNT]
	jae	summary_done
	mov	edx,[esi+CR_LINES+eax*8+4]
	mov	eax,[esi+CR_LINES+eax*8]
	add	eax,esi
	jmp	show_line
    summary_close:
	xor	eax,eax
	jmp	end_dialog
    summary_done:
	retn

; Appearance dialog

appearance_procedure:
	cmp	eax,DM_INIT
	je	appearance_init
	cmp	eax,DM_COMMAND
	je	appearance_command_message
	cmp	eax,DM_CLOSE
	je	appearance_close
	retn
    appearance_init:
	mov	eax,ID_CONSOLECARET
	mov	edx,[fedit_style]
	and	edx,FES_CONSOLECARET
	call	dlg_check
	mov	esi,editor_colors
	mov	edi,tmp_colors
	mov	ecx,8
	rep	movsd
	mov	esi,editor_font_record
	mov	edi,tmp_font_record
	mov	ecx,FONT_RECORD_SIZE/4
	rep	movsd
	mov	esi,font_name
	mov	edi,tmp_font_name
	call	copy_text
	mov	eax,ID_SETTING
	mov	esi,_appearance_settings
	call	dlg_combo_add_strings
	mov	eax,ID_SETTING
	xor	edx,edx
	call	dlg_combo_set_selection
	mov	eax,ID_PREVIEW
	mov	esi,preview_text
	call	dlg_set_text
	call	appearance_preview_editor
	mov	[fe_wparam],preview_selection
	mov	edx,FM_SETPOS
	call	send_editor
	call	appearance_preview_editor
	mov	[fe_wparam],tmp_font_record
	mov	edx,FM_SETFONT
	call	send_editor
    appearance_update_colors:
	call	appearance_preview_editor
	mov	ecx,[tmp_colors]
	mov	[fe_wparam],ecx
	mov	ecx,[tmp_colors+4]
	mov	[fe_lparam],ecx
	mov	edx,FM_SETTEXTCOLOR
	call	send_editor
	call	appearance_preview_editor
	mov	ecx,[tmp_colors+8]
	mov	[fe_wparam],ecx
	mov	ecx,[tmp_colors+12]
	mov	[fe_lparam],ecx
	mov	edx,FM_SETSELCOLOR
	call	send_editor
	call	appearance_preview_editor
	mov	[fe_wparam],tmp_colors+16
	mov	[fe_lparam],fasm_syntax
	mov	edx,FM_SETSYNTAXHIGHLIGHT
	jmp	send_editor
    appearance_command_message:
	cmp	edx,IDCANCEL
	je	appearance_close
	cmp	edx,IDOK
	je	appearance_ok
	cmp	edx,ID_CHANGE
	jne	appearance_done
	mov	eax,ID_SETTING
	call	dlg_combo_get_selection
	test	eax,eax
	jz	appearance_change_font
	cmp	eax,8
	ja	appearance_done
	lea	ebx,[tmp_colors+(eax-1)*4]
	push	ebx
	mov	eax,[ebx]
	call	choose_color
	pop	ebx
	jc	appearance_done
	mov	[ebx],eax
	jmp	appearance_update_colors
    appearance_change_font:
	mov	esi,tmp_font_name
	call	choose_font
	jc	appearance_done
	mov	esi,chosen_font_name
	call	x11_open_font
	jc	appearance_done
	mov	edi,tmp_font_record
	call	make_font_record
	mov	esi,chosen_font_name
	mov	edi,tmp_font_name
	call	copy_text
	call	appearance_preview_editor
	mov	[fe_wparam],tmp_font_record
	mov	edx,FM_SETFONT
	jmp	send_editor
    appearance_ok:
	mov	esi,tmp_colors
	mov	edi,editor_colors
	mov	ecx,8
	rep	movsd
	mov	esi,tmp_font_record
	mov	edi,editor_font_record
	mov	ecx,FONT_RECORD_SIZE/4
	rep	movsd
	mov	eax,[editor_font_record]
	mov	[editor_font_id],eax
	mov	esi,tmp_font_name
	mov	edi,font_name
	call	copy_text
	mov	eax,ID_CONSOLECARET
	call	dlg_is_checked
	and	[fedit_style],not FES_CONSOLECARET
	test	eax,eax
	jz	appearance_caret_ok
	or	[fedit_style],FES_CONSOLECARET
    appearance_caret_ok:
	mov	eax,1
	jmp	end_dialog
    appearance_close:
	xor	eax,eax
	jmp	end_dialog
    appearance_done:
	retn
    appearance_preview_editor:
	mov	eax,ID_PREVIEW
	call	dialog_item
	mov	eax,[ebx+F_EDITOR]
	retn

appearance_command:
	mov	esi,appearance_dialog
	mov	edi,appearance_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,1
	jne	appearance_done
	mov	eax,[current_tab]
	jmp	fm_select

; Compiler setup dialog

compiler_setup_procedure:
	cmp	eax,DM_INIT
	je	compiler_setup_init
	cmp	eax,DM_COMMAND
	je	compiler_setup_command_message
	cmp	eax,DM_CLOSE
	je	compiler_setup_close
	retn
    compiler_setup_init:
	mov	eax,ID_INCLUDEPATH
	mov	esi,include_path
	call	dlg_combo_add
	mov	esi,_include_variable
	call	find_environment_variable
	mov	esi,eax
	test	esi,esi
	jnz	include_default_ok
	mov	esi,_empty_string
    include_default_ok:
	mov	edi,path_buffer
	call	copy_text
	mov	esi,_section_environment
	mov	edx,_key_environment_include
	mov	edi,path_buffer
	mov	ecx,4000h
	call	ini_get
	mov	eax,ID_INCLUDEPATH
	mov	esi,path_buffer
	call	dlg_set_text
	mov	eax,ID_MEMORY
	mov	esi,_memory_settings
	call	dlg_combo_add_strings
	mov	edi,number_text
	mov	eax,[compiler_memory]
	call	format_number
	mov	eax,ID_MEMORY
	mov	esi,number_text
	call	dlg_combo_find
	cmp	eax,-1
	je	compiler_memory_text
	mov	edx,eax
	mov	eax,ID_MEMORY
	call	dlg_combo_set_selection
	jmp	compiler_memory_shown
    compiler_memory_text:
	mov	eax,ID_MEMORY
	mov	esi,number_text
	call	dlg_set_text
    compiler_memory_shown:
	mov	eax,ID_PASSES
	movzx	edx,[passes_limit]
	call	dlg_set_int
	mov	eax,ID_PRIORITY
	mov	esi,_priority_settings
	call	dlg_combo_add_strings
	call	priority_index
	mov	edx,eax
	mov	eax,ID_PRIORITY
	jmp	dlg_combo_set_selection
    compiler_setup_command_message:
	cmp	edx,IDCANCEL
	je	compiler_setup_close
	cmp	edx,IDOK
	jne	compiler_setup_done
	mov	eax,ID_MEMORY
	call	dlg_get_int
	test	eax,eax
	jz	compiler_memory_set
	mov	[compiler_memory],eax
    compiler_memory_set:
	mov	eax,ID_PASSES
	call	dlg_get_int
	cmp	eax,0FFFFh
	jbe	compiler_passes_ok
	mov	eax,0FFFFh
    compiler_passes_ok:
	test	eax,eax
	jz	compiler_passes_set
	mov	[passes_limit],ax
    compiler_passes_set:
	mov	eax,ID_PRIORITY
	call	dlg_combo_get_selection
	cmp	eax,4
	ja	compiler_priority_set
	mov	eax,[priority_values+eax*4]
	mov	[compiler_priority],eax
    compiler_priority_set:
	mov	eax,ID_INCLUDEPATH
	mov	edi,path_buffer
	mov	ecx,4000h
	call	dlg_get_text
	mov	esi,_section_environment
	mov	edx,_key_environment_include
	mov	ebx,path_buffer
	call	ini_set
	call	ini_flush
	mov	eax,1
	jmp	end_dialog
    compiler_setup_close:
	xor	eax,eax
	jmp	end_dialog
    compiler_setup_done:
	retn

; priority_index: returns eax = index of [compiler_priority] in priority list

priority_index:
	mov	eax,[compiler_priority]
	cmp	eax,2
	jg	priority_realtime
	cmp	eax,-2
	jl	priority_idle
	sar	eax,1
	add	eax,2
	retn
    priority_realtime:
	mov	eax,4
	retn
    priority_idle:
	xor	eax,eax
	retn

compiler_setup_command:
	mov	esi,compiler_setup_dialog
	mov	edi,compiler_setup_procedure
	xor	eax,eax
	jmp	dialog_box

; Calculator dialog (uses the expression evaluator of assembler)

calculator_procedure:
	cmp	eax,DM_INIT
	je	calculator_init
	cmp	eax,DM_COMMAND
	je	calculator_command_message
	cmp	eax,DM_CLOSE
	je	calculator_close
	retn
    calculator_init:
	mov	eax,ID_DECSELECT
	mov	edx,1
	jmp	dlg_check
    calculator_command_message:
	cmp	edx,IDCANCEL
	je	calculator_close
	cmp	edx,IDOK
	je	calculator_evaluate
	cmp	edx,ID_EXPRESSION
	jne	calculator_done
	cmp	ecx,EN_CHANGE
	jne	calculator_done
    calculate:
	mov	esi,string_buffer
	mov	eax,ID_EXPRESSION
	mov	edi,esi
	mov	ecx,400h
	call	dlg_get_text
	lea	edi,[esi+eax]
	xor	eax,eax
	stosb
	mov	[compiling],0
	mov	[hash_tree],eax
	mov	[macro_status],al
	mov	[symbols_file],eax
	not	eax
	mov	[source_start],eax
	lea	eax,[esi+1000h]
	mov	[memory_end],eax
	mov	[labels_list],eax
	mov	eax,esp
	and	eax,not 0FFFh
	sub	eax,1000h
	mov	[stack_limit],eax
	push	ebp
	mov	[resume_esp],esp
	mov	[resume_eip],calculator_error
	call	convert_line
	push	edi
	call	convert_expression
	cmp	byte [esi],0
	jne	invalid_expression
	mov	al,')'
	stosb
	pop	esi
	mov	dword [error_line],0
	or	[current_line],-1
	mov	[value_size],0
	call	calculate_expression
	cmp	dword [error_line],0
	je	present_result
	jmp	[error]
    present_result:
	mov	esi,edi
	cmp	byte [esi+13],0
	je	result_in_64bit_composite_range
	test	byte [esi+7],80h
	jnz	result_in_64bit_composite_range
	mov	eax,ID_BINRESULT
	mov	esi,_null
	call	dlg_set_text
	mov	eax,ID_OCTRESULT
	call	dlg_set_text
	mov	eax,ID_HEXRESULT
	call	dlg_set_text
	mov	esi,edi
	jmp	present_decimal
    result_in_64bit_composite_range:
	mov	eax,[esi]
	mov	edx,[esi+4]
	mov	edi,name_buffer+100
	mov	word [edi],'b'
    make_binary:
	mov	bl,'0'
	shrd	eax,edx,1
	adc	bl,0
	dec	edi
	mov	[edi],bl
	shr	edx,1
	jnz	make_binary
	test	eax,eax
	jnz	make_binary
	push	esi
	mov	esi,edi
	mov	eax,ID_BINRESULT
	call	dlg_set_text
	pop	esi
	mov	ecx,[esi]
	mov	edx,[esi+4]
	mov	edi,name_buffer+100
	mov	word [edi],'o'
    make_octal:
	mov	al,cl
	and	al,111b
	add	al,'0'
	dec	edi
	mov	[edi],al
	shrd	ecx,edx,3
	shr	edx,3
	jnz	make_octal
	test	ecx,ecx
	jnz	make_octal
	push	esi
	mov	esi,edi
	mov	eax,ID_OCTRESULT
	call	dlg_set_text
	pop	esi
	mov	ecx,[esi]
	mov	edx,[esi+4]
	mov	edi,name_buffer+100
	mov	word [edi],'h'
    make_hexadecimal:
	mov	al,cl
	and	al,0Fh
	cmp	al,10
	sbb	al,69h
	das
	dec	edi
	mov	[edi],al
	shrd	ecx,edx,4
	shr	edx,4
	jnz	make_hexadecimal
	test	ecx,ecx
	jnz	make_hexadecimal
	cmp	al,'A'
	jb	hexadecimal_ok
	dec	edi
	mov	byte [edi],'0'
    hexadecimal_ok:
	push	esi
	mov	esi,edi
	mov	eax,ID_HEXRESULT
	call	dlg_set_text
	pop	esi
    present_decimal:
	mov	edi,name_buffer+100
	mov	byte [edi],0
	mov	ecx,10
	xor	bl,bl
	cmp	byte [esi+13],0
	je	make_decimal
	mov	bl,'-'
	mov	eax,[esi]
	mov	edx,[esi+4]
	not	eax
	not	edx
	add	eax,1
	adc	edx,0
	mov	[esi],eax
	mov	[esi+4],edx
	or	eax,edx
	jnz	make_decimal
	dec	edi
	mov	byte [edi],'6'
	mov	dword [esi],99999999h
	mov	dword [esi+4],19999999h
    make_decimal:
	mov	eax,[esi+4]
	xor	edx,edx
	div	ecx
	mov	[esi+4],eax
	mov	eax,[esi]
	div	ecx
	mov	[esi],eax
	add	dl,'0'
	dec	edi
	mov	[edi],dl
	or	eax,[esi+4]
	jnz	make_decimal
	test	bl,bl
	jz	decimal_ok
	dec	edi
	mov	[edi],bl
    decimal_ok:
	mov	esi,edi
	mov	eax,ID_DECRESULT
	call	dlg_set_text
	pop	ebp
	retn
    calculator_error:
	pop	ebp
	mov	esi,_null
	mov	eax,ID_BINRESULT
	call	dlg_set_text
	mov	eax,ID_DECRESULT
	call	dlg_set_text
	mov	eax,ID_HEXRESULT
	call	dlg_set_text
	mov	eax,ID_OCTRESULT
	jmp	dlg_set_text
    calculator_evaluate:
	mov	ebx,ID_BINRESULT
	mov	eax,ID_BINSELECT
	call	dlg_is_checked
	test	eax,eax
	jnz	calculator_get_result
	mov	ebx,ID_HEXRESULT
	mov	eax,ID_HEXSELECT
	call	dlg_is_checked
	test	eax,eax
	jnz	calculator_get_result
	mov	ebx,ID_OCTRESULT
	mov	eax,ID_OCTSELECT
	call	dlg_is_checked
	test	eax,eax
	jnz	calculator_get_result
	mov	ebx,ID_DECRESULT
    calculator_get_result:
	mov	eax,ebx
	mov	edi,string_buffer
	mov	ecx,100h
	call	dlg_get_text
	mov	eax,ID_EXPRESSION
	mov	esi,string_buffer
	call	dlg_set_text
	mov	eax,ID_EXPRESSION
	call	dlg_set_focus
	jmp	calculate
    calculator_close:
	xor	eax,eax
	jmp	end_dialog
    calculator_done:
	retn

calculator_command:
	mov	esi,calculator_dialog
	mov	edi,calculator_procedure
	xor	eax,eax
	jmp	dialog_box

; About dialog

about_procedure:
	cmp	eax,DM_COMMAND
	je	about_close
	cmp	eax,DM_CLOSE
	je	about_close
	retn
    about_close:
	xor	eax,eax
	jmp	end_dialog

about_command:
	mov	esi,about_dialog
	mov	edi,about_procedure
	xor	eax,eax
	jmp	dialog_box

; file dialogs

FD_OPEN = 0
FD_SAVE = 1

; open_file_dialog: lets user choose files and opens them

open_file_dialog:
	call	initial_dialog_directory
	mov	[file_dialog_mode],FD_OPEN
	mov	esi,open_dialog
	mov	edi,file_dialog_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,1
	jne	file_dialog_done
	mov	esi,dialog_files
    open_chosen_file:
	cmp	byte [esi],0
	je	file_dialog_done
	push	esi
	mov	ecx,[dialog_readonly]
	call	fm_open
	pop	esi
	cmp	eax,-1
	jne	next_chosen_file
	push	esi
	call	show_loading_error
	pop	esi
    next_chosen_file:
	lodsb
	test	al,al
	jnz	next_chosen_file
	jmp	open_chosen_file
    file_dialog_done:
	retn

; save_file_dialog: [dialog_path] = initial path, returns CF=0 and
; chosen path in [dialog_path]

save_file_dialog:
	cmp	byte [dialog_path],0
	je	save_dialog_without_path
	mov	esi,dialog_path
	mov	edi,dialog_directory
	call	directory_of_path
	mov	esi,dialog_path
	call	file_title
	mov	edi,dialog_initial_name
	call	copy_text
	jmp	save_dialog_ready
    save_dialog_without_path:
	call	initial_dialog_directory
	mov	byte [dialog_initial_name],0
    save_dialog_ready:
	mov	[file_dialog_mode],FD_SAVE
	mov	esi,save_dialog
	mov	edi,file_dialog_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,1
	jne	save_dialog_cancelled
	mov	esi,dialog_files
	mov	edi,dialog_path
	call	copy_text
	clc
	retn
    save_dialog_cancelled:
	stc
	retn

; initial_dialog_directory: directory of current file or current directory

initial_dialog_directory:
	mov	byte [dialog_initial_name],0
	mov	eax,[current_tab]
	cmp	eax,-1
	je	dialog_current_directory
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	test	esi,esi
	jz	dialog_current_directory
	mov	edi,dialog_directory
	jmp	directory_of_path
    dialog_current_directory:
	cmp	byte [dialog_directory],0
	jne	dialog_directory_ready
	mov	edi,dialog_directory
	call	get_current_directory
    dialog_directory_ready:
	retn

file_dialog_procedure:
	cmp	eax,DM_INIT
	je	file_dialog_init
	cmp	eax,DM_COMMAND
	je	file_dialog_command
	cmp	eax,DM_CLOSE
	je	file_dialog_close
	retn
    file_dialog_init:
	mov	eax,ID_FD_FILTER
	mov	esi,_file_filters
	call	dlg_combo_add_strings
	mov	eax,ID_FD_FILTER
	mov	edx,[dialog_filter]
	call	dlg_combo_set_selection
	mov	eax,ID_FD_NAME
	mov	esi,dialog_initial_name
	call	dlg_set_text
	call	refresh_file_list
	mov	eax,ID_FD_NAME
	jmp	dlg_set_focus
    file_dialog_command:
	cmp	edx,IDCANCEL
	je	file_dialog_close
	cmp	edx,IDOK
	je	file_dialog_ok
	cmp	edx,ID_FD_FILTER
	je	file_dialog_filter
	cmp	edx,ID_FD_FILES
	jne	file_dialog_command_done
	cmp	ecx,LBN_DBLCLK
	je	file_dialog_double_click
	cmp	ecx,LBN_SELCHANGE
	jne	file_dialog_command_done
	jmp	file_list_selection
    file_dialog_filter:
	cmp	ecx,CBN_SELCHANGE
	jne	file_dialog_command_done
	mov	eax,ID_FD_FILTER
	call	dlg_combo_get_selection
	mov	[dialog_filter],eax
	jmp	refresh_file_list
    file_dialog_close:
	xor	eax,eax
	jmp	end_dialog
    file_dialog_command_done:
	retn
    file_dialog_double_click:
	mov	eax,ID_FD_FILES
	call	dialog_item
	mov	eax,[ebx+L_SELECTION]
	cmp	eax,0
	jl	file_dialog_command_done
	call	list_item
	cmp	dword [edx+4],0
	je	file_dialog_ok
	call	list_get_string
	jmp	enter_listed_directory
    file_list_selection:
	mov	eax,ID_FD_FILES
	call	dialog_item
	mov	edi,dialog_names
	xor	ecx,ecx
	xor	eax,eax
	mov	[selected_names_count],eax
    collect_selected_names:
	cmp	eax,[ebx+L_COUNT]
	jae	selected_names_collected
	call	list_is_selected
	jnc	next_selected_name
	call	list_item
	cmp	dword [edx+4],0
	jne	next_selected_name
	inc	[selected_names_count]
	push	eax
	call	list_get_string
	mov	byte [edi],'"'
	inc	edi
	call	copy_text
	mov	byte [edi],'"'
	mov	byte [edi+1],' '
	add	edi,2
	pop	eax
    next_selected_name:
	inc	eax
	cmp	edi,dialog_names+PATH_SIZE*4-PATH_SIZE
	jb	collect_selected_names
    selected_names_collected:
	mov	byte [edi],0
	cmp	[selected_names_count],0
	je	file_dialog_command_done
	cmp	[selected_names_count],1
	jne	show_selected_names
	mov	esi,dialog_names+1
	mov	edi,dialog_names
    unquote_selected_name:
	lodsb
	cmp	al,'"'
	je	selected_name_unquoted
	stosb
	jmp	unquote_selected_name
    selected_name_unquoted:
	mov	byte [edi],0
    show_selected_names:
	mov	eax,ID_FD_NAME
	mov	esi,dialog_names
	jmp	dlg_set_text
    file_dialog_ok:
	mov	eax,ID_FD_NAME
	mov	edi,dialog_names
	mov	ecx,PATH_SIZE*4
	call	dlg_get_text
	cmp	byte [dialog_names],0
	jne	file_name_given
	mov	eax,ID_FD_DIRECTORY
	mov	edi,dialog_names
	mov	ecx,PATH_SIZE
	call	dlg_get_text
	mov	esi,dialog_names
	jmp	enter_listed_directory
    file_name_given:
	mov	esi,dialog_names
	mov	edi,dialog_files
	cmp	byte [esi],'"'
	je	multiple_file_names
	call	make_dialog_path
	mov	ebx,dialog_files
	call	get_file_type
	cmp	eax,2
	je	enter_chosen_directory
	cmp	[file_dialog_mode],FD_SAVE
	je	check_saved_file
	test	eax,eax
	jz	chosen_file_missing
	mov	byte [edi+1],0
	jmp	file_dialog_accept
    check_saved_file:
	push	eax
	call	add_default_extension
	pop	eax
	mov	ebx,dialog_files
	call	get_file_type
	test	eax,eax
	jz	file_dialog_accept
	mov	edi,string_buffer
	mov	esi,dialog_files
	call	copy_text
	mov	esi,_overwrite_question
	call	copy_text
	mov	esi,string_buffer
	mov	edi,_save_caption
	mov	eax,MB_ICONQUESTION+MB_YESNO
	call	message_box
	cmp	eax,IDYES
	jne	file_dialog_command_done
	jmp	file_dialog_accept
    multiple_file_names:
	cmp	[file_dialog_mode],FD_SAVE
	je	file_dialog_command_done
    next_quoted_name:
	lodsb
	test	al,al
	jz	file_names_collected
	cmp	al,'"'
	jne	next_quoted_name
	mov	ebx,esi
    find_closing_quote:
	lodsb
	test	al,al
	jz	file_names_collected
	cmp	al,'"'
	jne	find_closing_quote
	mov	byte [esi-1],0
	push	esi
	mov	esi,ebx
	push	edi
	call	make_dialog_path
	pop	ebx
	push	edi
	call	get_file_type
	pop	edi
	pop	esi
	cmp	eax,1
	jne	chosen_file_missing_quoted
	inc	edi
	jmp	next_quoted_name
    chosen_file_missing_quoted:
	push	esi
	mov	esi,ebx
	jmp	report_missing_file
    file_names_collected:
	mov	byte [edi],0
	cmp	edi,dialog_files
	je	file_dialog_command_done
    file_dialog_accept:
	mov	eax,ID_FD_READONLY
	call	dlg_is_checked
	mov	[dialog_readonly],eax
	mov	eax,1
	jmp	end_dialog
    chosen_file_missing:
	push	esi
	mov	esi,dialog_files
    report_missing_file:
	mov	edi,string_buffer
	push	esi
	mov	esi,_file_missing_message
	call	copy_text
	pop	esi
	call	copy_text
	pop	esi
	mov	esi,string_buffer
	mov	edi,_caption
	mov	eax,MB_ICONERROR+MB_OK
	jmp	message_box
    enter_chosen_directory:
	mov	esi,dialog_files
    enter_listed_directory:
	mov	ebx,dialog_directory
	mov	edi,dialog_files
	call	full_path_name
	mov	ebx,dialog_files
	call	get_file_type
	cmp	eax,2
	jne	file_dialog_command_done
	mov	esi,dialog_files
	mov	edi,dialog_directory
	call	copy_text
	mov	eax,ID_FD_NAME
	mov	esi,_empty_string
	call	dlg_set_text
	jmp	refresh_file_list

; make_dialog_path: esi = name, edi = place for full path
; returns edi = end of path (at zero)

make_dialog_path:
	mov	ebx,dialog_directory
	call	full_path_name
	push	esi
	mov	esi,edi
	call	string_length
	add	edi,ecx
	pop	esi
	retn

; add_default_extension: appends .asm to dialog_files if name has no extension

add_default_extension:
	mov	esi,dialog_files
	call	file_title
    fd_find_extension:
	lodsb
	test	al,al
	jz	fd_no_extension
	cmp	al,'.'
	jne	fd_find_extension
	retn
    fd_no_extension:
	lea	edi,[esi-1]
	mov	esi,_default_extension
	jmp	copy_text

; refresh_file_list: lists dialog_directory into the files list

refresh_file_list:
	mov	eax,ID_FD_DIRECTORY
	mov	esi,dialog_directory
	call	dlg_set_text
	mov	eax,ID_FD_FILES
	call	dialog_item
	test	ebx,ebx
	jz	file_list_done
	mov	[file_list],ebx
	call	list_reset
	mov	ecx,10000h
	call	get_memory
	test	eax,eax
	jz	file_list_shown
	mov	[directory_entries],eax
	mov	[directory_entries_count],0
	mov	ecx,40000h
	call	get_memory
	test	eax,eax
	jz	directory_names_failed
	mov	[directory_names],eax
	mov	[directory_names_used],0
	mov	eax,SYS_OPEN
	mov	ebx,dialog_directory
	mov	ecx,O_RDONLY+O_DIRECTORY
	xor	edx,edx
	int	80h
	test	eax,eax
	js	directory_read
	mov	[directory_handle],eax
	cmp	word [dialog_directory],'/'
	je	read_directory_entries
	mov	esi,_parent_directory
	mov	eax,1
	call	add_directory_entry
    read_directory_entries:
	mov	eax,SYS_GETDENTS64
	mov	ebx,[directory_handle]
	mov	ecx,dirent_buffer
	mov	edx,4000h
	int	80h
	test	eax,eax
	jle	directory_entries_read
	mov	esi,dirent_buffer
	lea	edi,[esi+eax]
    process_directory_entry:
	cmp	esi,edi
	jae	read_directory_entries
	push	esi edi
	lea	edx,[esi+19]
	cmp	byte [edx],'.'
	je	skip_directory_entry
	movzx	eax,byte [esi+18]
	cmp	eax,4
	je	directory_entry_is_directory
	cmp	eax,8
	je	directory_entry_is_file
	push	edx
	mov	esi,edx
	mov	ebx,dialog_directory
	mov	edi,entry_path
	call	full_path_name
	mov	ebx,entry_path
	call	get_file_type
	pop	edx
	cmp	eax,2
	je	directory_entry_is_directory
	test	eax,eax
	jz	skip_directory_entry
    directory_entry_is_file:
	mov	esi,edx
	call	filter_file_name
	jc	skip_directory_entry
	xor	eax,eax
	call	add_directory_entry
	jmp	skip_directory_entry
    directory_entry_is_directory:
	mov	esi,edx
	mov	eax,1
	call	add_directory_entry
    skip_directory_entry:
	pop	edi esi
	movzx	eax,word [esi+16]
	add	esi,eax
	jmp	process_directory_entry
    directory_entries_read:
	mov	eax,SYS_CLOSE
	mov	ebx,[directory_handle]
	int	80h
    directory_read:
	call	sort_directory_entries
	mov	ebx,[file_list]
	xor	ecx,ecx
    list_directory_entry:
	cmp	ecx,[directory_entries_count]
	jae	directory_listed
	push	ecx
	mov	edx,[directory_entries]
	mov	esi,[edx+ecx*8]
	mov	edi,entry_path
	call	copy_text
	cmp	dword [edx+ecx*8+4],0
	je	listed_name_ready
	mov	word [edi],'/'
    listed_name_ready:
	push	dword [edx+ecx*8+4]
	mov	esi,entry_path
	call	list_add_string
	pop	ecx
	call	list_item
	mov	[edx+4],ecx
	pop	ecx
	inc	ecx
	jmp	list_directory_entry
    directory_listed:
	mov	ebx,[directory_names]
	call	release_memory
    directory_names_failed:
	mov	ebx,[directory_entries]
	call	release_memory
    file_list_shown:
	mov	ebx,[file_list]
	call	control_redraw
    file_list_done:
	retn

_parent_directory db '..',0
_default_extension db '.asm',0
_save_caption db 'Save as',0

; add_directory_entry: esi = name, eax = 1 for directory

add_directory_entry:
	mov	ecx,[directory_entries_count]
	cmp	ecx,10000h/8
	jae	directory_entry_added
	push	eax
	call	string_length
	inc	ecx
	mov	edx,[directory_names_used]
	add	edx,ecx
	cmp	edx,40000h
	pop	eax
	ja	directory_entry_added
	push	edi
	mov	edi,[directory_names]
	add	edi,[directory_names_used]
	mov	[directory_names_used],edx
	mov	edx,[directory_entries]
	push	ecx
	mov	ecx,[directory_entries_count]
	mov	[edx+ecx*8],edi
	mov	[edx+ecx*8+4],eax
	inc	[directory_entries_count]
	pop	ecx
	rep	movsb
	pop	edi
    directory_entry_added:
	retn

; filter_file_name: esi = name, returns CF=1 if not matching selected filter

filter_file_name:
	cmp	[dialog_filter],0
	jne	file_name_accepted
	push	esi
	mov	edx,esi
    find_name_extension:
	lodsb
	test	al,al
	jz	fd_extension_found
	cmp	al,'.'
	jne	find_name_extension
	mov	edx,esi
	jmp	find_name_extension
    fd_extension_found:
	mov	esi,edx
	mov	edi,_ext_asm
	call	compare_strings_nocase
	je	extension_accepted
	mov	edi,_ext_inc
	call	compare_strings_nocase
	je	extension_accepted
	mov	edi,_ext_ash
	call	compare_strings_nocase
	je	extension_accepted
	pop	esi
	stc
	retn
    extension_accepted:
	pop	esi
    file_name_accepted:
	clc
	retn

_ext_asm db 'asm',0
_ext_inc db 'inc',0
_ext_ash db 'ash',0

; sort_directory_entries: directories first, then names without case

sort_directory_entries:
	mov	ecx,1
    sort_next_entry:
	cmp	ecx,[directory_entries_count]
	jae	directory_sorted
	mov	edx,[directory_entries]
	mov	esi,[edx+ecx*8]
	mov	eax,[edx+ecx*8+4]
	mov	ebx,ecx
    shift_sorted_entries:
	test	ebx,ebx
	jz	insert_sorted_entry
	mov	edi,[edx+(ebx-1)*8]
	cmp	eax,[edx+(ebx-1)*8+4]
	ja	move_sorted_entry
	jb	insert_sorted_entry
	call	compare_names
	jae	insert_sorted_entry
    move_sorted_entry:
	mov	edi,[edx+(ebx-1)*8]
	mov	[edx+ebx*8],edi
	mov	edi,[edx+(ebx-1)*8+4]
	mov	[edx+ebx*8+4],edi
	dec	ebx
	jmp	shift_sorted_entries
    insert_sorted_entry:
	mov	[edx+ebx*8],esi
	mov	[edx+ebx*8+4],eax
	inc	ecx
	jmp	sort_next_entry
    directory_sorted:
	retn

; compare_names: esi, edi = names, compares without case (CF=1 if esi < edi)

compare_names:
	push	eax esi edi
    compare_name_character:
	mov	al,[esi]
	mov	ah,[edi]
	inc	esi
	inc	edi
	cmp	al,'A'
	jb	first_name_case_ok
	cmp	al,'Z'
	ja	first_name_case_ok
	add	al,20h
    first_name_case_ok:
	cmp	ah,'A'
	jb	second_name_case_ok
	cmp	ah,'Z'
	ja	second_name_case_ok
	add	ah,20h
    second_name_case_ok:
	cmp	al,ah
	jne	names_compared
	test	al,al
	jnz	compare_name_character
    names_compared:
	pop	edi esi eax
	retn

; font dialog

; choose_font: esi = current font name, returns CF=0 and chosen_font_name

choose_font:
	mov	edi,chosen_font_name
	call	copy_text
	mov	esi,font_dialog
	mov	edi,font_dialog_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,1
	jne	font_not_chosen
	clc
	retn
    font_not_chosen:
	stc
	retn

font_dialog_procedure:
	cmp	eax,DM_INIT
	je	font_dialog_init
	cmp	eax,DM_COMMAND
	je	font_dialog_command
	cmp	eax,DM_CLOSE
	je	font_dialog_close
	retn
    font_dialog_init:
	mov	[preview_font],0
	mov	eax,ID_FONTS
	call	dialog_item
	test	ebx,ebx
	jz	font_dialog_done
	mov	[font_list],ebx
	mov	ecx,40000h
	call	get_memory
	test	eax,eax
	jz	font_dialog_done
	mov	[font_names],eax
	mov	[font_names_used],0
	mov	edi,_font_patterns
    list_fonts_with_pattern:
	cmp	byte [edi],0
	je	fonts_listed
	push	edi
	mov	esi,edi
	call	x11_list_fonts
	jc	next_font_pattern
	movzx	ecx,word [esi+8]
	add	esi,32
    add_listed_font:
	jecxz	next_font_pattern
	push	ecx
	movzx	ecx,byte [esi]
	inc	esi
	call	add_font_name
	movzx	eax,byte [esi-1]
	add	esi,eax
	pop	ecx
	dec	ecx
	jmp	add_listed_font
    next_font_pattern:
	pop	edi
	mov	esi,edi
	call	string_length
	lea	edi,[edi+ecx+1]
	jmp	list_fonts_with_pattern
    fonts_listed:
	mov	ebx,[font_list]
	xor	ecx,ecx
    find_current_font:
	cmp	ecx,[ebx+L_COUNT]
	jae	font_dialog_done
	mov	eax,ecx
	call	list_item
	mov	esi,[edx+4]
	add	esi,[font_names]
	mov	edi,chosen_font_name
	call	compare_strings_nocase
	je	current_font_found
	inc	ecx
	jmp	find_current_font
    current_font_found:
	mov	eax,ecx
	call	list_set_selection
	jmp	font_selection_changed
    font_dialog_done:
	retn
    font_dialog_command:
	cmp	edx,IDCANCEL
	je	font_dialog_close
	cmp	edx,IDOK
	je	font_dialog_ok
	cmp	edx,ID_FONTS
	jne	font_dialog_done
	cmp	ecx,LBN_DBLCLK
	je	font_dialog_ok
	cmp	ecx,LBN_SELCHANGE
	jne	font_dialog_done
    font_selection_changed:
	mov	ebx,[font_list]
	mov	eax,[ebx+L_SELECTION]
	cmp	eax,0
	jl	font_dialog_done
	call	list_item
	mov	esi,[edx+4]
	add	esi,[font_names]
	mov	edi,chosen_font_name
	call	copy_text
	call	release_preview_font
	mov	esi,chosen_font_name
	call	x11_open_font
	jc	preview_font_ready
	mov	[preview_font],eax
	movsx	eax,word [x11_packet+52]
	mov	[preview_font_ascent],eax
	movzx	eax,byte [x11_packet+50]
	mov	[preview_font_16bit],eax
    preview_font_ready:
	mov	eax,ID_FONT_PREVIEW
	call	dialog_item
	test	ebx,ebx
	jz	font_dialog_done
	jmp	control_redraw
    font_dialog_ok:
	mov	ebx,[font_list]
	cmp	dword [ebx+L_SELECTION],0
	jl	font_dialog_done
	call	release_font_dialog
	mov	eax,1
	jmp	end_dialog
    font_dialog_close:
	call	release_font_dialog
	xor	eax,eax
	jmp	end_dialog

release_font_dialog:
	call	release_preview_font
	mov	ebx,[font_names]
	mov	[font_names],0
	jmp	release_memory

release_preview_font:
	mov	eax,[preview_font]
	test	eax,eax
	jz	preview_font_released
	mov	[preview_font],0
	call	x11_close_font
    preview_font_released:
	retn

; add_font_name: esi = name, ecx = length (adds font to the list unless
; it is a scalable one)

add_font_name:
	push	esi
	mov	edi,font_name_buffer
	cmp	ecx,250
	ja	font_name_rejected
	rep	movsb
	mov	byte [edi],0
	mov	esi,font_name_buffer
	mov	edi,_scalable_marker
	call	find_substring
	jnc	font_name_rejected
	mov	eax,[font_names_used]
	mov	edi,[font_names]
	add	edi,eax
	mov	esi,font_name_buffer
	call	string_length
	lea	edx,[eax+ecx+1]
	cmp	edx,40000h
	ja	font_name_rejected
	mov	[font_names_used],edx
	push	eax
	call	copy_text
	mov	esi,font_name_buffer
	call	describe_font
	mov	ebx,[font_list]
	mov	esi,font_description
	call	list_find_string
	cmp	eax,-1
	jne	font_name_duplicate
	call	sorted_list_position
	call	list_insert_string
	call	list_item
	pop	dword [edx+4]
	pop	esi
	retn
    font_name_duplicate:
	pop	eax
    font_name_rejected:
	pop	esi
	retn

_scalable_marker db '-0-0-',0

; sorted_list_position: ebx = list, esi = string, returns eax = index to keep
; the list sorted

sorted_list_position:
	push	ecx esi edi
	mov	edi,esi
	xor	ecx,ecx
    find_sorted_position:
	cmp	ecx,[ebx+L_COUNT]
	jae	sorted_position_found
	mov	eax,ecx
	call	list_get_string
	xchg	esi,edi
	call	compare_names
	xchg	esi,edi
	jb	sorted_position_found
	inc	ecx
	jmp	find_sorted_position
    sorted_position_found:
	mov	eax,ecx
	pop	edi esi ecx
	retn

; find_substring: esi = string, edi = substring, returns CF=0 if found

find_substring:
	push	eax ecx esi edi
    try_substring_position:
	cmp	byte [esi],0
	je	substring_not_found
	push	esi edi
    compare_substring:
	mov	al,[edi]
	test	al,al
	jz	substring_matches
	cmp	al,[esi]
	jne	substring_mismatch
	inc	esi
	inc	edi
	jmp	compare_substring
    substring_mismatch:
	pop	edi esi
	inc	esi
	jmp	try_substring_position
    substring_matches:
	pop	edi esi
	pop	edi esi ecx eax
	clc
	retn
    substring_not_found:
	pop	edi esi ecx eax
	stc
	retn

; describe_font: esi = XLFD name, makes readable description in font_description
; "family size weight slant (foundry)"

describe_font:
	push	esi
	mov	edi,xlfd_fields
	xor	ecx,ecx
    split_xlfd:
	lodsb
	test	al,al
	jz	xlfd_split
	cmp	al,'-'
	jne	split_xlfd
	cmp	ecx,14
	jae	split_xlfd
	mov	[edi+ecx*4],esi
	inc	ecx
	jmp	split_xlfd
    xlfd_split:
	pop	esi
	cmp	ecx,13
	jb	font_description_plain
	mov	edi,font_description
	mov	esi,[xlfd_fields+1*4]
	call	copy_xlfd_field
	mov	byte [edi],' '
	inc	edi
	mov	esi,[xlfd_fields+6*4]
	call	copy_xlfd_field
	mov	byte [edi],' '
	inc	edi
	mov	esi,[xlfd_fields+2*4]
	call	copy_xlfd_field
	mov	byte [edi],' '
	inc	edi
	mov	esi,[xlfd_fields+3*4]
	call	copy_xlfd_field
	mov	byte [edi],' '
	inc	edi
	mov	esi,[xlfd_fields+4*4]
	call	copy_xlfd_field
	mov	byte [edi],' '
	mov	byte [edi+1],'('
	add	edi,2
	mov	esi,[xlfd_fields+0*4]
	call	copy_xlfd_field
	mov	byte [edi],')'
	mov	byte [edi+1],0
	retn
    font_description_plain:
	mov	edi,font_description
	jmp	copy_text
    copy_xlfd_field:
	lodsb
	test	al,al
	jz	xlfd_field_copied
	cmp	al,'-'
	je	xlfd_field_copied
	stosb
	jmp	copy_xlfd_field
    xlfd_field_copied:
	retn

; preview of font

font_preview_proc:
	cmp	eax,CM_PAINT
	jne	font_preview_done
	mov	eax,[px_window]
	call	fill_control
	mov	eax,FRAME_SUNKEN
	call	frame_control
	mov	eax,[preview_font]
	test	eax,eax
	jz	font_preview_done
	push	ebx
	mov	ecx,eax
	mov	eax,[ui_gc]
	call	x11_set_gc_font
	mov	ecx,[px_text]
	mov	edx,[px_window]
	call	ui_set_colors
	mov	esi,_font_sample
	mov	edi,char2b_buffer
	xor	ecx,ecx
    convert_font_sample:
	call	utf8_decode_character
	test	eax,eax
	jz	font_sample_converted
	cmp	[preview_font_16bit],0
	jne	font_sample_glyph_ok
	cmp	eax,100h
	jb	font_sample_glyph_ok
	mov	eax,'?'
    font_sample_glyph_ok:
	xchg	al,ah
	stosw
	inc	ecx
	jmp	convert_font_sample
    font_sample_converted:
	mov	edi,ecx
	mov	ebx,[esp]
	mov	eax,[ebx+C_WINDOW]
	mov	ecx,4
	mov	edx,[ebx+C_HEIGHT]
	add	edx,[preview_font_ascent]
	shr	edx,1
	mov	ebx,[ui_gc]
	mov	esi,char2b_buffer
	call	x11_image_text16
	mov	eax,[ui_gc]
	mov	ecx,[ui_font]
	call	x11_set_gc_font
	pop	ebx
    font_preview_done:
	retn

; color dialog

; choose_color: eax = initial color, returns CF=0 and eax = chosen color

choose_color:
	mov	[dialog_color],eax
	mov	esi,color_dialog
	mov	edi,color_dialog_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,1
	jne	color_not_chosen
	mov	eax,[dialog_color]
	clc
	retn
    color_not_chosen:
	stc
	retn

color_dialog_procedure:
	cmp	eax,DM_INIT
	je	color_dialog_init
	cmp	eax,DM_COMMAND
	je	color_dialog_command
	cmp	eax,DM_CLOSE
	je	color_dialog_close
	retn
    color_dialog_init:
	mov	[color_updating],1
	call	show_color_components
	mov	[color_updating],0
	retn
    color_dialog_command:
	cmp	edx,IDCANCEL
	je	color_dialog_close
	cmp	edx,IDOK
	je	color_dialog_ok
	cmp	edx,ID_COLOR_GRID
	je	color_grid_clicked
	cmp	ecx,EN_CHANGE
	jne	color_dialog_done
	cmp	[color_updating],0
	jne	color_dialog_done
	mov	eax,ID_COLOR_RED
	call	color_component
	shl	eax,16
	mov	ebx,eax
	mov	eax,ID_COLOR_GREEN
	call	color_component
	shl	eax,8
	or	ebx,eax
	mov	eax,ID_COLOR_BLUE
	call	color_component
	or	eax,ebx
	mov	[dialog_color],eax
	jmp	redraw_color_sample
    color_grid_clicked:
	mov	[color_updating],1
	call	show_color_components
	mov	[color_updating],0
	retn
    color_dialog_ok:
	mov	eax,1
	jmp	end_dialog
    color_dialog_close:
	xor	eax,eax
	jmp	end_dialog
    color_dialog_done:
	retn

color_component:
	call	dlg_get_int
	cmp	eax,0FFh
	jbe	color_component_ok
	mov	eax,0FFh
    color_component_ok:
	retn

show_color_components:
	mov	eax,ID_COLOR_RED
	movzx	edx,byte [dialog_color+2]
	call	dlg_set_int
	mov	eax,ID_COLOR_GREEN
	movzx	edx,byte [dialog_color+1]
	call	dlg_set_int
	mov	eax,ID_COLOR_BLUE
	movzx	edx,byte [dialog_color]
	call	dlg_set_int
    redraw_color_sample:
	mov	eax,ID_COLOR_SAMPLE
	call	dialog_item
	test	ebx,ebx
	jz	color_dialog_done
	jmp	control_redraw

color_sample_proc:
	cmp	eax,CM_PAINT
	jne	color_sample_done
	mov	eax,FRAME_SUNKEN
	call	frame_control
	push	ebx
	mov	eax,[dialog_color]
	call	x11_rgb_to_pixel
	mov	edx,ebx
	mov	ebx,eax
	mov	eax,[edx+C_WINDOW]
	mov	ecx,2
	mov	esi,[edx+C_WIDTH]
	sub	esi,4
	mov	edi,[edx+C_HEIGHT]
	sub	edi,4
	mov	edx,2
	call	ui_fill
	pop	ebx
    color_sample_done:
	retn

COLOR_GRID_COLUMNS = 8
COLOR_GRID_ROWS = 6

color_grid_proc:
	cmp	eax,CM_PAINT
	je	paint_color_grid
	cmp	eax,CM_PRESS
	je	color_grid_press
	retn
    paint_color_grid:
	mov	eax,[px_face]
	call	fill_control
	xor	ecx,ecx
    paint_color_cell:
	cmp	ecx,COLOR_GRID_COLUMNS*COLOR_GRID_ROWS
	jae	color_grid_painted
	push	ecx
	call	color_cell_rectangle
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	push	eax ecx edx esi edi
	mov	ebx,FRAME_THIN_SUNKEN
	call	ui_frame
	pop	edi esi edx ecx eax
	inc	ecx
	inc	edx
	sub	esi,2
	sub	edi,2
	push	eax
	mov	eax,[esp+8]
	mov	eax,[basic_colors+eax*4]
	call	x11_rgb_to_pixel
	mov	ebx,eax
	pop	eax
	call	ui_fill
	pop	ebx
	pop	ecx
	inc	ecx
	jmp	paint_color_cell
    color_grid_painted:
	retn
    color_grid_press:
	xor	ecx,ecx
    find_pressed_color:
	cmp	ecx,COLOR_GRID_COLUMNS*COLOR_GRID_ROWS
	jae	color_grid_painted
	push	ecx
	call	color_cell_rectangle
	mov	eax,[cm_x]
	sub	eax,ecx
	jb	color_cell_missed
	cmp	eax,esi
	jae	color_cell_missed
	mov	eax,[cm_y]
	sub	eax,edx
	jb	color_cell_missed
	cmp	eax,edi
	jae	color_cell_missed
	pop	ecx
	mov	eax,[basic_colors+ecx*4]
	mov	[dialog_color],eax
	mov	edx,BN_CLICKED
	jmp	control_notify
    color_cell_missed:
	pop	ecx
	inc	ecx
	jmp	find_pressed_color

; color_cell_rectangle: ebx = grid, ecx = index, returns ecx, edx, esi, edi

color_cell_rectangle:
	mov	eax,ecx
	xor	edx,edx
	mov	ecx,COLOR_GRID_COLUMNS
	div	ecx
	push	eax
	mov	eax,[ebx+C_WIDTH]
	push	edx
	xor	edx,edx
	div	ecx
	mov	esi,eax
	pop	ecx
	imul	ecx,esi
	mov	eax,[ebx+C_HEIGHT]
	xor	edx,edx
	push	ecx
	mov	ecx,COLOR_GRID_ROWS
	div	ecx
	mov	edi,eax
	pop	ecx
	pop	edx
	imul	edx,edi
	sub	esi,2
	sub	edi,2
	retn
