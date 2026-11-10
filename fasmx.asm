
; fasmx - flat assembler IDE for Linux/X11
; based on the flat assembler IDE for Win32
; Copyright (c) 1999-2022, Tomasz Grysztar.
; All rights reserved.

	format	ELF executable 3
	entry	start

PATH_SIZE = 1000h
TAB_SIZE  = 16
MAX_TABS  = 1000h
TABS_MEMORY = MAX_TABS*TAB_SIZE

TAB_EDITOR = 0
TAB_PATH   = 4
TAB_NAME   = 8

INITIAL_WIDTH  = 800
INITIAL_HEIGHT = 600

STATUS_PART_1 = 72
STATUS_PART_2 = 144

segment readable executable

start:
	mov	ecx,[esp]
	mov	[argument_count],ecx
	lea	eax,[esp+4]
	mov	[arguments],eax
	lea	eax,[esp+4+ecx*4+4]
	mov	[environment],eax
	cld
	mov	[compile_pipe],-1
	call	init_character_tables
	call	init_settings
	call	x11_connect
	jc	startup_error
	call	intern_atoms
	cmp	[instance_flags],0
	je	instance_check_done
	call	pass_to_running_instance
	jnc	instance_passed
    instance_check_done:
	call	x11_load_keyboard_mapping
	call	open_editor_font
	mov	esi,_error_font
	jc	startup_error
	call	create_main_window
	call	ui_init
	call	create_editor_gcs
	mov	[status_mode],_null
	call	create_main_controls
	mov	esi,_error_memory
	jc	startup_error
	call	claim_instance
	mov	ecx,TABS_MEMORY
	call	get_memory
	mov	esi,_error_memory
	test	eax,eax
	jz	startup_error
	mov	[tabs],eax
	mov	[tabs_count],0
	mov	[current_tab],-1
	mov	[assigned_file],-1
	call	main_layout
	mov	esi,_untitled
	call	fm_new
	cmp	eax,-1
	mov	esi,_error_memory
	je	startup_error
	mov	esi,[arguments]
	add	esi,4
	xor	ebx,ebx
	call	process_arguments
	mov	eax,[menubar_window]
	call	x11_map_window
	mov	eax,[main_window]
	call	x11_map_window
	mov	[blink_active],1
	call	reset_blink
	mov	esi,quit_flag
	call	event_loop
	xor	al,al
	jmp	exit_program
    instance_passed:
	xor	al,al
	jmp	exit_program

startup_error:
	mov	ebx,STDERR
	push	esi
	mov	esi,_error_prefix
	call	write_string
	pop	esi
	call	write_string
	mov	esi,_new_line
	call	write_string
	mov	al,1
	jmp	exit_program

; intern_atoms: interns all atoms used by the program

intern_atoms:
	mov	esi,atom_names
	mov	edi,atom_values
    intern_next_atom:
	cmp	byte [esi],0
	je	atoms_interned
	push	edi
	call	x11_intern_atom
	pop	edi
	stosd
    skip_atom_name:
	lodsb
	test	al,al
	jnz	skip_atom_name
	jmp	intern_next_atom
    atoms_interned:
	retn

; open_editor_font: opens font from settings, returns CF=1 on failure

open_editor_font:
	mov	esi,font_name
	cmp	byte [esi],0
	jne	open_configured_font
	mov	esi,_default_font
    open_configured_font:
	call	x11_open_font
	jnc	editor_font_opened
	mov	esi,_default_font
	call	x11_open_font
	jnc	editor_font_opened
	mov	esi,_fallback_font
	call	x11_open_font
	jc	editor_font_failed
    editor_font_opened:
	mov	edi,editor_font_record
	call	make_font_record
	cmp	dword [edi+FONT_WIDTH],0
	jle	editor_font_failed
	cmp	dword [edi+FONT_HEIGHT],0
	jle	editor_font_failed
	mov	eax,[editor_font_record]
	mov	[editor_font_id],eax
	clc
	retn
    editor_font_failed:
	stc
	retn

; make_font_record: eax = font (reply of QueryFont in x11_packet), edi = record

make_font_record:
	mov	[edi+FONT_ID],eax
	movsx	eax,word [x11_packet+28]
	mov	[edi+FONT_WIDTH],eax
	movsx	eax,word [x11_packet+52]
	mov	[edi+FONT_ASCENT],eax
	movsx	edx,word [x11_packet+54]
	add	eax,edx
	mov	[edi+FONT_HEIGHT],eax
	movzx	eax,byte [x11_packet+50]
	mov	[edi+FONT_16BIT],eax
	retn

; create_main_window

create_main_window:
	mov	eax,[x11_white_pixel]
	mov	[window_values],eax
	mov	[window_values+4],X11_KeyPressMask+X11_StructureNotifyMask+X11_FocusChangeMask+X11_PropertyChangeMask
	mov	[window_value_mask],2+800h
	mov	eax,[x11_root]
	mov	ecx,[window_left]
	mov	edx,[window_top]
	mov	esi,[window_right]
	sub	esi,ecx
	mov	edi,[window_bottom]
	sub	edi,edx
	cmp	esi,240
	jge	main_width_ok
	mov	esi,INITIAL_WIDTH
    main_width_ok:
	cmp	edi,160
	jge	main_height_ok
	mov	edi,INITIAL_HEIGHT
    main_height_ok:
	mov	[main_width],esi
	mov	[main_height],edi
	call	x11_create_window
	mov	[main_window],eax
	mov	ecx,WK_MAIN
	xor	edx,edx
	xor	esi,esi
	call	register_window
	mov	eax,[main_window]
	mov	ebx,X11_WM_NAME
	mov	ecx,X11_STRING
	mov	edx,8
	mov	esi,_caption
	mov	edi,_caption_length
	call	x11_change_property
	mov	ebx,[atom_net_wm_name]
	mov	ecx,[atom_utf8_string]
	call	x11_change_property
	mov	ebx,X11_WM_CLASS
	mov	ecx,X11_STRING
	mov	esi,_wm_class
	mov	edi,_wm_class_length
	call	x11_change_property
	mov	ebx,[atom_wm_protocols]
	mov	ecx,X11_ATOM
	mov	edx,32
	mov	esi,atom_wm_delete_window
	mov	edi,1
	call	x11_change_property
	mov	ebx,X11_WM_HINTS
	mov	ecx,X11_WM_HINTS
	mov	esi,wm_hints
	mov	edi,9
	call	x11_change_property
	mov	ebx,[atom_xdnd_aware]
	mov	ecx,X11_ATOM
	mov	esi,xdnd_version
	mov	edi,1
	call	x11_change_property
	mov	edi,size_hints
	xor	ecx,ecx
	mov	dword [edi],16
	cmp	[window_position_known],0
	je	main_position_hint_ok
	or	dword [edi],1
	mov	ecx,[window_left]
	mov	[edi+4],ecx
	mov	ecx,[window_top]
	mov	[edi+8],ecx
    main_position_hint_ok:
	mov	ecx,[main_width]
	mov	[edi+12],ecx
	mov	ecx,[main_height]
	mov	[edi+16],ecx
	mov	dword [edi+20],240
	mov	dword [edi+24],160
	mov	ebx,X11_WM_NORMAL_HINTS
	mov	ecx,X11_WM_SIZE_HINTS
	mov	esi,size_hints
	mov	edi,18
	call	x11_change_property
	cmp	[window_maximized],0
	je	main_window_created
	mov	ebx,[atom_net_wm_state]
	mov	ecx,X11_ATOM
	mov	esi,atom_net_wm_state_maximized_vert
	mov	edi,2
	call	x11_change_property
    main_window_created:
	retn

; create_editor_gcs

create_editor_gcs:
	mov	eax,[x11_black_pixel]
	mov	[gc_values],eax
	mov	[gc_foreground],eax
	mov	eax,[x11_white_pixel]
	mov	[gc_values+4],eax
	mov	[gc_background],eax
	mov	eax,[editor_font_id]
	mov	[gc_values+8],eax
	mov	[editor_gc_font],eax
	mov	dword [gc_values+12],0
	mov	eax,[main_window]
	mov	ecx,4+8+4000h+10000h
	mov	esi,gc_values
	call	x11_create_gc
	mov	[editor_gc],eax
	mov	[gc_values],X11_GXinvert
	mov	dword [gc_values+4],0
	mov	eax,[main_window]
	mov	ecx,1+10000h
	mov	esi,gc_values
	call	x11_create_gc
	mov	[caret_gc],eax
	retn

; create_main_controls: menu bar, tab control and status bar

create_main_controls:
	mov	eax,[ui_font_height]
	add	eax,6
	mov	[menubar_height],eax
	mov	[status_height],eax
	add	eax,2
	mov	[tabs_height],eax
	mov	eax,[px_face]
	mov	[window_values],eax
	mov	[window_values+4],X11_ExposureMask+X11_ButtonPressMask+X11_ButtonReleaseMask+X11_PointerMotionMask
	mov	[window_value_mask],2+800h
	mov	eax,[main_window]
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,[main_width]
	mov	edi,[menubar_height]
	call	x11_create_window
	mov	[menubar_window],eax
	mov	ecx,WK_MENU
	xor	edx,edx
	xor	esi,esi
	call	register_window
	mov	[building_dialog],0
	mov	[new_control_type],CT_CUSTOM
	mov	[new_control_id],0
	mov	[new_control_x],0
	mov	[new_control_y],0
	mov	eax,[main_width]
	mov	[new_control_width],eax
	mov	eax,[tabs_height]
	mov	[new_control_height],eax
	mov	[new_control_style],0
	mov	[new_control_text],0
	mov	eax,[main_window]
	mov	[new_control_parent],eax
	mov	[new_control_notify],0
	mov	[new_control_procedure],tabs_proc
	call	create_control
	test	ebx,ebx
	jz	main_controls_failed
	mov	[tabs_control],ebx
	mov	eax,[status_height]
	mov	[new_control_height],eax
	mov	[new_control_procedure],status_proc
	call	create_control
	mov	[status_control],ebx
	test	ebx,ebx
	jz	main_controls_failed
	clc
	retn
    main_controls_failed:
	stc
	retn

; main_layout: positions components of the main window

main_layout:
	mov	eax,[menubar_window]
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,[main_width]
	mov	edi,[menubar_height]
	call	x11_move_resize_window
	call	menubar_redraw
	mov	ebx,[status_control]
	xor	ecx,ecx
	mov	edx,[main_height]
	sub	edx,[status_height]
	mov	esi,[main_width]
	mov	edi,[status_height]
	call	move_control
	call	control_redraw
	mov	ebx,[tabs_control]
	xor	ecx,ecx
	mov	edx,[main_height]
	sub	edx,[status_height]
	sub	edx,[tabs_height]
	mov	esi,[main_width]
	mov	edi,[tabs_height]
	call	move_control
	call	control_redraw
	mov	eax,[current_tab]
	cmp	eax,-1
	je	main_layout_done
	call	tab_editor
	jmp	move_editor
    main_layout_done:
	retn

; move_editor: eax = editor handle, sets it into the editing area

move_editor:
	mov	[fe_x],0
	mov	ecx,[menubar_height]
	mov	[fe_y],ecx
	mov	ecx,[main_width]
	mov	[fe_width],ecx
	mov	ecx,[main_height]
	sub	ecx,[menubar_height]
	sub	ecx,[tabs_height]
	sub	ecx,[status_height]
	cmp	ecx,1
	jge	editor_area_ok
	mov	ecx,1
    editor_area_ok:
	mov	[fe_height],ecx
	mov	edx,FM_MOVE
	jmp	send_editor

; tab_editor: eax = tab index, returns eax = editor handle

tab_editor:
	shl	eax,4
	add	eax,[tabs]
	mov	eax,[eax+TAB_EDITOR]
	retn

; tab_record: eax = tab index, returns edi = address of tab

tab_record:
	mov	edi,eax
	shl	edi,4
	add	edi,[tabs]
	retn

; current_editor: returns eax = handle of selected editor

current_editor:
	mov	eax,[current_tab]
	cmp	eax,-1
	je	no_current_editor
	jmp	tab_editor
    no_current_editor:
	xor	eax,eax
	retn

; send_current_editor: edx = message, returns eax = result

send_current_editor:
	call	current_editor
	test	eax,eax
	jz	no_current_editor
	jmp	send_editor

; main window events

main_focus_event:
	cmp	byte [esi+1],2
	je	main_focus_done
	xor	eax,eax
	cmp	byte [esi],X11_FocusIn
	jne	main_focus_flag
	inc	eax
    main_focus_flag:
	cmp	eax,[main_focused]
	je	main_focus_done
	mov	[main_focused],eax
	mov	edx,FM_KILLFOCUS
	test	eax,eax
	jz	main_focus_message
	mov	edx,FM_SETFOCUS
	call	reset_blink
    main_focus_message:
	jmp	send_current_editor
    main_focus_done:
	retn

main_configure_event:
	movzx	ecx,word [esi+20]
	movzx	edx,word [esi+22]
	cmp	ecx,[main_width]
	jne	main_resized
	cmp	edx,[main_height]
	je	main_focus_done
    main_resized:
	mov	[main_width],ecx
	mov	[main_height],edx
	jmp	main_layout

main_close_event:
	cmp	[current_dialog],0
	jne	main_focus_done
	mov	[close_flags],MB_DEFBUTTON2
	jmp	close_window

main_blink:
	cmp	[main_focused],0
	je	main_focus_done
	mov	edx,FM_BLINK
	jmp	send_current_editor

; main_key: keyboard input for the main window

main_key:
	call	find_accelerator
	jc	not_accelerator
	jmp	main_command
    not_accelerator:
	cmp	[cm_flags],KS_ALT
	jne	not_menu_mnemonic
	mov	eax,[cm_base_keysym]
	cmp	eax,'A'
	jb	not_menu_mnemonic
	cmp	eax,'z'
	ja	not_menu_mnemonic
	or	eax,20h
	call	menubar_mnemonic
	jc	not_menu_mnemonic
	jmp	open_menu_by_keyboard
    not_menu_mnemonic:
	mov	eax,[cm_keysym]
	cmp	eax,XK_F10
	jne	not_menu_key
	cmp	[cm_flags],0
	jne	not_menu_key
	xor	eax,eax
	jmp	open_menu_by_keyboard
    not_menu_key:
	cmp	eax,XK_Menu
	je	caret_context_menu
	mov	[fe_wparam],eax
	mov	eax,[cm_base_keysym]
	mov	[fe_lparam],eax
	mov	eax,[cm_state]
	mov	[fe_state],eax
	mov	edx,FM_KEYDOWN
	call	send_current_editor
	jmp	reset_blink

; find_accelerator: returns CF=0 and eax = command for the pressed key

find_accelerator:
	mov	esi,accelerators
	mov	dl,[cm_flags]
    check_accelerator:
	mov	eax,[esi]
	test	eax,eax
	jz	no_accelerator
	cmp	dl,[esi+4]
	jne	next_accelerator
	cmp	eax,'a'
	jb	accelerator_by_keysym
	cmp	eax,'z'
	ja	accelerator_by_keysym
	mov	ecx,[cm_base_keysym]
	or	ecx,20h
	cmp	ecx,eax
	je	accelerator_found
	jmp	next_accelerator
    accelerator_by_keysym:
	cmp	eax,[cm_keysym]
	je	accelerator_found
	cmp	eax,[cm_base_keysym]
	je	accelerator_found
    next_accelerator:
	add	esi,12
	jmp	check_accelerator
    accelerator_found:
	mov	eax,[esi+8]
	clc
	retn
    no_accelerator:
	stc
	retn

; context menus

editor_context_menu:
	mov	esi,edit_menu
	or	[menu_bar_index],-1
	mov	ecx,[cm_root_x]
	mov	edx,[cm_root_y]
	mov	[menu_ignore_release],1
	jmp	open_menu

caret_context_menu:
	cmp	[cm_flags],0
	jne	main_focus_done
	call	current_editor
	test	eax,eax
	jz	main_focus_done
	mov	ecx,[eax+caret_x-editor_data]
	mov	edx,[eax+caret_y-editor_data]
	add	edx,[eax+font_height-editor_data]
	mov	eax,[eax+editor_window-editor_data]
	call	x11_translate_coordinates
	mov	esi,edit_menu
	or	[menu_bar_index],-1
	mov	[menu_ignore_release],0
	jmp	open_menu

; editor_notification: eax = notification code, [fedit_handle] = editor

editor_notification:
	push	eax
	call	current_editor
	mov	edx,eax
	pop	eax
	cmp	edx,[fedit_handle]
	jne	notification_ignored
	cmp	eax,FEN_SETFOCUS
	je	update_status_bar
	cmp	eax,FEN_TEXTCHANGE
	je	update_status_bar
	cmp	eax,FEN_POSCHANGE
	je	update_status_bar
	cmp	eax,FEN_OUTOFMEMORY
	je	editor_out_of_memory
    notification_ignored:
	retn
    editor_out_of_memory:
	mov	[pending_memory_error],1
	retn

; update_status_bar: shows position and state of the selected editor

update_status_bar:
	call	current_editor
	test	eax,eax
	jz	notification_ignored
	mov	[status_editor],eax
	mov	[fe_wparam],fepos
	mov	edx,FM_GETPOS
	call	send_editor
	mov	edi,status_position
	mov	eax,[fepos+FEPOS_CARET_LINE]
	call	format_number
	mov	al,','
	stosb
	mov	eax,[fepos+FEPOS_CARET_POSITION]
	call	format_number
	mov	[status_mode],_null
	mov	eax,[status_editor]
	mov	edx,FM_GETMODE
	call	send_editor
	test	eax,FEMODE_READONLY
	jnz	readonly_status
	mov	eax,[status_editor]
	mov	edx,FM_ISUNMODIFIED
	call	send_editor
	test	eax,eax
	jnz	status_text_ready
	mov	[status_mode],_modified_status
	jmp	status_text_ready
    readonly_status:
	mov	[status_mode],_readonly_status
    status_text_ready:
	mov	ebx,[status_control]
	call	control_redraw
	cmp	[pending_memory_error],0
	je	status_bar_updated
	mov	[pending_memory_error],0
	mov	edx,FM_RELEASESEARCH
	call	send_current_editor
	mov	esi,_memory_error
	mov	edi,_caption
	mov	eax,MB_ICONERROR+MB_OK
	call	message_box
    status_bar_updated:
	retn

; status bar

status_proc:
	cmp	eax,CM_PAINT
	jne	status_done
	mov	eax,[px_face]
	call	fill_control
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,FRAME_THIN_SUNKEN
	mov	ecx,0
	mov	edx,2
	mov	esi,STATUS_PART_1
	mov	edi,[status_height]
	sub	edi,4
	call	ui_frame
	mov	ecx,STATUS_PART_1+2
	mov	esi,STATUS_PART_2-STATUS_PART_1-2
	call	ui_frame
	mov	ecx,STATUS_PART_2+2
	mov	esi,[main_width]
	sub	esi,ecx
	call	ui_frame
	pop	ebx
	mov	esi,status_position
	mov	ecx,0
	mov	edx,STATUS_PART_1
	call	status_text
	mov	esi,[status_mode]
	mov	ecx,STATUS_PART_1+2
	mov	edx,STATUS_PART_2-STATUS_PART_1-2
    status_text:
	push	ecx edx
	mov	[ui_text_mnemonics],0
	call	ui_text_width
	pop	edx ecx
	sub	edx,eax
	sar	edx,1
	add	ecx,edx
	mov	edx,[status_height]
	sub	edx,[ui_font_height]
	sar	edx,1
	mov	eax,[px_face]
	mov	[ui_text_background],eax
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[px_text]
	call	ui_draw_text
	pop	ebx
    status_done:
	retn

; tab control

tabs_proc:
	cmp	eax,CM_PAINT
	je	paint_tabs
	cmp	eax,CM_PRESS
	je	tabs_press
	retn
    paint_tabs:
	mov	eax,[px_face]
	call	fill_control
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,[ebx+C_WIDTH]
	mov	edi,edx
	mov	ebx,[px_shadow]
	call	ui_line
	pop	ebx
	call	measure_tabs
	mov	ecx,[tabs_first]
    paint_next_tab:
	cmp	ecx,[tabs_count]
	jae	tabs_painted
	mov	eax,[tab_positions+ecx*4]
	cmp	eax,-1
	je	tabs_painted
	push	ecx
	call	paint_tab
	pop	ecx
	inc	ecx
	jmp	paint_next_tab
    tabs_painted:
	retn

; measure_tabs: computes positions of visible tabs

measure_tabs:
	mov	eax,[tabs_first]
	cmp	eax,[current_tab]
	jle	first_tab_ok
	mov	eax,[current_tab]
	cmp	eax,0
	jge	first_tab_ok
	xor	eax,eax
    first_tab_ok:
	mov	[tabs_first],eax
    layout_tabs:
	mov	ecx,[tabs_first]
	mov	edx,2
    measure_next_tab:
	cmp	ecx,[tabs_count]
	jae	tabs_measured
	cmp	ecx,MAX_VISIBLE_TABS
	jae	tabs_measured
	mov	eax,ecx
	call	tab_record
	mov	esi,[edi+TAB_NAME]
	mov	[ui_text_mnemonics],0
	call	ui_text_width
	add	eax,[ui_font_width]
	add	eax,[ui_font_width]
	cmp	ecx,[assigned_file]
	jne	tab_width_ok
	add	eax,TAB_MARKER_WIDTH
    tab_width_ok:
	mov	[tab_widths+ecx*4],eax
	mov	esi,edx
	add	esi,eax
	cmp	esi,[main_width]
	jbe	tab_fits
	cmp	ecx,[tabs_first]
	je	tab_fits
	cmp	ecx,[current_tab]
	jg	tabs_measured
	inc	[tabs_first]
	jmp	layout_tabs
    tab_fits:
	mov	[tab_positions+ecx*4],edx
	mov	edx,esi
	inc	ecx
	jmp	measure_next_tab
    tabs_measured:
	cmp	ecx,MAX_VISIBLE_TABS
	jae	tabs_measure_done
	mov	[tab_positions+ecx*4],-1
    tabs_measure_done:
	retn

MAX_VISIBLE_TABS = 400h
TAB_MARKER_WIDTH = 12

; paint_tab: ebx = tab control, ecx = tab index

paint_tab:
	mov	[painted_tab],ecx
	mov	eax,[tab_positions+ecx*4]
	mov	[tab_x],eax
	mov	eax,[tab_widths+ecx*4]
	mov	[tab_width],eax
	mov	eax,2
	cmp	ecx,[current_tab]
	jne	tab_top_ok
	xor	eax,eax
    tab_top_ok:
	mov	[tab_top],eax
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[px_face]
	mov	ecx,[tab_x]
	mov	edx,[tab_top]
	mov	esi,[tab_width]
	mov	edi,[tabs_height]
	sub	edi,edx
	sub	edi,2
	call	ui_fill
	mov	ebx,[px_light]
	mov	esi,ecx
	add	edi,edx
	mov	[tab_bottom],edi
	call	ui_line
	mov	ecx,[tab_x]
	add	ecx,[tab_width]
	dec	ecx
	mov	esi,ecx
	mov	ebx,[px_dark]
	call	ui_line
	mov	ecx,[tab_x]
	mov	edx,[tab_bottom]
	mov	edi,edx
	mov	esi,[tab_x]
	add	esi,[tab_width]
	dec	esi
	call	ui_line
	pop	ebx
	mov	ecx,[tab_x]
	add	ecx,[ui_font_width]
	mov	eax,[painted_tab]
	cmp	eax,[assigned_file]
	jne	tab_marker_done
	push	ebx ecx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[px_red]
	call	ui_set_fill_color
	mov	edx,[tabs_height]
	sub	edx,8
	sar	edx,1
	add	edx,[tab_top]
	mov	esi,8
	mov	edi,8
	call	x11_fill_arc
	pop	ecx ebx
	add	ecx,TAB_MARKER_WIDTH
    tab_marker_done:
	mov	eax,[painted_tab]
	call	tab_record
	mov	esi,[edi+TAB_NAME]
	mov	edx,[tabs_height]
	sub	edx,[ui_font_height]
	sar	edx,1
	add	edx,[tab_top]
	mov	eax,[px_face]
	mov	[ui_text_background],eax
	mov	[ui_text_mnemonics],0
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[px_text]
	call	ui_draw_text
	pop	ebx
	retn

; tabs_press: ebx = tab control

tabs_press:
	call	measure_tabs
	mov	ecx,[tabs_first]
	mov	eax,[cm_x]
    find_pressed_tab:
	cmp	ecx,[tabs_count]
	jae	tab_press_done
	mov	edx,[tab_positions+ecx*4]
	cmp	edx,-1
	je	tab_press_done
	cmp	eax,edx
	jl	tab_press_done
	add	edx,[tab_widths+ecx*4]
	cmp	eax,edx
	jl	tab_pressed
	inc	ecx
	jmp	find_pressed_tab
    tab_pressed:
	mov	eax,[cm_button]
	cmp	eax,1
	je	tab_select_press
	cmp	eax,3
	jne	tab_press_done
	mov	eax,ecx
	call	fm_select
	call	tab_menu_states
	mov	esi,tab_menu
	or	[menu_bar_index],-1
	mov	ecx,[cm_root_x]
	mov	edx,[cm_root_y]
	mov	[menu_ignore_release],1
	jmp	open_menu
    tab_select_press:
	cmp	ecx,[current_tab]
	je	tab_press_done
	mov	eax,ecx
	jmp	fm_select
    tab_press_done:
	retn

; tab_menu_states: updates states of tab menu items

tab_menu_states:
	mov	eax,[current_tab]
	xor	edx,edx
	cmp	eax,[assigned_file]
	jne	tab_assign_state
	mov	edx,MIS_CHECKED
    tab_assign_state:
	mov	eax,IDM_ASSIGN
	mov	ecx,MIS_CHECKED
	call	set_menu_state
	mov	edx,FM_GETMODE
	call	send_current_editor
	xor	edx,edx
	test	eax,FEMODE_READONLY
	jz	tab_readonly_state
	mov	edx,MIS_CHECKED
    tab_readonly_state:
	push	edx
	mov	edx,FM_ISUNMODIFIED
	call	send_current_editor
	pop	edx
	test	eax,eax
	jz	tab_readonly_grayed
	mov	eax,[current_tab]
	call	tab_record
	cmp	dword [edi+TAB_PATH],0
	jne	tab_readonly_enabled
    tab_readonly_grayed:
	or	edx,MIS_GRAYED
    tab_readonly_enabled:
	mov	eax,IDM_READONLY
	mov	ecx,MIS_CHECKED+MIS_GRAYED
	jmp	set_menu_state

; main_init_menu: updates states of menu items (port of WM_INITMENU)

main_init_menu:
	call	current_editor
	test	eax,eax
	jz	main_menu_initialized
	mov	[menu_editor],eax
	mov	edx,FM_CANUNDO
	call	menu_editor_query
	mov	ebx,eax
	mov	eax,IDM_UNDO
	call	gray_if_zero_ebx
	mov	edx,FM_CANREDO
	call	menu_editor_query
	mov	ebx,eax
	mov	eax,IDM_REDO
	call	gray_if_zero_ebx
	mov	[fe_wparam],fepos
	mov	edx,FM_GETPOS
	call	menu_editor_query
	mov	eax,[fepos+FEPOS_SELECTION_LINE]
	cmp	eax,[fepos+FEPOS_CARET_LINE]
	jne	menu_selection_exists
	mov	eax,[fepos+FEPOS_SELECTION_POSITION]
	cmp	eax,[fepos+FEPOS_CARET_POSITION]
	jne	menu_selection_exists
	xor	eax,eax
	jmp	menu_selection_known
    menu_selection_exists:
	or	eax,-1
    menu_selection_known:
	mov	[menu_selection_flag],eax
	mov	eax,IDM_CUT
	mov	ebx,[menu_selection_flag]
	call	gray_if_zero_ebx
	mov	eax,IDM_COPY
	call	gray_if_zero_ebx
	mov	eax,IDM_DELETE
	call	gray_if_zero_ebx
	call	clipboard_available
	mov	ebx,eax
	mov	eax,IDM_PASTE
	call	gray_if_zero_ebx
	mov	edx,FM_GETMODE
	call	menu_editor_query
	mov	[menu_editor_mode],eax
	xor	edx,edx
	test	eax,FEMODE_VERTICALSEL
	jz	menu_vertical_state
	mov	edx,MIS_CHECKED
    menu_vertical_state:
	mov	eax,IDM_VERTICAL
	mov	ecx,MIS_CHECKED
	call	set_menu_state
	test	[menu_editor_mode],FEMODE_READONLY
	jz	menu_not_readonly
	mov	edx,MIS_GRAYED
	mov	ecx,MIS_GRAYED
	mov	eax,IDM_CUT
	call	set_menu_state
	mov	eax,IDM_PASTE
	call	set_menu_state
	mov	eax,IDM_DELETE
	call	set_menu_state
	mov	eax,IDM_REPLACE
	call	set_menu_state
	jmp	menu_readonly_done
    menu_not_readonly:
	mov	eax,IDM_REPLACE
	mov	ecx,MIS_GRAYED
	xor	edx,edx
	call	set_menu_state
    menu_readonly_done:
	mov	edx,FM_CANFINDNEXT
	call	menu_editor_query
	mov	ebx,eax
	mov	eax,IDM_FINDNEXT
	call	gray_if_zero_ebx
	mov	eax,IDM_SECURESEL
	mov	ebx,FES_SECURESEL
	call	check_style_item
	mov	eax,IDM_AUTOBRACKETS
	mov	ebx,FES_AUTOBRACKETS
	call	check_style_item
	mov	eax,IDM_AUTOINDENT
	mov	ebx,FES_AUTOINDENT
	call	check_style_item
	mov	eax,IDM_SMARTTABS
	mov	ebx,FES_SMARTTABS
	call	check_style_item
	mov	eax,IDM_OPTIMALFILL
	mov	ebx,FES_OPTIMALFILL
	call	check_style_item
	mov	eax,IDM_REVIVEDEADKEYS
	mov	ebx,FES_REVIVEDEADKEYS
	call	check_style_item
	mov	eax,IDM_TIMESCROLL
	mov	ebx,FES_TIMESCROLL
	call	check_style_item
	xor	edx,edx
	cmp	[instance_flags],0
	je	menu_instance_state
	mov	edx,MIS_CHECKED
    menu_instance_state:
	mov	eax,IDM_ONEINSTANCEONLY
	mov	ecx,MIS_CHECKED
	call	set_menu_state
	call	tab_menu_states
    main_menu_initialized:
	retn
    menu_editor_query:
	mov	eax,[menu_editor]
	jmp	send_editor
    gray_if_zero_ebx:
	xor	edx,edx
	test	ebx,ebx
	jnz	gray_state_ok
	mov	edx,MIS_GRAYED
    gray_state_ok:
	mov	ecx,MIS_GRAYED
	jmp	set_menu_state
    check_style_item:
	xor	edx,edx
	test	[fedit_style],ebx
	jz	style_item_state
	mov	edx,MIS_CHECKED
    style_item_state:
	mov	ecx,MIS_CHECKED
	jmp	set_menu_state

; main_command: eax = command identifier (port of menu commands of FASMW)

main_command:
	mov	[command_id],eax
	cmp	eax,IDM_NEW
	je	new_file
	cmp	eax,IDM_OPEN
	je	open_file
	cmp	eax,IDM_SAVE
	je	save_file_command
	cmp	eax,IDM_SAVEAS
	je	save_file_as
	cmp	eax,IDM_NEXT
	je	next_file
	cmp	eax,IDM_PREVIOUS
	je	previous_file
	cmp	eax,IDM_OPENFOLDER
	je	open_folder
	cmp	eax,IDM_CLOSE
	je	close_file
	cmp	eax,IDM_EXIT
	je	exit_command
	cmp	eax,IDM_UNDO
	je	undo_command
	cmp	eax,IDM_REDO
	je	redo_command
	cmp	eax,IDM_DISCARD_UNDO
	je	discard_undo
	cmp	eax,IDM_CUT
	je	cut_command
	cmp	eax,IDM_COPY
	je	copy_command
	cmp	eax,IDM_PASTE
	je	paste_command
	cmp	eax,IDM_DELETE
	je	delete_command
	cmp	eax,IDM_SELECTALL
	je	select_all
	cmp	eax,IDM_VERTICAL
	je	vertical_command
	cmp	eax,IDM_READONLY
	je	read_only_command
	cmp	eax,IDM_POSITION
	je	position_command
	cmp	eax,IDM_FIND
	je	find_command
	cmp	eax,IDM_FINDNEXT
	je	findnext_command
	cmp	eax,IDM_REPLACE
	je	replace_command
	cmp	eax,IDM_RUN
	je	run_command
	cmp	eax,IDM_COMPILE
	je	compile_command
	cmp	eax,IDM_SYMBOLS
	je	build_symbols
	cmp	eax,IDM_ASSIGN
	je	assign_command
	cmp	eax,IDM_APPEARANCE
	je	appearance_command
	cmp	eax,IDM_COMPILERSETUP
	je	compiler_setup_command
	cmp	eax,IDM_SECURESEL
	je	option_securesel
	cmp	eax,IDM_AUTOBRACKETS
	je	option_autobrackets
	cmp	eax,IDM_AUTOINDENT
	je	option_autoindent
	cmp	eax,IDM_SMARTTABS
	je	option_smarttabs
	cmp	eax,IDM_OPTIMALFILL
	je	option_optimalfill
	cmp	eax,IDM_REVIVEDEADKEYS
	je	option_revivedeadkeys
	cmp	eax,IDM_TIMESCROLL
	je	option_timescroll
	cmp	eax,IDM_ONEINSTANCEONLY
	je	option_oneinstanceonly
	cmp	eax,IDM_CALCULATOR
	je	calculator_command
	cmp	eax,IDM_ABOUT
	je	about_command
	sub	eax,IDM_SELECTFILE
	jc	command_done
	cmp	eax,9
	ja	command_done
	dec	eax
	js	command_done
	cmp	eax,[tabs_count]
	jae	command_done
	jmp	fm_select
    command_done:
	retn
    new_file:
	mov	esi,_untitled
	jmp	fm_new
    open_file:
	call	open_file_dialog
	retn
    save_file_command:
	mov	eax,[current_tab]
	xor	edx,edx
	call	fm_save
	test	eax,eax
	jnz	save_file_as
	retn
    save_file_as:
	mov	eax,[current_tab]
	jmp	save_tab_as
    next_file:
	mov	eax,[current_tab]
	inc	eax
	cmp	eax,[tabs_count]
	jb	select_file
	xor	eax,eax
    select_file:
	jmp	fm_select
    previous_file:
	mov	eax,[current_tab]
	sub	eax,1
	jnc	select_file
	mov	eax,[tabs_count]
	dec	eax
	jmp	select_file
    open_folder:
	mov	eax,[current_tab]
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	test	esi,esi
	jz	command_done
	mov	edi,path_buffer
	call	directory_of_path
	mov	esi,path_buffer
	jmp	open_with_viewer
    close_file:
	mov	[close_flags],0
	jmp	close_tab
    exit_command:
	mov	[close_flags],0
	jmp	close_window
    undo_command:
	mov	edx,FM_UNDO
	jmp	send_current_editor
    redo_command:
	mov	edx,FM_REDO
	jmp	send_current_editor
    cut_command:
	mov	edx,FM_CUT
	jmp	send_current_editor
    copy_command:
	mov	edx,FM_COPY
	jmp	send_current_editor
    paste_command:
	mov	edx,FM_PASTE
	jmp	send_current_editor
    delete_command:
	mov	edx,FM_CLEAR
	jmp	send_current_editor
    discard_undo:
	mov	edx,FM_EMPTYUNDOBUFFER
	jmp	send_current_editor
    select_all:
	mov	[fepos+FEPOS_SELECTION_LINE],1
	mov	[fepos+FEPOS_SELECTION_POSITION],1
	mov	[fepos+FEPOS_CARET_LINE],-1
	mov	[fepos+FEPOS_CARET_POSITION],1
	mov	[fe_wparam],fepos
	mov	edx,FM_SETPOS
	call	send_current_editor
	mov	[fe_wparam],fepos
	mov	edx,FM_GETPOS
	call	send_current_editor
	mov	eax,[fepos+FEPOS_CARET_LINE]
	mov	[fe_wparam],eax
	mov	edx,FM_GETLINELENGTH
	call	send_current_editor
	inc	eax
	mov	[fepos+FEPOS_CARET_POSITION],eax
	mov	[fe_wparam],fepos
	mov	edx,FM_SETPOS
	jmp	send_current_editor
    vertical_command:
	mov	edx,FM_GETMODE
	call	send_current_editor
	xor	eax,FEMODE_VERTICALSEL
	mov	[fe_wparam],eax
	mov	edx,FM_SETMODE
	jmp	send_current_editor
    read_only_command:
	mov	eax,[current_tab]
	call	tab_record
	cmp	dword [edi+TAB_PATH],0
	je	command_done
	mov	edx,FM_ISUNMODIFIED
	call	send_current_editor
	test	eax,eax
	jz	command_done
	mov	edx,FM_GETMODE
	call	send_current_editor
	xor	eax,FEMODE_READONLY
	mov	[fe_wparam],eax
	mov	edx,FM_SETMODE
	call	send_current_editor
	jmp	update_status_bar
    assign_command:
	mov	eax,[current_tab]
	cmp	eax,[assigned_file]
	jne	do_assign
	or	eax,-1
    do_assign:
	jmp	fm_assign
    option_securesel:
	mov	eax,FES_SECURESEL
	jmp	toggle_style
    option_autobrackets:
	mov	eax,FES_AUTOBRACKETS
	jmp	toggle_style
    option_autoindent:
	mov	eax,FES_AUTOINDENT
	jmp	toggle_style
    option_smarttabs:
	mov	eax,FES_SMARTTABS
	jmp	toggle_style
    option_optimalfill:
	mov	eax,FES_OPTIMALFILL
	jmp	toggle_style
    option_revivedeadkeys:
	mov	eax,FES_REVIVEDEADKEYS
	jmp	toggle_style
    option_timescroll:
	mov	eax,FES_TIMESCROLL
    toggle_style:
	xor	[fedit_style],eax
	mov	eax,[current_tab]
	jmp	fm_select
    option_oneinstanceonly:
	xor	[instance_flags],1
	call	save_instance_setting
	retn

; fm_new: esi = name of tab, returns eax = index of new tab (-1 on failure)

fm_new:
	cmp	[tabs_count],MAX_TABS
	jae	fm_new_failed
	call	ui_store_text_global
	test	eax,eax
	jz	fm_new_failed
	mov	[new_tab_name],eax
	mov	[building_dialog],0
	mov	[fedit_create_flags],FES_SCROLLBARS+FES_MAIN
	mov	eax,[main_window]
	xor	ecx,ecx
	mov	edx,[menubar_height]
	mov	esi,[main_width]
	mov	edi,[main_height]
	sub	edi,edx
	call	fedit_create
	test	eax,eax
	jnz	fm_new_editor_ready
	mov	ebx,[new_tab_name]
	call	release_memory
	jmp	fm_new_failed
    fm_new_editor_ready:
	mov	edx,eax
	mov	eax,[tabs_count]
	inc	[tabs_count]
	call	tab_record
	mov	[edi+TAB_EDITOR],edx
	mov	dword [edi+TAB_PATH],0
	mov	ecx,[new_tab_name]
	mov	[edi+TAB_NAME],ecx
	push	eax
	call	fm_select
	pop	eax
	retn
    fm_new_failed:
	or	eax,-1
	retn

; ui_store_text_global: esi = text, returns eax = copy (separate memory)

ui_store_text_global:
	push	ebx
	mov	eax,[building_dialog]
	push	eax
	mov	[building_dialog],0
	call	ui_store_text
	pop	ebx
	mov	[building_dialog],ebx
	pop	ebx
	retn

; fm_select: eax = tab index (port of FM_SELECT)

fm_select:
	cmp	eax,[tabs_count]
	jae	tab_selected
	mov	[selected_tab],eax
	mov	eax,[current_tab]
	cmp	eax,-1
	je	previous_tab_hidden
	cmp	eax,[selected_tab]
	je	previous_tab_hidden
	call	tab_editor
	push	eax
	mov	edx,FM_KILLFOCUS
	call	send_editor
	pop	eax
	mov	[fe_wparam],0
	mov	edx,FM_SHOW
	call	send_editor
    previous_tab_hidden:
	mov	eax,[selected_tab]
	mov	[current_tab],eax
	call	tab_editor
	mov	ebx,eax
	mov	eax,[fedit_style]
	mov	[fe_wparam],eax
	mov	eax,ebx
	mov	edx,FM_SETSTYLE
	call	send_editor
	mov	[fe_wparam],editor_font_record
	mov	eax,ebx
	mov	edx,FM_SETFONT
	call	send_editor
	mov	eax,[editor_colors]
	mov	[fe_wparam],eax
	mov	eax,[editor_colors+4]
	mov	[fe_lparam],eax
	mov	eax,ebx
	mov	edx,FM_SETTEXTCOLOR
	call	send_editor
	mov	eax,[editor_colors+8]
	mov	[fe_wparam],eax
	mov	eax,[editor_colors+12]
	mov	[fe_lparam],eax
	mov	eax,ebx
	mov	edx,FM_SETSELCOLOR
	call	send_editor
	mov	[fe_wparam],asm_syntax_colors
	mov	[fe_lparam],fasm_syntax
	mov	eax,ebx
	mov	edx,FM_SETSYNTAXHIGHLIGHT
	call	send_editor
	mov	eax,ebx
	call	move_editor
	mov	[fe_wparam],1
	mov	eax,ebx
	mov	edx,FM_SHOW
	call	send_editor
	cmp	[main_focused],0
	je	selected_editor_focus_ok
	mov	eax,ebx
	mov	edx,FM_SETFOCUS
	call	send_editor
    selected_editor_focus_ok:
	mov	ebx,[tabs_control]
	call	control_redraw
	call	update_status_bar
    tab_selected:
	retn

; fm_assign: eax = index of tab (-1 to remove assignment)

fm_assign:
	cmp	eax,[tabs_count]
	jl	assign_ok
	or	eax,-1
    assign_ok:
	mov	[assigned_file],eax
	mov	ebx,[tabs_control]
	jmp	control_redraw

; fm_open: esi = full path, ecx = 1 to open as read-only
; returns eax = index of tab (-1 on failure)

fm_open:
	mov	[open_readonly],ecx
	mov	[open_path],esi
	xor	ebx,ebx
    check_if_already_loaded:
	cmp	ebx,[tabs_count]
	jae	load_new_file
	mov	eax,ebx
	call	tab_record
	mov	edi,[edi+TAB_PATH]
	test	edi,edi
	jz	check_next_loaded
	mov	esi,[open_path]
	call	compare_strings
	je	show_already_loaded
    check_next_loaded:
	inc	ebx
	jmp	check_if_already_loaded
    show_already_loaded:
	mov	eax,ebx
	call	fm_select
	jmp	update_fedit_mode
    load_new_file:
	mov	ebx,[open_path]
	call	load_file
	jc	open_failed
	mov	[open_file_memory],ebx
	mov	[open_file_size],ecx
	lea	ecx,[ecx*4+4]
	call	get_memory
	test	eax,eax
	jz	open_out_of_memory
	mov	[open_cells_memory],eax
	mov	esi,[open_file_memory]
	mov	ecx,[open_file_size]
	mov	edi,eax
	call	utf8_to_cells
	mov	ebx,[open_file_memory]
	call	release_memory
	mov	ecx,PATH_SIZE
	call	get_memory
	test	eax,eax
	jz	open_cells_failed
	mov	[open_path_copy],eax
	mov	edi,eax
	mov	esi,[open_path]
	call	copy_text
	mov	esi,[open_path]
	call	file_title
	call	ui_store_text_global
	test	eax,eax
	jz	open_title_failed
	mov	[open_title],eax
	cmp	[tabs_count],1
	jne	new_fedit
	xor	eax,eax
	call	tab_record
	cmp	dword [edi+TAB_PATH],0
	jne	new_fedit
	xor	eax,eax
	call	tab_editor
	mov	edx,FM_ISUNMODIFIED
	call	send_editor
	test	eax,eax
	jz	new_fedit
	xor	eax,eax
	call	tab_record
	jmp	set_tab_path
    new_fedit:
	mov	esi,[open_path]
	call	file_title
	call	fm_new
	cmp	eax,-1
	je	open_new_tab_failed
	call	tab_record
    set_tab_path:
	mov	eax,[open_path_copy]
	mov	[edi+TAB_PATH],eax
	mov	ebx,[edi+TAB_NAME]
	call	release_memory
	mov	eax,[open_title]
	mov	[edi+TAB_NAME],eax
	mov	eax,edi
	sub	eax,[tabs]
	shr	eax,4
	push	eax
	call	fm_select
	mov	eax,[open_cells_memory]
	mov	[fe_lparam],eax
	mov	edx,FM_SETTEXT
	call	send_current_editor
	mov	edx,FM_MARKUNMODIFIED
	call	send_current_editor
	mov	ebx,[open_cells_memory]
	call	release_memory
	pop	eax
	push	eax
	call	update_fedit_mode_of_current
	pop	eax
	retn
    update_fedit_mode:
	call	update_fedit_mode_of_current
	mov	eax,[current_tab]
	retn
    open_new_tab_failed:
	mov	ebx,[open_title]
	call	release_memory
    open_title_failed:
	mov	ebx,[open_path_copy]
	call	release_memory
    open_cells_failed:
	mov	ebx,[open_cells_memory]
	call	release_memory
	jmp	open_failed
    open_out_of_memory:
	mov	ebx,[open_file_memory]
	call	release_memory
    open_failed:
	or	eax,-1
	retn

update_fedit_mode_of_current:
	mov	edx,FM_GETMODE
	call	send_current_editor
	and	eax,not FEMODE_READONLY
	cmp	[open_readonly],0
	je	set_fedit_mode
	or	eax,FEMODE_READONLY
    set_fedit_mode:
	mov	[fe_wparam],eax
	mov	edx,FM_SETMODE
	call	send_current_editor
	jmp	update_status_bar

; fm_save: eax = tab index, edx = nonzero to save only when modified
; returns eax = 0 if saved (or no need), -1 if failed or file has no name

fm_save:
	mov	[save_only_modified],edx
	cmp	eax,[tabs_count]
	jae	save_failed
	mov	[save_tab],eax
	call	tab_record
	cmp	dword [edi+TAB_PATH],0
	je	save_failed
	mov	eax,[edi+TAB_EDITOR]
	mov	[save_editor],eax
	mov	edx,FM_GETMODE
	call	send_editor
	test	eax,FEMODE_READONLY
	jnz	save_ok
	mov	eax,[save_editor]
	mov	edx,FM_ISUNMODIFIED
	call	send_editor
	and	eax,[save_only_modified]
	jnz	save_ok
	mov	eax,[save_editor]
	mov	edx,FM_GETTEXTLENGTH
	call	send_editor
	lea	ecx,[eax+1]
	call	get_memory
	test	eax,eax
	jz	save_failed
	mov	[save_memory],eax
	mov	[fe_lparam],eax
	mov	eax,[save_editor]
	mov	edx,FM_GETTEXT
	call	send_editor
	mov	ecx,eax
	mov	eax,[save_tab]
	call	tab_record
	mov	ebx,[edi+TAB_PATH]
	mov	esi,[save_memory]
	call	save_file
	pushfd
	mov	ebx,[save_memory]
	call	release_memory
	popfd
	jc	save_failed
	mov	eax,[save_editor]
	mov	edx,FM_MARKUNMODIFIED
	call	send_editor
	mov	eax,[save_tab]
	cmp	eax,[current_tab]
	jne	save_ok
	call	update_status_bar
    save_ok:
	xor	eax,eax
	retn
    save_failed:
	or	eax,-1
	retn

; save_tab_as: eax = tab index, returns eax = 0 if saved

save_tab_as:
	mov	[save_as_tab],eax
	mov	[save_as_title],0
	mov	eax,[current_tab]
	mov	[save_as_current],eax
	mov	eax,[save_as_tab]
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	mov	edi,dialog_path
	mov	byte [edi],0
	test	esi,esi
	jz	save_as_path_ready
	call	copy_text
    save_as_path_ready:
	call	save_file_dialog
	jc	save_as_cancelled
	; Keep the destination separate until the write succeeds.
	mov	ecx,PATH_SIZE
	call	get_memory
	test	eax,eax
	jz	save_as_cancelled
	mov	[save_as_destination],eax
	mov	edi,eax
	mov	esi,dialog_path
	call	copy_text
	mov	esi,[save_as_destination]
	call	file_title
	call	ui_store_text_global
	test	eax,eax
	jz	save_as_release_destination
	mov	[save_as_title],eax
	mov	[save_as_duplicate],-1
	xor	ecx,ecx
    check_if_overwritten:
	cmp	ecx,[tabs_count]
	jae	save_as_write
	cmp	ecx,[save_as_tab]
	je	not_overwritten
	mov	eax,ecx
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	test	esi,esi
	jz	not_overwritten
	mov	edi,[save_as_destination]
	call	compare_strings
	je	save_as_check_modified
    not_overwritten:
	inc	ecx
	jmp	check_if_overwritten
    save_as_check_modified:
	mov	[save_as_duplicate],ecx
	mov	eax,ecx
	call	tab_editor
	mov	edx,FM_ISUNMODIFIED
	call	send_editor
	test	eax,eax
	jnz	save_as_write
	mov	eax,[save_as_duplicate]
	call	fm_select
	mov	esi,_save_destination_question
	mov	edi,_save_caption
	mov	eax,MB_ICONQUESTION+MB_YESNOCANCEL
	call	message_box
	cmp	eax,IDNO
	je	save_as_write
	cmp	eax,IDYES
	jne	save_as_release_destination
	call	save_destination_copy
	test	eax,eax
	jnz	save_as_release_destination
    save_as_write:
	mov	eax,[save_as_current]
	call	fm_select
	; fm_save skips read-only editors; do not mistake that for a write.
	mov	eax,[save_as_tab]
	call	tab_editor
	mov	edx,FM_GETMODE
	call	send_editor
	test	eax,FEMODE_READONLY
	jnz	save_as_destination_failed
	mov	eax,[save_as_tab]
	call	tab_record
	mov	eax,[edi+TAB_PATH]
	mov	[save_as_old_path],eax
	mov	eax,[save_as_destination]
	mov	[edi+TAB_PATH],eax
	mov	eax,[save_as_tab]
	xor	edx,edx
	call	fm_save
	test	eax,eax
	jnz	save_as_write_failed
	mov	ebx,[save_as_old_path]
	call	release_memory
	mov	eax,[save_as_tab]
	call	tab_record
	mov	ebx,[edi+TAB_NAME]
	call	release_memory
	mov	eax,[save_as_title]
	mov	[edi+TAB_NAME],eax
	mov	eax,[save_as_duplicate]
	cmp	eax,-1
	je	save_as_done
	call	delete_tab
    save_as_done:
	mov	ebx,[tabs_control]
	call	control_redraw
	call	update_status_bar
	xor	eax,eax
	retn
    save_as_write_failed:
	mov	eax,[save_as_tab]
	call	tab_record
	mov	eax,[save_as_old_path]
	mov	[edi+TAB_PATH],eax
    save_as_destination_failed:
	mov	esi,[save_as_destination]
	call	show_saving_error
	jmp	save_as_release_destination
    save_as_release_destination:
	mov	ebx,[save_as_title]
	call	release_memory
	mov	ebx,[save_as_destination]
	call	release_memory
	mov	eax,[save_as_current]
	call	fm_select
    save_as_cancelled:
	or	eax,-1
	retn

; save_destination_copy: saves the duplicate's edits elsewhere, without
; changing either tab. Returns eax = 0 on success, -1 on cancellation/error.
save_destination_copy:
	mov	byte [dialog_path],0
    choose_destination_copy:
	call	save_file_dialog
	jc	destination_copy_failed
	xor	ecx,ecx
    check_copy_path:
	cmp	ecx,[tabs_count]
	jae	write_destination_copy
	mov	eax,ecx
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	test	esi,esi
	jz	next_copy_path
	mov	edi,dialog_path
	call	compare_strings
	je	copy_path_in_use
    next_copy_path:
	inc	ecx
	jmp	check_copy_path
    copy_path_in_use:
	mov	esi,_copy_path_in_use
	mov	edi,_save_caption
	mov	eax,MB_ICONERROR+MB_OK
	call	message_box
	jmp	choose_destination_copy
    write_destination_copy:
	mov	ebx,dialog_path
	call	get_file_type
	test	eax,eax
	jz	copy_destination_available
	mov	esi,_copy_needs_new_file
	mov	edi,_save_caption
	mov	eax,MB_ICONERROR+MB_OK
	call	message_box
	jmp	choose_destination_copy
    copy_destination_available:
	mov	eax,[save_as_duplicate]
	call	tab_editor
	mov	edx,FM_GETTEXTLENGTH
	call	send_editor
	lea	ecx,[eax+1]
	call	get_memory
	test	eax,eax
	jz	destination_copy_error
	mov	[save_copy_memory],eax
	mov	[fe_lparam],eax
	mov	eax,[save_as_duplicate]
	call	tab_editor
	mov	edx,FM_GETTEXT
	call	send_editor
	mov	ecx,eax
	mov	esi,[save_copy_memory]
	mov	ebx,dialog_path
	call	save_new_file
	pushfd
	mov	ebx,[save_copy_memory]
	call	release_memory
	popfd
	jc	destination_copy_error
	xor	eax,eax
	retn
    destination_copy_error:
	mov	esi,dialog_path
	call	show_saving_error
    destination_copy_failed:
	or	eax,-1
	retn

; show_saving_error: esi = path

show_saving_error:
	mov	edi,string_buffer
	push	esi
	mov	esi,_saving_error
	call	copy_text
	pop	esi
	call	copy_text
	mov	esi,string_buffer
	mov	edi,_caption
	mov	eax,MB_ICONERROR+MB_OK
	jmp	message_box

; delete_tab: eax = index, removes tab and destroys its editor

delete_tab:
	mov	[deleted_tab],eax
	call	tab_record
	mov	ebx,[edi+TAB_PATH]
	call	release_memory
	mov	eax,[edi+TAB_EDITOR]
	push	eax
	mov	esi,edi
	add	esi,TAB_SIZE
	mov	ecx,[tabs_count]
	sub	ecx,[deleted_tab]
	dec	ecx
	shl	ecx,2
	rep	movsd
	dec	[tabs_count]
	mov	eax,[deleted_tab]
	cmp	eax,[assigned_file]
	jg	assigned_after_delete_ok
	je	assigned_deleted
	dec	[assigned_file]
	jmp	assigned_after_delete_ok
    assigned_deleted:
	mov	[assigned_file],-1
    assigned_after_delete_ok:
	mov	eax,[deleted_tab]
	cmp	eax,[current_tab]
	jg	current_after_delete_ok
	je	current_deleted
	dec	[current_tab]
	jmp	current_after_delete_ok
    current_deleted:
	or	[current_tab],-1
    current_after_delete_ok:
	pop	eax
	mov	edx,FM_DESTROY
	call	send_editor
	cmp	[current_tab],-1
	jne	tab_deleted
	mov	eax,[deleted_tab]
	mov	ecx,[tabs_count]
	jecxz	tab_deleted
	cmp	eax,ecx
	jb	select_after_delete
	lea	eax,[ecx-1]
    select_after_delete:
	call	fm_select
    tab_deleted:
	mov	ebx,[tabs_control]
	jmp	control_redraw

; close_tab: closes the selected tab (port of IDM_CLOSE)

close_tab:
	cmp	[tabs_count],1
	jbe	close_window
	mov	eax,[current_tab]
	call	tab_record
	mov	eax,[edi+TAB_EDITOR]
	mov	edx,FM_ISUNMODIFIED
	call	send_editor
	test	eax,eax
	jz	close_modified
	jmp	do_close
    close_modified:
	call	ask_to_save
	cmp	eax,IDCANCEL
	je	close_cancelled
	cmp	eax,IDNO
	je	do_close
	call	save_file_command
	test	eax,eax
	jnz	close_cancelled
    do_close:
	mov	eax,[current_tab]
	jmp	delete_tab
    close_cancelled:
	retn

; ask_to_save: asks about saving the selected file, returns eax = answer

ask_to_save:
	mov	eax,[current_tab]
	call	tab_record
	mov	edi,[edi+TAB_NAME]
	mov	esi,_saving_question
	mov	eax,MB_ICONQUESTION+MB_YESNOCANCEL
	or	eax,[close_flags]
	jmp	message_box

; close_window: checks all files and ends the program

close_window:
	xor	ebx,ebx
    check_before_exiting:
	cmp	ebx,[tabs_count]
	jae	check_done
	push	ebx
	mov	eax,ebx
	call	tab_editor
	mov	edx,FM_ISUNMODIFIED
	call	send_editor
	pop	ebx
	test	eax,eax
	jnz	check_next
	push	ebx
	mov	eax,ebx
	call	fm_select
	call	ask_to_save
	pop	ebx
	cmp	eax,IDCANCEL
	je	close_cancelled
	cmp	eax,IDNO
	je	check_next
	push	ebx
	call	save_file_command
	pop	ebx
	test	eax,eax
	jnz	close_cancelled
    check_next:
	inc	ebx
	jmp	check_before_exiting
    check_done:
	call	save_settings
	mov	[quit_flag],1
	retn

; process_arguments: esi = list of argument pointers (zero terminated),
; ebx = base directory (0 for current one)

process_arguments:
	mov	[arguments_base],ebx
    process_next_argument:
	lodsd
	test	eax,eax
	jz	arguments_processed
	push	esi
	mov	esi,eax
	xor	ecx,ecx
    argument_prefix:
	cmp	byte [esi],'+'
	je	argument_assign
	cmp	byte [esi],'-'
	je	argument_readonly
	cmp	byte [esi],0
	je	argument_done
	mov	[argument_flags],ecx
	mov	ebx,[arguments_base]
	mov	edi,argument_path
	call	full_path_name
	mov	esi,argument_path
	mov	ecx,[argument_flags]
	and	ecx,ARG_READONLY
	shr	ecx,1
	call	fm_open
	cmp	eax,-1
	je	argument_loading_error
	test	[argument_flags],ARG_ASSIGN
	jz	argument_done
	call	fm_assign
	jmp	argument_done
    argument_assign:
	or	ecx,ARG_ASSIGN
	inc	esi
	jmp	argument_prefix
    argument_readonly:
	or	ecx,ARG_READONLY
	inc	esi
	jmp	argument_prefix
    argument_loading_error:
	mov	esi,argument_path
	call	show_loading_error
    argument_done:
	pop	esi
	jmp	process_next_argument
    arguments_processed:
	retn

ARG_ASSIGN   = 1
ARG_READONLY = 2

; show_loading_error: esi = path

show_loading_error:
	mov	edi,string_buffer
	push	esi
	mov	esi,_loading_error
	call	copy_text
	pop	esi
	call	copy_text
	mov	al,'.'
	stosb
	mov	byte [edi],0
	mov	esi,string_buffer
	mov	edi,_caption
	mov	eax,MB_ICONERROR+MB_OK
	jmp	message_box

; open_with_viewer: esi = path or address to open with xdg-open

open_with_viewer:
	mov	[viewer_arguments+4],esi
	mov	eax,SYS_FORK
	int	80h
	test	eax,eax
	jnz	viewer_parent
	call	detach_process
	mov	eax,SYS_EXECVE
	mov	ebx,_xdg_open
	mov	ecx,viewer_arguments
	mov	edx,[environment]
	int	80h
	mov	eax,SYS_EXIT_GROUP
	mov	ebx,1
	int	80h
    viewer_parent:
	js	viewer_done
	mov	ebx,eax
	mov	eax,SYS_WAITPID
	xor	ecx,ecx
	xor	edx,edx
	int	80h
    viewer_done:
	retn

; detach_process: in child process forks again and exits the intermediate one

detach_process:
	mov	eax,SYS_FORK
	int	80h
	test	eax,eax
	jz	process_detached
	mov	eax,SYS_EXIT_GROUP
	xor	ebx,ebx
	int	80h
    process_detached:
	mov	eax,SYS_SETSID
	int	80h
	retn

include 'linux.inc'
include 'x11.inc'
include 'keyboard.inc'
include 'utf8.inc'
include 'syntax.inc'
include 'ui.inc'
include 'fedit.inc'
include 'dialogs.inc'
include 'compiler.inc'
include 'clipboard.inc'
include 'settings.inc'

segment readable writeable

include 'source/version.inc'
include 'ide/version.inc'
include 'resources.inc'
include 'data.inc'
