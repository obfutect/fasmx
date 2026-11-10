
; flat editor interface for X11
; Copyright (c) 2001-2014, Tomasz Grysztar.
; All rights reserved.

; Linux port of FEDIT.INC from the Win32 IDE: window messages became calls
; of fedit_proc, drawing goes through X11 requests, text cells are 32-bit.

SEGMENT_LENGTH	      = 100h
BLOCK_LENGTH	      = 400h * SEGMENT_LENGTH
SEGMENT_HEADER_LENGTH = 16
SEGMENT_DATA_LENGTH   = SEGMENT_LENGTH - SEGMENT_HEADER_LENGTH
SEGMENT_DATA_CHARS    = SEGMENT_DATA_LENGTH / 4

; flat editor mode flags

FEMODE_OVERWRITE   = 1
FEMODE_VERTICALSEL = 2
FEMODE_NOUNDO	   = 4
FEMODE_READONLY    = 8

; flat editor search flags

FEFIND_CASESENSITIVE = 1
FEFIND_WHOLEWORDS    = 2
FEFIND_BACKWARD      = 4
FEFIND_INWHOLETEXT   = 8

; flat editor styles

FES_AUTOINDENT	   = 0001h
FES_AUTOBRACKETS   = 0002h
FES_SMARTTABS	   = 0004h
FES_SECURESEL	   = 0008h
FES_OPTIMALFILL    = 0010h
FES_CONSOLECARET   = 0020h
FES_REVIVEDEADKEYS = 0040h
FES_TIMESCROLL	   = 0080h

; flat editor notifications

FEN_SETFOCUS	= 01h
FEN_KILLFOCUS	= 02h
FEN_TEXTCHANGE	= 03h
FEN_POSCHANGE	= 04h
FEN_MODECHANGE	= 05h
FEN_OUTOFMEMORY = 0Fh

; flat editor messages (parameters in fe_wparam, fe_lparam, fe_state)

FM_DESTROY	 = 1	; -
FM_PAINT	 = 2	; wparam = x+y shl 16, lparam = width+height shl 16
FM_SIZE 	 = 3	; wparam = width, lparam = height
FM_BLINK	 = 4	; -
FM_SETFOCUS	 = 5	; -
FM_KILLFOCUS	 = 6	; -
FM_KEYDOWN	 = 7	; wparam = keysym, lparam = keysym from first group
FM_LBUTTONDOWN	 = 8	; wparam = x, lparam = y
FM_LBUTTONUP	 = 9	; -
FM_MOUSEMOVE	 = 10	; wparam = x, lparam = y
FM_LBUTTONDBLCLK = 11	; wparam = x, lparam = y
FM_MOUSEWHEEL	 = 12	; wparam = delta (120 per step, positive = up)
FM_MOUSEHWHEEL	 = 13	; wparam = delta (positive = right)
FM_SETTEXT	 = 14	; lparam = zero-terminated string of cells
FM_GETTEXTLENGTH = 15	; returns maximum length of text in UTF-8
FM_GETTEXT	 = 16	; lparam = buffer, returns length of UTF-8 text
FM_UNDO 	 = 17	; -
FM_REDO 	 = 18	; -
FM_SETSYNTAXHIGHLIGHT = 19 ; wparam = colors, lparam = syntax procedure
FM_MOVE 	 = 20	; position in fe_x, fe_y, fe_width, fe_height
FM_SHOW 	 = 21	; wparam = 1 to show, 0 to hide
FM_VSCROLL	 = 22	; wparam = scroll code, lparam = position
FM_HSCROLL	 = 23	; wparam = scroll code, lparam = position
FM_SETFONT	 = 24	; wparam = font record
FM_SETTEXTCOLOR  = 25	; wparam = text color, lparam = background
FM_SETSELCOLOR	 = 26	; wparam = text color, lparam = background
FM_SETSTYLE	 = 27	; wparam = FES_* flags
FM_SETMODE	 = 28	; wparam = FEMODE_* flags
FM_GETMODE	 = 29
FM_SETPOS	 = 30	; wparam = FEPOS structure
FM_GETPOS	 = 31	; wparam = FEPOS structure
FM_GETLINELENGTH = 32	; wparam = line number (0 for caret line)
FM_GETLINE	 = 33	; wparam = line number, lparam = buffer for cells
FM_GETWORDATCARET = 34	; wparam = size of buffer in cells, lparam = buffer
FM_FINDFIRST	 = 35	; wparam = FEFIND_* flags, lparam = cells
FM_FINDNEXT	 = 36
FM_CANFINDNEXT	 = 37
FM_GETSEARCHTEXT = 38	; wparam = buffer, lparam = pointer to its size in cells
FM_GETSEARCHFLAGS = 39
FM_RELEASESEARCH = 40
FM_BEGINOPERATION = 41
FM_ENDOPERATION  = 42
FM_MARKUNMODIFIED = 43
FM_ISUNMODIFIED  = 44
FM_CANUNDO	 = 45
FM_CANREDO	 = 46
FM_EMPTYUNDOBUFFER = 47
FM_REPLACESEL	 = 48	; wparam = flag, lparam = cells
FM_COPY 	 = 49
FM_CUT		 = 50
FM_PASTE	 = 51
FM_CLEAR	 = 52

; internal style flags

FES_SCROLLBARS = 10000h
FES_DISABLED   = 20000h
FES_MAIN       = 40000h

; flat editor position structure

FEPOS_SELECTION_POSITION = 0
FEPOS_SELECTION_LINE	 = 4
FEPOS_CARET_POSITION	 = 8
FEPOS_CARET_LINE	 = 12
FEPOS_SIZE		 = 16

; font record

FONT_ID     = 0
FONT_WIDTH  = 4
FONT_HEIGHT = 8
FONT_ASCENT = 12
FONT_16BIT  = 16
FONT_RECORD_SIZE = 20

; key state flags

KS_SHIFT   = 1
KS_CONTROL = 2
KS_ALT	   = 4

CARET_WIDTH = 2

; fedit_create: eax = parent window, ecx = x, edx = y, esi = width,
; edi = height, [fedit_create_flags] = internal style flags,
; [building_dialog] = owner dialog (0 for main window)
; returns eax = editor handle (0 on failure)

fedit_create:
	push	ebx esi edi ebp
	mov	[editor_parent],eax
	mov	[editor_x],ecx
	mov	[editor_y],edx
	mov	[editor_width],esi
	mov	[editor_height],edi
	mov	ecx,editor_data_size
	call	get_memory
	test	eax,eax
	jz	fedit_create_failed
	mov	[fedit_handle],eax
	mov	[fedit_stack],esp
	call	init_editor_memory
	jc	fedit_create_failed
	mov	eax,[fedit_style]
	and	eax,0FFFFh
	or	eax,[fedit_create_flags]
	mov	[editor_style],eax
	mov	eax,[building_dialog]
	mov	[editor_owner],eax
	mov	[syntax_proc],no_syntax
	mov	[syntax_colors],0
	xor	eax,eax
	mov	[macro_operation],al
	mov	[current_operation],al
	mov	[focus],al
	mov	[mouse_select],al
	mov	[caret_shown],al
	mov	[screen_base],eax
	mov	[editor_vscroll],eax
	mov	[editor_hscroll],eax
	mov	[editor_visible],eax
	mov	eax,SEGMENT_DATA_CHARS*5
	mov	[line_buffer_size],eax
	mov	ecx,eax
	call	get_memory
	test	eax,eax
	jz	fedit_create_failed
	mov	[line_buffer],eax
	mov	eax,[editor_colors]
	mov	[text_color],eax
	mov	eax,[editor_colors+4]
	mov	[background_color],eax
	mov	eax,[editor_colors+8]
	mov	[selection_text],eax
	mov	eax,[editor_colors+12]
	mov	[selection_background],eax
	mov	eax,editor_font_record
	mov	[editor_font],eax
	mov	ecx,[eax+FONT_WIDTH]
	mov	[font_width],ecx
	mov	ecx,[eax+FONT_HEIGHT]
	mov	[font_height],ecx
	mov	ecx,[eax+FONT_ASCENT]
	mov	[font_ascent],ecx
	call	editor_geometry
	mov	[window_values],0
	mov	[window_values+4],1
	mov	[window_values+8],X11_ButtonPressMask+X11_ButtonReleaseMask+X11_ButtonMotionMask+X11_ExposureMask
	mov	eax,[text_cursor]
	mov	[window_values+12],eax
	mov	[window_value_mask],1+10h+800h+4000h
	mov	eax,[editor_parent]
	mov	ecx,[editor_x]
	mov	edx,[editor_y]
	mov	esi,[client_width]
	mov	edi,[client_height]
	call	x11_create_window
	mov	[editor_window],eax
	mov	ecx,WK_EDITOR
	mov	edx,[fedit_handle]
	mov	esi,[editor_owner]
	call	register_window
	test	dword [editor_style],FES_SCROLLBARS
	jz	editor_windows_created
	mov	eax,[editor_x]
	add	eax,[client_width]
	mov	[new_control_x],eax
	mov	eax,[editor_y]
	mov	[new_control_y],eax
	mov	[new_control_width],SCROLLBAR_SIZE
	mov	eax,[client_height]
	mov	[new_control_height],eax
	call	create_editor_scrollbar
	mov	[editor_vscroll],ebx
	mov	eax,[editor_x]
	mov	[new_control_x],eax
	mov	eax,[editor_y]
	add	eax,[client_height]
	mov	[new_control_y],eax
	mov	eax,[client_width]
	mov	[new_control_width],eax
	mov	[new_control_height],SCROLLBAR_SIZE
	call	create_editor_scrollbar
	mov	[editor_hscroll],ebx
    editor_windows_created:
	lea	esi,[editor_data]
	mov	edi,[fedit_handle]
	mov	ecx,editor_data_size shr 2
	rep	movsd
	mov	eax,[fedit_handle]
	pop	ebp edi esi ebx
	retn
    fedit_create_failed:
	xor	eax,eax
	pop	ebp edi esi ebx
	retn
    create_editor_scrollbar:
	mov	[new_control_type],CT_SCROLLBAR
	mov	[new_control_id],0
	mov	[new_control_style],0
	mov	[new_control_text],0
	mov	eax,[editor_parent]
	mov	[new_control_parent],eax
	mov	[new_control_notify],editor_scrolled
	mov	eax,[fedit_handle]
	mov	[new_control_notify_data],eax
	call	create_control
	test	ebx,ebx
	jz	editor_scrollbar_created
	mov	eax,[ebx+C_WINDOW]
	call	x11_unmap_window
    editor_scrollbar_created:
	retn

; editor_geometry: computes size of text window from editor_width/height

editor_geometry:
	mov	eax,[editor_width]
	mov	edx,[editor_height]
	test	dword [editor_style],FES_SCROLLBARS
	jz	editor_geometry_ok
	sub	eax,SCROLLBAR_SIZE
	sub	edx,SCROLLBAR_SIZE
    editor_geometry_ok:
	cmp	eax,1
	jge	editor_client_width_ok
	mov	eax,1
    editor_client_width_ok:
	cmp	edx,1
	jge	editor_client_height_ok
	mov	edx,1
    editor_client_height_ok:
	mov	[client_width],eax
	mov	[client_height],edx
	retn

; fedit_proc: processes message [fe_message] for editor [fedit_handle]
; returns eax = result

fedit_proc:
	push	ebx esi edi ebp
	mov	[fedit_stack],esp
	mov	esi,[fedit_handle]
	test	esi,esi
	jz	fedit_no_editor
	lea	edi,[editor_data]
	mov	ecx,editor_data_size shr 2
	rep	movsd
	mov	[return_value],0
	mov	[notification],0
	mov	eax,[fe_state]
	xor	dl,dl
	test	eax,X11_ShiftMask
	jz	shift_state_ok
	or	dl,KS_SHIFT
    shift_state_ok:
	test	eax,X11_ControlMask
	jz	editor_control_state_ok
	or	dl,KS_CONTROL
    editor_control_state_ok:
	test	eax,[x11_alt_mask]
	jz	alt_state_ok
	or	dl,KS_ALT
    alt_state_ok:
	mov	[key_flags],dl
	mov	eax,[fe_message]
	cmp	eax,FM_DESTROY
	je	wm_destroy
	cmp	eax,FM_PAINT
	je	wm_paint
	cmp	eax,FM_SIZE
	je	wm_size
	cmp	eax,FM_BLINK
	je	wm_blink
	cmp	eax,FM_MOVE
	je	wm_move
	cmp	eax,FM_SHOW
	je	wm_show
	test	dword [editor_style],FES_DISABLED
	jz	editor_input_allowed
	cmp	eax,FM_KEYDOWN
	jb	editor_input_allowed
	cmp	eax,FM_MOUSEHWHEEL
	jbe	fedit_input_ignored
	cmp	eax,FM_VSCROLL
	je	fedit_input_ignored
	cmp	eax,FM_HSCROLL
	je	fedit_input_ignored
    editor_input_allowed:
	mov	eax,[selection_line]
	or	eax,eax
	jz	no_selection
	cmp	eax,[caret_line]
	jne	selection_present
	mov	eax,[selection_position]
	cmp	eax,[caret_position]
	je	no_selection
    selection_present:
	mov	[was_selection],1
	jmp	selection_status_ok
    no_selection:
	mov	[was_selection],0
    selection_status_ok:
	xor	al,al
	xchg	[current_operation],al
	mov	[last_operation],al
	mov	eax,[fe_message]
	cmp	eax,FM_SETFOCUS
	je	wm_setfocus
	cmp	eax,FM_KILLFOCUS
	je	wm_killfocus
	cmp	eax,FM_KEYDOWN
	je	wm_keydown
	cmp	eax,FM_LBUTTONDOWN
	je	wm_lbuttondown
	cmp	eax,FM_LBUTTONUP
	je	wm_lbuttonup
	cmp	eax,FM_MOUSEMOVE
	je	wm_mousemove
	cmp	eax,FM_LBUTTONDBLCLK
	je	wm_lbuttondblclk
	cmp	eax,FM_MOUSEWHEEL
	je	wm_mousewheel
	cmp	eax,FM_MOUSEHWHEEL
	je	wm_mousehwheel
	cmp	eax,FM_SETTEXT
	je	wm_settext
	cmp	eax,FM_GETTEXTLENGTH
	je	wm_gettextlength
	cmp	eax,FM_GETTEXT
	je	wm_gettext
	cmp	eax,FM_UNDO
	je	wm_undo
	cmp	eax,FM_REDO
	je	fem_redo
	cmp	eax,FM_SETSYNTAXHIGHLIGHT
	je	fem_setsyntaxhighlight
	cmp	eax,FM_VSCROLL
	je	wm_vscroll
	cmp	eax,FM_HSCROLL
	je	wm_hscroll
	cmp	eax,FM_SETFONT
	je	wm_setfont
	cmp	eax,FM_SETTEXTCOLOR
	je	fem_settextcolor
	cmp	eax,FM_SETSELCOLOR
	je	fem_setselcolor
	cmp	eax,FM_SETSTYLE
	je	fem_setstyle
	cmp	eax,FM_SETMODE
	je	fem_setmode
	cmp	eax,FM_GETMODE
	je	fem_getmode
	cmp	eax,FM_SETPOS
	je	fem_setpos
	cmp	eax,FM_GETPOS
	je	fem_getpos
	cmp	eax,FM_GETLINELENGTH
	je	fem_getlinelength
	cmp	eax,FM_GETLINE
	je	fem_getline
	cmp	eax,FM_GETWORDATCARET
	je	fem_getwordatcaret
	cmp	eax,FM_FINDFIRST
	je	fem_findfirst
	cmp	eax,FM_FINDNEXT
	je	fem_findnext
	cmp	eax,FM_CANFINDNEXT
	je	fem_canfindnext
	cmp	eax,FM_GETSEARCHTEXT
	je	fem_getsearchtext
	cmp	eax,FM_GETSEARCHFLAGS
	je	fem_getsearchflags
	cmp	eax,FM_RELEASESEARCH
	je	fem_releasesearch
	cmp	eax,FM_BEGINOPERATION
	je	fem_beginoperation
	cmp	eax,FM_ENDOPERATION
	je	fem_endoperation
	cmp	eax,FM_MARKUNMODIFIED
	je	fem_setunmodified
	cmp	eax,FM_ISUNMODIFIED
	je	fem_isunmodified
	cmp	eax,FM_CANUNDO
	je	em_canundo
	cmp	eax,FM_CANREDO
	je	fem_canredo
	cmp	eax,FM_EMPTYUNDOBUFFER
	je	em_emptyundobuffer
	cmp	eax,FM_REPLACESEL
	je	em_replacesel
	cmp	eax,FM_COPY
	je	wm_copy
	cmp	eax,FM_CUT
	je	wm_cut
	cmp	eax,FM_PASTE
	je	wm_paste
	cmp	eax,FM_CLEAR
	je	wm_clear
	jmp	ignore
    fedit_input_ignored:
	mov	[was_selection],1
	jmp	ignore
    fedit_no_editor:
	xor	eax,eax
	jmp	finish_fedit_proc
wm_destroy:
	call	caret_hide
	mov	eax,[editor_window]
	call	unregister_window
	call	x11_destroy_window
	mov	ebx,[editor_vscroll]
	call	destroy_editor_scrollbar
	mov	ebx,[editor_hscroll]
	call	destroy_editor_scrollbar
	mov	ebx,[line_buffer]
	call	release_memory
	mov	ebx,[screen_base]
	call	release_memory
	call	release_search_data
	call	release_editor_memory
	mov	ebx,[fedit_handle]
	call	release_memory
	mov	[fedit_handle],0
	xor	eax,eax
	jmp	finish_fedit_proc
wm_paint:
	cmp	[screen_base],0
	je	done
	call	caret_hide
	movsx	eax,word [fe_wparam]
	movsx	ebx,word [fe_wparam+2]
	movzx	ecx,word [fe_lparam]
	movzx	edx,word [fe_lparam+2]
	lea	ecx,[eax+ecx-1]
	lea	edx,[ebx+edx-1]
	call	paint_area
	jmp	done
wm_blink:
	cmp	[focus],0
	je	caret_ok
	cmp	[caret_shown],0
	je	blink_on
	call	caret_hide
	jmp	caret_ok
    blink_on:
	call	update_caret_position
	jmp	caret_ok
wm_setfocus:
	or	[focus],-1
	call	create_caret
	mov	[notification],FEN_SETFOCUS
	cmp	[was_selection],0
	je	done
	jmp	moved_window
wm_killfocus:
	mov	[focus],0
	call	caret_hide
	mov	[notification],FEN_KILLFOCUS
	cmp	[was_selection],0
	je	done
	jmp	moved_window
    hscroll_ignore:
	jmp	done
    hscroll_left:
	cmp	[window_position],0
	je	hscroll_ignore
	dec	[window_position]
	jmp	moved_window
    hscroll_right:
	mov	eax,[maximum_position]
	sub	eax,[window_width]
	cmp	[window_position],eax
	jge	hscroll_ignore
	inc	[window_position]
	jmp	moved_window
    scrolling_up:
	mov	eax,[esi+4]
	or	eax,eax
	jz	scroll_ok
	dec	[window_line_number]
	mov	esi,eax
	loop	scrolling_up
	jmp	scroll_ok
    scrolling_down:
	mov	eax,esi
    scroll_one_line:
	mov	eax,[eax]
	test	eax,eax
	jz	scroll_ok
	btr	eax,0
	jc	scroll_one_line
	or	eax,eax
	jz	scroll_ok
	inc	[window_line_number]
	mov	esi,eax
	loop	scrolling_down
    scroll_ok:
	mov	[window_line],esi
	jmp	moved_window
wm_mousewheel:
	test	dword [editor_style],FES_TIMESCROLL
	jz	space_scroll
	test	[key_flags],KS_ALT
	jz	space_scroll
	test	[key_flags],KS_CONTROL
	jnz	time_scroll
    space_scroll:
	mov	esi,[window_line]
	mov	ecx,[wheel_scroll_lines]
	cmp	ecx,-1
	jne	calculate_scroll
	mov	ecx,[window_height]
    calculate_scroll:
	mov	eax,[fe_wparam]
	imul	ecx
	mov	ecx,120
	idiv	ecx
	mov	ecx,eax
	cmp	ecx,0
	jg	scrolling_up
	neg	ecx
	jnz	scrolling_down
	jmp	done
    time_scroll:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	test	dword [editor_mode],FEMODE_NOUNDO
	jnz	enable_undo
	mov	eax,[fe_wparam]
	cmp	eax,0
	jge	backward_in_time
	neg	eax
	mov	[fe_wparam],eax
	jmp	forward_in_time
      backward_in_time:
	sub	[fe_wparam],120
	jc	time_scroll_done
	mov	eax,[undo_data]
	test	eax,eax
	jz	time_scroll_done
	call	undo_changes
	jmp	backward_in_time
      forward_in_time:
	sub	[fe_wparam],120
	jc	time_scroll_done
	mov	eax,[redo_data]
	test	eax,eax
	jz	time_scroll_done
	call	redo_changes
	jmp	forward_in_time
      time_scroll_done:
	call	create_caret
	mov	[last_operation],0
	jmp	text_changed
wm_mousehwheel:
	mov	esi,[window_line]
	cmp	[fe_wparam],0
	jl	hscroll_left
	jg	hscroll_right
	jmp	done
wm_size:
	mov	eax,[fe_wparam]
	mov	[client_width],eax
	mov	eax,[fe_lparam]
	mov	[client_height],eax
    resize_screen:
	call	update_positions
	mov	eax,[screen_width]
	mul	[screen_height]
	mov	[screen_size],eax
	mov	[screen_offset],0
	lea	ecx,[eax*5]
	shl	ecx,1
	mov	ebx,[screen_base]
	call	release_memory
	xor	eax,eax
	jecxz	screen_allocated
	call	get_memory
    screen_allocated:
	mov	[screen_base],eax
	call	update_screen
	call	caret_hide
	xor	eax,eax
	xor	ebx,ebx
	mov	ecx,[client_width]
	mov	edx,[client_height]
	dec	ecx
	dec	edx
	call	paint_area
	jmp	done
wm_keydown:
	cmp	[was_selection],0
	jne	process_key
	mov	eax,[caret_line]
	mov	[selection_line],eax
	mov	eax,[caret_position]
	mov	[selection_position],eax
	mov	eax,[caret_line_number]
	mov	[selection_line_number],eax
    process_key:
	mov	eax,[fe_wparam]
	cmp	eax,XK_Left
	je	left_key
	cmp	eax,XK_Right
	je	right_key
	cmp	eax,XK_Up
	je	up_key
	cmp	eax,XK_Down
	je	down_key
	cmp	eax,XK_BackSpace
	je	backspace_key
	cmp	eax,XK_Home
	je	home_key
	cmp	eax,XK_End
	je	end_key
	cmp	eax,XK_Prior
	je	pgup_key
	cmp	eax,XK_Next
	je	pgdn_key
	cmp	eax,XK_Menu
	je	editor_menu_key
	cmp	eax,XK_Insert
	je	ins_key
	cmp	eax,XK_Delete
	je	del_key
	cmp	eax,XK_F6
	je	f6_key
	test	[key_flags],KS_CONTROL
	jz	convert_to_character
	mov	eax,[fe_lparam]
	or	eax,20h
	cmp	eax,'y'
	je	ctrl_y_key
    convert_to_character:
	test	[key_flags],KS_CONTROL+KS_ALT
	jnz	ignore
	mov	eax,[fe_wparam]
	cmp	eax,XK_Return
	je	return_character
	cmp	eax,XK_KP_Enter
	je	return_character
	cmp	eax,XK_Tab
	je	tab_character
	cmp	eax,XK_KP_Tab
	je	tab_character
	cmp	eax,XK_ISO_Left_Tab
	je	tab_character
	call	keysym_to_character
	test	eax,eax
	jnz	process_character
	test	dword [editor_style],FES_REVIVEDEADKEYS
	jz	ignore
	mov	eax,[fe_wparam]
	call	dead_keysym_to_character
	test	eax,eax
	jz	ignore
    process_character:
	cmp	eax,20h
	jae	character
	jmp	ignore
    return_character:
	mov	eax,0Dh
	jmp	character
    tab_character:
	mov	eax,9
	jmp	character
  left_key:
	test	[key_flags],KS_ALT
	jnz	scroll_left
	test	[key_flags],KS_CONTROL
	jnz	word_left
	cmp	[caret_position],0
	jle	moved_caret
	dec	[caret_position]
	jmp	moved_caret
  right_key:
	test	[key_flags],KS_ALT
	jnz	scroll_right
	test	[key_flags],KS_CONTROL
	jnz	word_right
	mov	eax,[caret_position]
	cmp	eax,[maximum_position]
	jae	moved_caret
	inc	[caret_position]
	jmp	moved_caret
  up_key:
	test	[key_flags],KS_ALT
	jnz	scroll_up
	call	move_line_up
	jmp	moved_caret
  down_key:
	test	[key_flags],KS_ALT
	jnz	scroll_down
	call	move_line_down
	jmp	moved_caret
  home_key:
	test	[key_flags],KS_CONTROL
	jnz	screen_home
	mov	[caret_position],0
	jmp	moved_caret
  end_key:
	test	[key_flags],KS_CONTROL
	jnz	screen_end
	call	move_to_line_end
	jmp	moved_caret
  screen_home:
	mov	eax,[window_line]
	mov	[caret_line],eax
	mov	eax,[window_line_number]
	mov	[caret_line_number],eax
	jmp	moved_caret
  screen_end:
	mov	eax,[window_line_number]
	add	eax,[window_height]
	dec	eax
	call	find_line
	mov	[caret_line],esi
	mov	[caret_line_number],ecx
	jmp	moved_caret
  pgup_key:
	test	[key_flags],KS_CONTROL
	jnz	text_home
	call	move_page_up
	jmp	moved_caret
  pgdn_key:
	test	[key_flags],KS_CONTROL
	jnz	text_end
	call	move_page_down
	jmp	moved_caret
  text_home:
	mov	eax,[first_line]
	mov	[caret_line],eax
	mov	[caret_line_number],1
	jmp	moved_caret
  text_end:
	or	eax,-1
	call	find_line
	mov	[caret_line],esi
	mov	[caret_line_number],ecx
	jmp	moved_caret
  word_left:
	call	move_to_previous_word
	jmp	moved_caret
  word_right:
	call	get_caret_segment
	call	move_to_next_word
	jmp	moved_caret
  scroll_left:
	cmp	[window_position],0
	je	scroll_done
	dec	[window_position]
	jmp	scroll_done
  scroll_right:
	inc	[window_position]
	jmp	scroll_done
  scroll_up:
	mov	esi,[window_line]
	mov	esi,[esi+4]
	or	esi,esi
	jz	scroll_done
	mov	[window_line],esi
	dec	[window_line_number]
	jmp	scroll_done
  scroll_down:
	mov	esi,[window_line]
      find_next_window_line:
	mov	esi,[esi]
	btr	esi,0
	jc	find_next_window_line
	or	esi,esi
	jz	scroll_done
	mov	[window_line],esi
	inc	[window_line_number]
      scroll_done:
	cmp	[was_selection],0
	jne	moved_window
	mov	[selection_line],0
	jmp	moved_window
  ins_key:
	test	[key_flags],KS_ALT
	jnz	switch_blocks
	test	[key_flags],KS_CONTROL
	jnz	wm_copy
	test	[key_flags],KS_SHIFT
	jnz	wm_paste
	xor	dword [editor_mode],FEMODE_OVERWRITE
	mov	[notification],FEN_MODECHANGE
	call	create_caret
	cmp	[was_selection],1
	je	done
	mov	[selection_line],0
	jmp	done
  del_key:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	test	[key_flags],KS_CONTROL
	jnz	wm_clear
	test	[key_flags],KS_SHIFT
	jnz	wm_cut
	cmp	[was_selection],0
	je	no_selection_on_del
	test	dword [editor_style],FES_SECURESEL
	jz	wm_clear
    no_selection_on_del:
	mov	esi,[caret_line]
	test	dword [editor_mode],FEMODE_OVERWRITE
	jnz	delete_char
	call	check_line_length
	cmp	ecx,[caret_position]
	ja	delete_char
	cmp	dword [esi],0
	je	ignore
	call	make_undopoint
	call	cut_line_break
	call	finish_edit
	jmp	text_altered
    delete_char:
	mov	[current_operation],1
	cmp	[last_operation],1
	jne	undopoint_for_delete
	mov	eax,[unmodified_state]
	cmp	eax,[undo_data]
	jne	undo_delete_ok
	or	[unmodified_state],-1
    undopoint_for_delete:
	call	make_undopoint
    undo_delete_ok:
	call	delete_character
	call	finish_edit
	jmp	text_altered
  backspace_key:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	test	[key_flags],KS_ALT
	jnz	undo_key
	cmp	[was_selection],0
	je	no_selection_to_clear
	test	dword [editor_style],FES_SECURESEL
	jz	wm_clear
    no_selection_to_clear:
	cmp	[caret_position],0
	je	line_back
	test	[key_flags],KS_CONTROL
	jnz	word_back
	mov	[current_operation],2
	cmp	[last_operation],2
	jne	backspace_undopoint
	mov	eax,[unmodified_state]
	cmp	eax,[undo_data]
	jne	undo_backspace_ok
	or	[unmodified_state],-1
	jmp	undo_backspace_ok
    backspace_undopoint:
	call	make_undopoint
    undo_backspace_ok:
	dec	[caret_position]
	call	delete_character
	call	finish_edit
	jmp	text_altered
    line_back:
	test	dword [editor_mode],FEMODE_OVERWRITE
	jnz	ignore
	mov	esi,[caret_line]
	mov	esi,[esi+4]
	or	esi,esi
	jz	ignore
	call	make_undopoint
	mov	[caret_line],esi
	dec	[caret_line_number]
	call	check_line_length
	mov	[caret_position],ecx
	call	cut_line_break
	call	finish_edit
	jmp	text_altered
    word_back:
	call	make_undopoint
	push	[caret_position]
	mov	esi,[caret_line]
	xor	eax,eax
	xchg	eax,[esi+4]
	push	eax
	call	move_to_previous_word
	pop	eax
	mov	esi,[caret_line]
	mov	[esi+4],eax
	pop	ecx
	sub	ecx,[caret_position]
	call	delete_from_line
	call	finish_edit
	jmp	text_altered
  character:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	cmp	eax,0Dh
	je	enter_key
	cmp	eax,9
	je	tab_key
	cmp	[was_selection],0
	je	no_selection_to_replace
	call	make_undopoint
	test	dword [editor_style],FES_SECURESEL
	jnz	character_undo_ok
	push	eax
	call	delete_block
	pop	eax
	call	put_character
	call	finish_edit
	jmp	text_altered
    no_selection_to_replace:
	mov	[current_operation],3
	cmp	[last_operation],3
	jne	character_undopoint
	mov	edx,[unmodified_state]
	cmp	edx,[undo_data]
	jne	character_undo_ok
	or	[unmodified_state],-1
	jmp	character_undo_ok
    character_undopoint:
	call	make_undopoint
    character_undo_ok:
	call	put_character
	call	finish_edit
	jmp	text_altered
  tab_key:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	call	make_undopoint
	cmp	[was_selection],0
	je	tab_securesel
	test	dword [editor_style],FES_SECURESEL
	jnz	tab_securesel
	call	delete_block
    tab_securesel:
	call	tabulate
	call	finish_edit
	jmp	text_altered
  enter_key:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	call	make_undopoint
	cmp	[was_selection],0
	je	enter_secureselection_ok
	test	dword [editor_style],FES_SECURESEL
	jnz	enter_secureselection_ok
	call	delete_block
    enter_secureselection_ok:
	call	carriage_return
	test	dword [editor_mode],FEMODE_OVERWRITE
	jnz	text_altered
	call	finish_edit
	jmp	text_altered
  ctrl_y_key:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	call	make_undopoint
	call	remove_line
	jmp	text_altered
  f6_key:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	call	make_undopoint
	call	duplicate_line
	jmp	text_altered
  editor_menu_key:
	jmp	ignore
  switch_blocks:
	xor	dword [editor_mode],FEMODE_VERTICALSEL
	mov	[notification],FEN_MODECHANGE
	cmp	[was_selection],0
	je	ignore
	jmp	moved_window
wm_lbuttondown:
	cmp	[was_selection],0
	jne	selection_ok
	mov	eax,[caret_line]
	mov	[selection_line],eax
	mov	eax,[caret_position]
	mov	[selection_position],eax
	mov	eax,[caret_line_number]
	mov	[selection_line_number],eax
    selection_ok:
	call	get_mouse_position
	or	[mouse_select],-1
	jmp	moved_caret
    get_mouse_position:
	mov	eax,[fe_wparam]
	test	dword [editor_style],FES_CONSOLECARET
	jnz	get_mouse_column
	test	dword [editor_mode],FEMODE_OVERWRITE
	jnz	get_mouse_column
	mov	ebx,[font_width]
	shr	ebx,1
	adc	eax,ebx
      get_mouse_column:
	cdq
	idiv	[font_width]
	add	eax,[window_position]
	cmp	eax,[maximum_position]
	jg	mouse_out_of_line
	cmp	eax,0
	jge	click_position_ok
	xor	eax,eax
	jmp	click_position_ok
    mouse_out_of_line:
	mov	eax,[maximum_position]
    click_position_ok:
	mov	[caret_position],eax
	mov	eax,[fe_lparam]
	cdq
	idiv	[font_height]
	add	eax,[window_line_number]
	cmp	eax,0
	jg	click_line_ok
	mov	eax,1
    click_line_ok:
	call	find_line
	mov	[caret_line],esi
	mov	[caret_line_number],ecx
	retn
wm_lbuttonup:
	mov	[mouse_select],0
	jmp	done
wm_mousemove:
	cmp	[mouse_select],0
	je	ignore
	cmp	[was_selection],0
	jne	select
	mov	eax,[caret_line]
	mov	ebx,[caret_line_number]
	mov	[selection_line],eax
	mov	[selection_line_number],ebx
	mov	eax,[caret_position]
	mov	[selection_position],eax
    select:
	call	get_mouse_position
	jmp	moved_selection
wm_lbuttondblclk:
	mov	[mouse_select],0
	call	get_mouse_position
	call	get_word_at_caret
	mov	[selection_position],edx
	add	edx,ecx
	mov	[caret_position],edx
	mov	eax,[caret_line]
	mov	[selection_line],eax
	mov	eax,[caret_line_number]
	mov	[selection_line_number],eax
	jmp	moved_selection
wm_copy:
	cmp	[was_selection],0
	je	ignore
	call	copy_to_clipboard
	jmp	ignore
    copy_to_clipboard:
	call	get_block_length
	lea	ecx,[ecx*4+8]
	call	get_memory
	test	eax,eax
	jz	clipboard_copy_failed
	push	ebx
	mov	edi,eax
	call	copy_block
	mov	esi,[esp]
	xor	ecx,ecx
    count_copied_cells:
	cmp	dword [esi+ecx*4],0
	je	copied_cells_counted
	inc	ecx
	jmp	count_copied_cells
    copied_cells_counted:
	call	clipboard_set_cells
	pop	ebx
	pushfd
	call	release_memory
	popfd
	retn
    clipboard_copy_failed:
	stc
	retn
wm_cut:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	cmp	[was_selection],0
	je	ignore
	call	copy_to_clipboard
	jc	ignore
	jmp	wm_clear
wm_paste:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	call	clipboard_get_cells
	test	eax,eax
	jz	ignore
	mov	[pasted_cells],eax
	call	make_undopoint
	cmp	[was_selection],0
	je	paste_secureselection_ok
	test	dword [editor_style],FES_SECURESEL
	jnz	paste_secureselection_ok
	call	delete_block
    paste_secureselection_ok:
	mov	esi,[pasted_cells]
	call	insert_block
	jc	paste_failed
	mov	ebx,[pasted_cells]
	call	release_memory
	test	dword [editor_style],FES_SECURESEL
	jz	no_selection_after_paste
	mov	eax,[caret_line]
	mov	ecx,[caret_line_number]
	mov	edx,[caret_position]
	xchg	eax,[selection_line]
	xchg	ecx,[selection_line_number]
	xchg	edx,[selection_position]
	mov	[caret_line],eax
	mov	[caret_line_number],ecx
	mov	[caret_position],edx
	jmp	text_changed
    no_selection_after_paste:
	mov	[selection_line],0
	jmp	text_changed
    paste_failed:
	call	undo_changes
	mov	ebx,[pasted_cells]
	call	release_memory
	jmp	text_changed
wm_clear:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	cmp	[was_selection],0
	je	ignore
	call	make_undopoint
	call	delete_block
	mov	[selection_line],0
	jmp	text_changed
wm_settext:
	mov	esi,[fe_lparam]
	call	set_text
	mov	[return_value],1
	jmp	text_changed
wm_gettextlength:
	mov	esi,[first_line]
    count_text_length:
	mov	eax,[esi+8]
	shl	eax,2
	add	[return_value],eax
    skip_counted_line_segments:
	mov	esi,[esi]
	or	esi,esi
	jz	ignore
	btr	esi,0
	jc	skip_counted_line_segments
	inc	[return_value]
	jmp	count_text_length
wm_gettext:
	mov	esi,[first_line]
	mov	edi,[fe_lparam]
    copy_text_line:
	mov	ecx,[esi+8]
	mov	[gettext_length],ecx
	push	edi
	mov	edi,[line_buffer]
	xor	edx,edx
	call	copy_from_line
	mov	[gettext_next_line],esi
	pop	edi
	mov	esi,[line_buffer]
	mov	ecx,[gettext_length]
	jecxz	text_line_copied
	test	dword [editor_style],FES_OPTIMALFILL
	jz	direct_text_copy
	cmp	ecx,8
	jb	direct_text_copy
	push	edi
	lea	edi,[esi+ecx*4]
	mov	ebx,edi
	xor	eax,eax
	rep	stosb
	mov	edi,ebx
	mov	ecx,[gettext_length]
	call	[syntax_proc]
	pop	edi
	mov	ecx,[gettext_length]
	xor	edx,edx
	push	ebp
	xor	ebp,ebp
    optimal_fill:
	mov	eax,[esi+ebp*4]
	inc	ebp
	cmp	eax,20h
	jne	store_text_character
	cmp	byte [ebx+ebp-1],0
	jne	store_text_character
	test	ebp,111b
	jz	store_tab
	inc	edx
	mov	byte [edi],20h
	inc	edi
	loop	optimal_fill
	jmp	optimal_fill_done
    store_tab:
	or	edx,edx
	jz	store_text_character
	sub	edi,edx
	mov	byte [edi],9
	inc	edi
	xor	edx,edx
	loop	optimal_fill
	jmp	optimal_fill_done
    store_text_character:
	call	utf8_encode
	xor	edx,edx
	loop	optimal_fill
    optimal_fill_done:
	pop	ebp
	jmp	text_line_copied
    direct_text_copy:
	lodsd
	call	utf8_encode
	loop	direct_text_copy
    text_line_copied:
	mov	esi,[gettext_next_line]
	or	esi,esi
	jz	text_copied
	mov	byte [edi],0Ah
	inc	edi
	jmp	copy_text_line
    text_copied:
	mov	byte [edi],0
	sub	edi,[fe_lparam]
	mov	[return_value],edi
	jmp	ignore
wm_undo:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	test	dword [editor_mode],FEMODE_NOUNDO
	jnz	enable_undo
	mov	eax,[undo_data]
	test	eax,eax
	jz	ignore
	call	undo_changes
	call	create_caret
	mov	[last_operation],0
	jmp	text_changed
    enable_undo:
	and	dword [editor_mode],not FEMODE_NOUNDO
	jmp	ignore
    undo_key:
	test	[key_flags],KS_SHIFT
	jz	wm_undo
fem_redo:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	cmp	[redo_data],0
	je	ignore
	call	redo_changes
	call	create_caret
	mov	[last_operation],0
	jmp	text_changed
wm_move:
	mov	eax,[fe_x]
	mov	[editor_x],eax
	mov	eax,[fe_y]
	mov	[editor_y],eax
	mov	eax,[fe_width]
	mov	[editor_width],eax
	mov	eax,[fe_height]
	mov	[editor_height],eax
	call	editor_geometry
	mov	eax,[editor_window]
	mov	ecx,[editor_x]
	mov	edx,[editor_y]
	mov	esi,[client_width]
	mov	edi,[client_height]
	call	x11_move_resize_window
	mov	ebx,[editor_vscroll]
	test	ebx,ebx
	jz	editor_scrollbars_moved
	add	ecx,esi
	mov	edi,[client_height]
	mov	esi,SCROLLBAR_SIZE
	call	move_control
	mov	ebx,[editor_hscroll]
	mov	ecx,[editor_x]
	mov	edx,[editor_y]
	add	edx,[client_height]
	mov	esi,[client_width]
	mov	edi,SCROLLBAR_SIZE
	call	move_control
    editor_scrollbars_moved:
	jmp	resize_screen
wm_show:
	mov	eax,[fe_wparam]
	mov	[editor_visible],eax
	test	eax,eax
	jnz	show_editor
	call	caret_hide
	mov	eax,[editor_window]
	call	x11_unmap_window
	mov	ebx,[editor_vscroll]
	test	ebx,ebx
	jz	done
	mov	eax,[ebx+C_WINDOW]
	call	x11_unmap_window
	mov	ebx,[editor_hscroll]
	mov	eax,[ebx+C_WINDOW]
	call	x11_unmap_window
	jmp	done
    show_editor:
	mov	eax,[editor_window]
	call	x11_map_window
	mov	ebx,[editor_vscroll]
	test	ebx,ebx
	jz	done
	mov	eax,[ebx+C_WINDOW]
	call	x11_map_window
	mov	ebx,[editor_hscroll]
	mov	eax,[ebx+C_WINDOW]
	call	x11_map_window
	jmp	done
wm_vscroll:
	mov	eax,[fe_wparam]
	cmp	eax,SB_LINEUP
	je	vscroll_up
	cmp	eax,SB_LINEDOWN
	je	vscroll_down
	cmp	eax,SB_THUMBTRACK
	je	vscroll_pos
	cmp	eax,SB_PAGEUP
	je	vscroll_pageup
	cmp	eax,SB_PAGEDOWN
	je	vscroll_pagedown
	jmp	done
    vscroll_up:
	mov	esi,[window_line]
	mov	esi,[esi+4]
	or	esi,esi
	jz	done
	dec	[window_line_number]
	mov	[window_line],esi
	jmp	moved_window
    vscroll_down:
	mov	eax,[window_line_number]
	add	eax,[window_height]
	cmp	eax,[lines_count]
	ja	done
	mov	esi,[window_line]
    vscroll_skip_line:
	mov	esi,[esi]
	btr	esi,0
	jc	vscroll_skip_line
	or	esi,esi
	jz	done
	inc	[window_line_number]
	mov	[window_line],esi
	jmp	moved_window
    vscroll_pos:
	mov	eax,[fe_lparam]
	call	find_line
	or	esi,esi
	jz	done
	mov	[window_line],esi
	mov	[window_line_number],ecx
	jmp	moved_window
    vscroll_pageup:
	mov	esi,[window_line]
	mov	ecx,[window_height]
	test	ecx,ecx
	jz	done
	jmp	scrolling_up
    vscroll_pagedown:
	mov	esi,[window_line]
	mov	eax,[window_line_number]
	add	eax,[window_height]
	mov	ecx,[lines_count]
	sub	ecx,eax
	inc	ecx
	cmp	ecx,[window_height]
	jbe	vscroll_pagedown_count_ok
	mov	ecx,[window_height]
    vscroll_pagedown_count_ok:
	cmp	ecx,0
	jle	done
	jmp	scrolling_down
wm_hscroll:
	mov	eax,[fe_wparam]
	cmp	eax,SB_LINEUP
	je	hscroll_left
	cmp	eax,SB_LINEDOWN
	je	hscroll_right
	cmp	eax,SB_THUMBTRACK
	je	hscroll_pos
	cmp	eax,SB_PAGEUP
	je	hscroll_wleft
	cmp	eax,SB_PAGEDOWN
	je	hscroll_wright
	jmp	done
    hscroll_pos:
	mov	eax,[fe_lparam]
	mov	[window_position],eax
	jmp	moved_window
    hscroll_wleft:
	mov	eax,[window_width]
	sub	[window_position],eax
	jnc	moved_window
	mov	[window_position],0
	jmp	moved_window
    hscroll_wright:
	mov	eax,[window_width]
	mov	ecx,[maximum_position]
	sub	ecx,eax
	add	[window_position],eax
	cmp	[window_position],ecx
	jbe	moved_window
	mov	[window_position],ecx
	jmp	moved_window
wm_setfont:
	mov	esi,[fe_wparam]
	mov	[editor_font],esi
	mov	eax,[esi+FONT_WIDTH]
	mov	[font_width],eax
	mov	eax,[esi+FONT_HEIGHT]
	mov	[font_height],eax
	mov	eax,[esi+FONT_ASCENT]
	mov	[font_ascent],eax
	cmp	[focus],0
	je	resize_screen
	call	create_caret
	jmp	resize_screen
fem_settextcolor:
	mov	eax,[fe_wparam]
	mov	ebx,[fe_lparam]
	mov	[text_color],eax
	mov	[background_color],ebx
	jmp	resize_screen
fem_setselcolor:
	mov	eax,[fe_wparam]
	mov	ebx,[fe_lparam]
	mov	[selection_text],eax
	mov	[selection_background],ebx
	jmp	resize_screen
fem_setstyle:
	mov	eax,[fe_wparam]
	and	eax,0FFFFh
	and	dword [editor_style],not 0FFFFh
	or	[editor_style],eax
	cmp	[focus],0
	je	done
	call	create_caret
	jmp	done
em_canundo:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	mov	eax,[undo_data]
	or	eax,eax
	jz	ignore
	mov	[return_value],1
	jmp	ignore
fem_canredo:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	mov	eax,[redo_data]
	or	eax,eax
	jz	ignore
	mov	[return_value],1
	jmp	ignore
fem_isunmodified:
	mov	eax,[undo_data]
	cmp	eax,[unmodified_state]
	jne	ignore
	mov	[return_value],1
	jmp	ignore
em_emptyundobuffer:
	call	clear_redo_data
	call	clear_undo_data
	jmp	ignore
fem_setunmodified:
	mov	eax,[undo_data]
	mov	[unmodified_state],eax
	jmp	ignore
em_replacesel:
	test	dword [editor_mode],FEMODE_VERTICALSEL + FEMODE_OVERWRITE + FEMODE_READONLY
	jnz	ignore
	cmp	[undo_data],0
	je	replacesel_undopoint
	cmp	[fe_wparam],0
	je	replace_undo_ok
    replacesel_undopoint:
	call	make_undopoint
    replace_undo_ok:
	cmp	[was_selection],0
	je	simple_replace
	push	[caret_line_number]
	push	[caret_position]
	call	delete_block
	pop	edx ecx
	cmp	ecx,[caret_line_number]
	jne	simple_replace
	cmp	edx,[caret_position]
	jne	simple_replace
	mov	esi,[fe_lparam]
	call	insert_block
	mov	esi,[caret_line]
	mov	ecx,[caret_line_number]
	mov	edx,[caret_position]
	xchg	esi,[selection_line]
	xchg	ecx,[selection_line_number]
	xchg	edx,[selection_position]
	mov	[caret_line],esi
	mov	[caret_line_number],ecx
	mov	[caret_position],edx
	jmp	text_changed
    simple_replace:
	mov	esi,[fe_lparam]
	call	insert_block
	jmp	text_changed
fem_setmode:
	mov	eax,[fe_wparam]
	xchg	[editor_mode],eax
	cmp	eax,[editor_mode]
	je	ignore
	mov	[notification],FEN_MODECHANGE
	call	create_caret
	cmp	[was_selection],0
	jne	moved_window
	jmp	done
fem_getmode:
	mov	eax,[editor_mode]
	mov	[return_value],eax
	jmp	ignore
fem_setpos:
	mov	edi,[fe_wparam]
	cmp	[selection_line],0
	jne	selection_current_ok
	mov	eax,[caret_line]
	mov	[selection_line],eax
	mov	eax,[caret_line_number]
	mov	[selection_line_number],eax
	mov	eax,[caret_position]
	mov	[selection_position],eax
    selection_current_ok:
	mov	eax,[edi+FEPOS_SELECTION_LINE]
	or	eax,eax
	jz	selection_line_ok
	call	find_line
	mov	[selection_line],esi
	mov	[selection_line_number],ecx
    selection_line_ok:
	mov	eax,[edi+FEPOS_SELECTION_POSITION]
	sub	eax,1
	jc	selection_position_set
	mov	[selection_position],eax
    selection_position_set:
	mov	eax,[edi+FEPOS_CARET_LINE]
	or	eax,eax
	jz	caret_line_ok
	call	find_line
	mov	[caret_line],esi
	mov	[caret_line_number],ecx
    caret_line_ok:
	mov	eax,[edi+FEPOS_CARET_POSITION]
	sub	eax,1
	jc	moved_selection
	mov	[caret_position],eax
	jmp	moved_selection
fem_getpos:
	mov	edi,[fe_wparam]
	mov	eax,[caret_line_number]
	mov	[edi+FEPOS_SELECTION_LINE],eax
	mov	[edi+FEPOS_CARET_LINE],eax
	mov	eax,[caret_position]
	inc	eax
	mov	[edi+FEPOS_SELECTION_POSITION],eax
	mov	[edi+FEPOS_CARET_POSITION],eax
	cmp	[selection_line],0
	je	ignore
	mov	eax,[selection_line_number]
	mov	[edi+FEPOS_SELECTION_LINE],eax
	mov	eax,[selection_position]
	inc	eax
	mov	[edi+FEPOS_SELECTION_POSITION],eax
	jmp	ignore
fem_getlinelength:
	mov	esi,[caret_line]
	mov	eax,[fe_wparam]
	or	eax,eax
	jz	return_length
	call	find_line
	cmp	ecx,[fe_wparam]
	je	return_length
	or	[return_value],-1
	jmp	ignore
    return_length:
	mov	ecx,[esi+8]
	mov	[return_value],ecx
	jmp	ignore
fem_getline:
	mov	esi,[caret_line]
	mov	eax,[fe_wparam]
	or	eax,eax
	jz	get_line
	call	find_line
	cmp	ecx,[fe_wparam]
	jne	ignore
    get_line:
	mov	ecx,[esi+8]
	mov	[return_value],ecx
	mov	edi,[fe_lparam]
	xor	edx,edx
	call	copy_from_line
	xor	eax,eax
	stosd
	jmp	ignore
fem_findfirst:
	mov	esi,[fe_lparam]
	mov	eax,[fe_wparam]
	call	find_first
	jnc	show_found_text
	jmp	ignore
fem_findnext:
	call	find_next
	jc	ignore
    show_found_text:
	mov	[return_value],1
	cmp	[macro_operation],0
	jne	moved_selection
	mov	eax,[caret_position]
	xchg	eax,[selection_position]
	mov	[caret_position],eax
	call	update_window
	call	let_caret_appear
	call	update_positions
	mov	eax,[caret_position]
	xchg	eax,[selection_position]
	mov	[caret_position],eax
	jmp	moved_selection
fem_getsearchtext:
	call	get_search_text_length
	mov	edx,[fe_lparam]
	jc	store_required_buffer_size
	inc	ecx
	cmp	ecx,[edx]
	ja	store_required_buffer_size
	mov	edi,[fe_wparam]
	call	get_search_text
	mov	[return_value],1
	jmp	ignore
     store_required_buffer_size:
	mov	[edx],ecx
	jmp	ignore
fem_getsearchflags:
	cmp	[search_data],0
	je	ignore
	mov	eax,[search_flags]
	mov	[return_value],eax
	jmp	ignore
fem_releasesearch:
	call	release_search_data
	jmp	ignore
fem_canfindnext:
	cmp	[search_data],0
	je	ignore
	mov	[return_value],1
	jmp	ignore
fem_getwordatcaret:
	cmp	[fe_wparam],0
	je	ignore
	call	get_word_at_caret
	mov	edi,[fe_lparam]
	mov	eax,[fe_wparam]
	dec	eax
	jz	word_copied
	cmp	ecx,eax
	jbe	copy_word
	mov	ecx,eax
    copy_word:
	mov	[return_value],ecx
	mov	esi,[caret_line]
	call	copy_from_line
    word_copied:
	xor	eax,eax
	stosd
	jmp	ignore
fem_beginoperation:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	mov	[macro_operation],1
	call	store_status_for_undo
	jmp	done
fem_endoperation:
	test	dword [editor_mode],FEMODE_READONLY
	jnz	ignore
	xor	al,al
	xchg	al,[macro_operation]
	test	al,2
	jnz	text_changed
	test	al,4
	jz	update_all
	mov	[notification],FEN_POSCHANGE
	jmp	update_all
fem_setsyntaxhighlight:
	mov	eax,[fe_wparam]
	mov	ebx,[fe_lparam]
	mov	[syntax_colors],eax
	mov	[syntax_proc],ebx
	or	eax,eax
	jnz	resize_screen
	mov	[syntax_proc],no_syntax
	jmp	resize_screen
moved_caret:
	test	[key_flags],KS_SHIFT
	jnz	moved_selection
	mov	[selection_line],0
moved_selection:
	cmp	[macro_operation],0
	jne	position_changed_by_macro_operation
	mov	[notification],FEN_POSCHANGE
	jmp	update_all
moved_window:
	call	update_positions
	jmp	refresh
text_altered:
	mov	[selection_line],0
text_changed:
	cmp	[macro_operation],0
	jne	text_changed_by_macro_operation
	mov	[notification],FEN_TEXTCHANGE
	mov	eax,[peak_line_length]
	xor	edx,edx
	mov	ebx,SEGMENT_DATA_CHARS
	div	ebx
	inc	eax
	imul	eax,SEGMENT_DATA_CHARS*5
	cmp	eax,[line_buffer_size]
	je	update_all
	mov	[line_buffer_size],eax
	mov	ecx,eax
	call	get_memory
	or	eax,eax
	jz	memory_shortage
	xchg	[line_buffer],eax
	mov	ebx,eax
	call	release_memory
    update_all:
	cmp	[macro_operation],0
	jne	done
	call	update_window
	call	let_caret_appear
	call	update_positions
    refresh:
	cmp	[macro_operation],0
	jne	done
	cmp	[screen_base],0
	je	resize_screen
	mov	eax,[screen_size]
	lea	eax,[eax*5]
	xor	[screen_offset],eax
	call	update_screen
	call	caret_hide
	call	paint_changes
	jmp	done
    position_changed_by_macro_operation:
	or	[macro_operation],4
	jmp	done
    text_changed_by_macro_operation:
	or	[macro_operation],2
	jmp	done
ignore:
	mov	dl,[last_operation]
	mov	[current_operation],dl
	cmp	[was_selection],0
	jne	done
	mov	[selection_line],0
done:
	cmp	[focus],0
	je	caret_ok
	call	update_caret_position
    caret_ok:
	lea	esi,[editor_data]
	mov	edi,[fedit_handle]
	mov	ecx,editor_data_size shr 2
	rep	movsd
	cmp	[notification],0
	je	notification_ok
	push	[return_value]
	movzx	eax,[notification]
	call	editor_notification
	pop	[return_value]
    notification_ok:
	mov	eax,[return_value]
finish_fedit_proc:
	pop	ebp edi esi ebx
	retn
not_enough_memory:
	mov	esp,[fedit_stack]
	mov	[notification],FEN_OUTOFMEMORY
	or	[return_value],-1
	jmp	ignore
make_undopoint:
	cmp	[macro_operation],0
	jne	undopoint_ok
	call	store_status_for_undo
    undopoint_ok:
	retn
update_positions:
	mov	eax,[client_width]
	xor	edx,edx
	div	[font_width]
	mov	[window_width],eax
	add	edx,-1
	adc	eax,0
	mov	[screen_width],eax
	mov	eax,[client_height]
	xor	edx,edx
	div	[font_height]
	mov	[window_height],eax
	add	edx,-1
	adc	eax,0
	mov	[screen_height],eax
	call	update_window
	push	esi edi
	mov	ebx,[editor_vscroll]
	test	ebx,ebx
	jz	editor_scrollbars_set
	mov	eax,1
	mov	ecx,[lines_count]
	mov	edx,[window_height]
	mov	esi,[window_line_number]
	call	set_scroll_info
	mov	ebx,[editor_hscroll]
	xor	eax,eax
	mov	ecx,[maximum_position]
	mov	edx,[window_width]
	mov	esi,[window_position]
	call	set_scroll_info
    editor_scrollbars_set:
	pop	edi esi
    setup_caret:
	mov	eax,[font_width]
	mov	edx,[caret_position]
	sub	edx,[window_position]
	imul	eax,edx
	mov	[caret_x],eax
	mov	eax,[font_height]
	mov	edx,[caret_line_number]
	sub	edx,[window_line_number]
	imul	eax,edx
	mov	[caret_y],eax
	retn

; caret is drawn by inverting a rectangle of the window

create_caret:
	call	caret_hide
	cmp	[focus],0
	je	caret_drawn
update_caret_position:
	mov	eax,[caret_x]
	mov	ebx,[caret_y]
	mov	ecx,[font_width]
	mov	edx,[font_height]
	test	dword [editor_mode],FEMODE_OVERWRITE
	jnz	caret_shape_ok
	test	dword [editor_style],FES_CONSOLECARET
	jz	insert_caret_shape
	lea	ebx,[ebx+edx-2]
	mov	edx,2
	jmp	caret_shape_ok
    insert_caret_shape:
	mov	ecx,CARET_WIDTH
    caret_shape_ok:
	cmp	[caret_shown],0
	je	draw_caret
	cmp	eax,[caret_shown_x]
	jne	move_caret
	cmp	ebx,[caret_shown_y]
	jne	move_caret
	cmp	ecx,[caret_shown_width]
	jne	move_caret
	cmp	edx,[caret_shown_height]
	je	caret_drawn
    move_caret:
	push	eax ebx ecx edx
	call	caret_hide
	pop	edx ecx ebx eax
    draw_caret:
	mov	[caret_shown_x],eax
	mov	[caret_shown_y],ebx
	mov	[caret_shown_width],ecx
	mov	[caret_shown_height],edx
	mov	[caret_shown],1
	call	invert_caret
    caret_drawn:
	retn
caret_hide:
	cmp	[caret_shown],0
	je	caret_hidden
	mov	[caret_shown],0
	call	invert_caret
    caret_hidden:
	retn
invert_caret:
	push	eax ebx ecx edx esi edi
	mov	eax,[editor_window]
	mov	ebx,[caret_gc]
	mov	ecx,[caret_shown_x]
	mov	edx,[caret_shown_y]
	mov	esi,[caret_shown_width]
	mov	edi,[caret_shown_height]
	call	x11_fill_rectangle
	pop	edi esi edx ecx ebx eax
	retn

; screen buffer contains two copies of the screen, each one made of
; screen_size cells (32-bit) followed by screen_size attribute bytes
; (low bits - syntax color, bit 7 - selection)

update_screen:
	mov	edi,[screen_base]
	or	edi,edi
	jz	screen_prepared
	add	edi,[screen_offset]
	mov	ebx,[screen_size]
	lea	ebx,[edi+ebx*4]
	push	ebx
	mov	ecx,[screen_height]
	mov	edx,[window_line_number]
	mov	[line_number],edx
	mov	esi,[window_line]
    prepare_screen:
	push	ecx
	test	byte [esi],1
	jnz	prepare_long_line
	push	esi
	add	esi,SEGMENT_HEADER_LENGTH
	mov	ecx,SEGMENT_DATA_CHARS
	call	prepare_line
	pop	esi
	mov	esi,[esi]
	jmp	prepare_next_line
    prepare_long_line:
	push	edi
	mov	edi,[line_buffer]
	xor	eax,eax
    combine_long_line_segments:
	add	esi,SEGMENT_HEADER_LENGTH
	mov	ecx,SEGMENT_DATA_CHARS
	rep	movsd
	add	eax,SEGMENT_DATA_CHARS
	mov	esi,[esi-SEGMENT_LENGTH]
	btr	esi,0
	jc	combine_long_line_segments
	mov	ecx,eax
	mov	eax,esi
	mov	esi,[line_buffer]
	pop	edi
	push	eax
	call	prepare_line
	pop	esi
    prepare_next_line:
	inc	[line_number]
	pop	ecx
	dec	ecx
	jz	prepare_selection
	or	esi,esi
	jnz	prepare_screen
    prepare_empty_lines:
	imul	ecx,[screen_width]
	mov	edx,ecx
	xor	eax,eax
	rep	stosd
	xchg	edi,ebx
	mov	ecx,edx
	rep	stosb
	xchg	edi,ebx
    prepare_selection:
	pop	ebx
	cmp	[selection_line],0
	je	screen_prepared
	mov	eax,[window_line_number]
	mov	esi,[selection_line_number]
	mov	edi,[caret_line_number]
	sub	esi,eax
	sub	edi,eax
	mov	ecx,[window_position]
	mov	eax,[selection_position]
	mov	edx,[caret_position]
	sub	eax,ecx
	sub	edx,ecx
	cmp	esi,edi
	jle	selection_boundaries_ok
	xchg	esi,edi
	xchg	eax,edx
    selection_boundaries_ok:
	mov	ecx,[screen_height]
	cmp	edi,0
	jl	screen_prepared
	cmp	esi,ecx
	jge	screen_prepared
	cmp	esi,edi
	je	prepare_vsel
	test	dword [editor_mode],FEMODE_VERTICALSEL
	jz	prepare_hsel
    prepare_vsel:
	cmp	eax,edx
	jle	vselection_boundaries_ok
	xchg	eax,edx
    vselection_boundaries_ok:
	cmp	esi,0
	jge	vselection_start_ok
	xor	esi,esi
    vselection_start_ok:
	inc	edi
	cmp	edi,ecx
	jle	vselection_end_ok
	mov	edi,ecx
    vselection_end_ok:
	mov	ecx,[screen_width]
	cmp	edx,0
	jl	screen_prepared
	cmp	eax,ecx
	jge	screen_prepared
	cmp	eax,0
	jge	vselection_line_start_ok
	xor	eax,eax
    vselection_line_start_ok:
	cmp	edx,ecx
	jle	vselection_line_end_ok
	mov	edx,ecx
    vselection_line_end_ok:
	mov	ecx,edi
	sub	ecx,esi
	imul	esi,[screen_width]
	add	ebx,esi
    prepare_vselection_line:
	push	eax ecx
	mov	ecx,edx
	sub	ecx,eax
	lea	edi,[ebx+eax]
	mov	al,80h
	rep	stosb
	add	ebx,[screen_width]
	pop	ecx eax
	loop	prepare_vselection_line
	jmp	screen_prepared
    prepare_hsel:
	cmp	esi,0
	jge	hselection_start_ok
	xor	esi,esi
	xor	eax,eax
    hselection_start_ok:
	cmp	edi,ecx
	jl	hselection_end_ok
	mov	edi,ecx
	xor	edx,edx
    hselection_end_ok:
	inc	esi
	mov	ecx,edi
	sub	ecx,esi
	imul	ecx,[screen_width]
	imul	esi,[screen_width]
	lea	edi,[ebx+esi]
	neg	eax
	add	eax,[screen_width]
	cmp	eax,0
	jle	hselection_start_line_ok
	sub	edi,eax
	add	ecx,eax
	sub	eax,[screen_width]
	jle	hselection_start_line_ok
	add	edi,eax
	sub	ecx,eax
    hselection_start_line_ok:
	cmp	edx,0
	jle	hselection_end_line_ok
	add	ecx,edx
	sub	edx,[screen_width]
	jle	hselection_end_line_ok
	sub	ecx,edx
    hselection_end_line_ok:
	mov	al,80h
	rep	stosb
    screen_prepared:
	retn
    prepare_line:
	push	edi ecx
	mov	edi,[line_buffer]
	lea	edi,[edi+ecx*4]
	push	edi
	xor	eax,eax
	rep	stosb
	pop	edi
	mov	ecx,[esp]
	call	[syntax_proc]
	mov	edx,edi
	pop	eax edi
	push	esi edx
	mov	edx,[window_position]
	lea	esi,[esi+edx*4]
	sub	eax,edx
	jnc	line_position_ok
	xor	eax,eax
	jmp	line_length_ok
    line_position_ok:
	cmp	eax,[screen_width]
	jbe	line_length_ok
	mov	eax,[screen_width]
    line_length_ok:
	mov	ecx,eax
	rep	movsd
	pop	esi
	add	esi,edx
	mov	ecx,eax
	xchg	edi,ebx
	rep	movsb
	xchg	edi,ebx
	pop	esi
	cmp	eax,[screen_width]
	jb	after_end_of_line
	retn
    after_end_of_line:
	mov	ecx,[screen_width]
	sub	ecx,eax
	mov	edx,ecx
	xor	eax,eax
	rep	stosd
	xchg	edi,ebx
	mov	ecx,edx
	rep	stosb
	xchg	edi,ebx
	retn

; paint_changes: paints the cells that differ between two copies of screen

paint_changes:
	mov	eax,[screen_size]
	test	eax,eax
	jz	changes_painted
	mov	ebx,[screen_base]
	mov	[paint_cells_a],ebx
	lea	ecx,[ebx+eax*4]
	mov	[paint_attributes_a],ecx
	lea	ecx,[eax*5]
	add	ebx,ecx
	mov	[paint_cells_b],ebx
	lea	ecx,[ebx+eax*4]
	mov	[paint_attributes_b],ecx
	xor	ecx,ecx
	xor	esi,esi
    compare_screen_line:
	xor	edi,edi
    compare_screen_cell:
	cmp	edi,[screen_width]
	jae	next_compared_line
	call	compare_cell
	jne	changed_cells_found
	inc	esi
	inc	edi
	jmp	compare_screen_cell
    changed_cells_found:
	mov	ebx,edi
    find_end_of_changes:
	inc	esi
	inc	edi
	cmp	edi,[screen_width]
	jae	paint_changed_cells
	call	compare_cell
	jne	find_end_of_changes
    paint_changed_cells:
	push	esi edi ecx
	mov	eax,ecx
	mov	edx,edi
	sub	edx,ebx
	mov	ecx,ebx
	call	paint_cells
	pop	ecx edi esi
	jmp	compare_screen_cell
    next_compared_line:
	inc	ecx
	cmp	ecx,[screen_height]
	jb	compare_screen_line
    changes_painted:
	retn
    compare_cell:
	mov	eax,[paint_cells_a]
	mov	edx,[paint_cells_b]
	mov	eax,[eax+esi*4]
	cmp	eax,[edx+esi*4]
	jne	cell_compared
	mov	eax,[paint_attributes_a]
	mov	edx,[paint_attributes_b]
	mov	al,[eax+esi]
	cmp	al,[edx+esi]
    cell_compared:
	retn

; paint_area: eax = left, ebx = top, ecx = right, edx = bottom (pixels)

paint_area:
	cmp	eax,0
	jge	area_left_ok
	xor	eax,eax
    area_left_ok:
	cmp	ebx,0
	jge	area_top_ok
	xor	ebx,ebx
    area_top_ok:
	cmp	ecx,eax
	jl	area_painted
	cmp	edx,ebx
	jl	area_painted
	push	edx ecx ebx
	xor	edx,edx
	div	[font_width]
	mov	[paint_first_column],eax
	pop	eax
	xor	edx,edx
	div	[font_height]
	mov	[paint_first_row],eax
	pop	eax
	xor	edx,edx
	div	[font_width]
	cmp	eax,[screen_width]
	jb	area_right_ok
	mov	eax,[screen_width]
	dec	eax
    area_right_ok:
	mov	[paint_last_column],eax
	pop	eax
	xor	edx,edx
	div	[font_height]
	cmp	eax,[screen_height]
	jb	area_bottom_ok
	mov	eax,[screen_height]
	dec	eax
    area_bottom_ok:
	mov	[paint_last_row],eax
	mov	eax,[paint_first_row]
    paint_area_row:
	cmp	eax,[paint_last_row]
	jg	area_painted
	mov	ecx,[paint_first_column]
	mov	edx,[paint_last_column]
	sub	edx,ecx
	jl	area_painted
	inc	edx
	push	eax
	call	paint_cells
	pop	eax
	inc	eax
	jmp	paint_area_row
    area_painted:
	retn

; paint_cells: eax = row, ecx = first column, edx = number of cells
; paints cells from the current copy of screen

paint_cells:
	push	ebx esi edi ebp
	test	edx,edx
	jz	cells_painted
	mov	ebp,edx
	mov	ebx,eax
	imul	eax,[font_height]
	mov	[paint_y],eax
	mov	eax,ecx
	imul	eax,[font_width]
	mov	[paint_x],eax
	mov	eax,ebx
	imul	eax,[screen_width]
	add	eax,ecx
	mov	esi,[screen_base]
	add	esi,[screen_offset]
	mov	edi,[screen_size]
	lea	edi,[esi+edi*4]
	add	edi,eax
	lea	esi,[esi+eax*4]
    paint_next_run:
	test	ebp,ebp
	jz	cells_painted
	mov	bl,[edi]
	cmp	dword [esi],0
	je	paint_empty_run
	mov	ecx,1
    measure_text_run:
	cmp	ecx,ebp
	jae	text_run_measured
	cmp	ecx,255
	jae	text_run_measured
	cmp	dword [esi+ecx*4],0
	je	text_run_measured
	cmp	bl,[edi+ecx]
	jne	text_run_measured
	inc	ecx
	jmp	measure_text_run
    text_run_measured:
	push	ecx
	mov	eax,[editor_font]
	mov	eax,[eax+FONT_ID]
	cmp	eax,[editor_gc_font]
	je	editor_gc_font_ok
	mov	[editor_gc_font],eax
	mov	ecx,eax
	mov	eax,[editor_gc]
	call	x11_set_gc_font
    editor_gc_font_ok:
	call	set_run_colors
	mov	ecx,[esp]
	push	edi
	mov	edi,char2b_buffer
    convert_run_characters:
	lodsd
	call	cell_to_glyph
	xchg	al,ah
	stosw
	loop	convert_run_characters
	pop	edi
	mov	eax,[editor_window]
	mov	ebx,[editor_gc]
	mov	ecx,[paint_x]
	mov	edx,[paint_y]
	add	edx,[font_ascent]
	push	esi edi
	mov	esi,char2b_buffer
	mov	edi,[esp+8]
	call	x11_image_text16
	pop	edi esi
	pop	ecx
	jmp	run_painted
    paint_empty_run:
	mov	ecx,1
    measure_empty_run:
	cmp	ecx,ebp
	jae	empty_run_measured
	cmp	dword [esi+ecx*4],0
	jne	empty_run_measured
	mov	al,[edi+ecx]
	xor	al,bl
	test	al,80h
	jnz	empty_run_measured
	inc	ecx
	jmp	measure_empty_run
    empty_run_measured:
	push	ecx
	mov	eax,[background_color]
	test	bl,80h
	jz	empty_run_color_ok
	mov	eax,[selection_background]
    empty_run_color_ok:
	call	x11_rgb_to_pixel
	mov	ecx,eax
	mov	edx,eax
	call	set_gc_colors
	mov	ecx,[esp]
	lea	esi,[esi+ecx*4]
	imul	ecx,[font_width]
	push	esi edi
	mov	esi,ecx
	mov	edi,[font_height]
	mov	eax,[editor_window]
	mov	ebx,[editor_gc]
	mov	ecx,[paint_x]
	mov	edx,[paint_y]
	call	x11_fill_rectangle
	pop	edi esi
	pop	ecx
    run_painted:
	add	edi,ecx
	sub	ebp,ecx
	mov	eax,ecx
	imul	eax,[font_width]
	add	[paint_x],eax
	jmp	paint_next_run
    cells_painted:
	pop	ebp edi esi ebx
	retn
    set_run_colors:
	test	bl,80h
	jnz	selection_colors
	mov	eax,[background_color]
	call	x11_rgb_to_pixel
	mov	edx,eax
	mov	eax,[text_color]
	movzx	ecx,bl
	jecxz	run_color_ok
	mov	eax,[syntax_colors]
	test	eax,eax
	jz	default_run_color
	mov	eax,[eax+(ecx-1)*4]
	jmp	run_color_ok
    default_run_color:
	mov	eax,[text_color]
    run_color_ok:
	call	x11_rgb_to_pixel
	mov	ecx,eax
	jmp	set_gc_colors
    selection_colors:
	mov	eax,[selection_background]
	call	x11_rgb_to_pixel
	mov	edx,eax
	mov	eax,[selection_text]
	call	x11_rgb_to_pixel
	mov	ecx,eax
    set_gc_colors:
	cmp	ecx,[gc_foreground]
	jne	change_gc_colors
	cmp	edx,[gc_background]
	je	gc_colors_ok
    change_gc_colors:
	mov	[gc_foreground],ecx
	mov	[gc_background],edx
	mov	eax,[editor_gc]
	call	x11_set_gc_colors
    gc_colors_ok:
	retn

; cell_to_glyph: eax = cell, returns eax = character of the font

cell_to_glyph:
	cmp	eax,20h
	jb	control_character_glyph
	cmp	eax,7Fh
	je	delete_character_glyph
	cmp	eax,ESCAPED_BYTE
	jb	glyph_in_range
	cmp	eax,ESCAPED_BYTE+100h
	jb	replacement_glyph
	cmp	eax,10000h
	jae	replacement_glyph
    glyph_in_range:
	push	edx
	mov	edx,[editor_font]
	cmp	byte [edx+FONT_16BIT],0
	pop	edx
	jne	glyph_ok
	cmp	eax,100h
	jb	glyph_ok
	mov	eax,'?'
    glyph_ok:
	retn
    control_character_glyph:
	add	eax,2400h
	jmp	glyph_in_range
    delete_character_glyph:
	mov	eax,2421h
	jmp	glyph_in_range
    replacement_glyph:
	mov	eax,0FFFDh
	jmp	glyph_in_range

include 'ide/memory.inc'
include 'ide/navigate.inc'
include 'ide/edit.inc'
include 'ide/blocks.inc'
include 'ide/search.inc'
include 'ide/undo.inc'

; destroy_editor_scrollbar: ebx = scroll bar control (may be 0)

destroy_editor_scrollbar:
	test	ebx,ebx
	jz	editor_scrollbar_destroyed
	call	destroy_control
	mov	eax,[ebx+C_WINDOW]
	call	x11_destroy_window
	cmp	dword [ebx+C_DIALOG],0
	jne	editor_scrollbar_destroyed
	call	release_memory
    editor_scrollbar_destroyed:
	retn

; editor_scrolled: ebx = scroll bar of editor, eax = scroll code

editor_scrolled:
	mov	edx,[ebx+C_NOTIFY_DATA]
	mov	[fedit_handle],edx
	mov	[fe_wparam],eax
	mov	eax,[scroll_track_position]
	mov	[fe_lparam],eax
	mov	eax,FM_HSCROLL
	cmp	dword [ebx+S_VERTICAL],0
	je	editor_scroll_message_ok
	mov	eax,FM_VSCROLL
    editor_scroll_message_ok:
	mov	[fe_message],eax
	jmp	fedit_proc

; editor_expose: edi = window entry, esi = event

editor_expose:
	mov	eax,[edi+8]
	mov	[fedit_handle],eax
	mov	eax,[esi+8]
	mov	[fe_wparam],eax
	mov	eax,[esi+12]
	mov	[fe_lparam],eax
	mov	[fe_message],FM_PAINT
	jmp	fedit_proc

; editor_pointer_event: edi = window entry

editor_pointer_event:
	mov	eax,[edi+8]
	mov	[pointer_editor],eax
	test	dword [eax+editor_style-editor_data],FES_DISABLED
	jnz	editor_pointer_done
	mov	ecx,[cm_state]
	mov	[fe_state],ecx
	mov	ecx,[cm_x]
	mov	[fe_wparam],ecx
	mov	ecx,[cm_y]
	mov	[fe_lparam],ecx
	mov	al,[cm_event]
	cmp	al,X11_MotionNotify
	je	editor_pointer_motion
	cmp	al,X11_ButtonRelease
	je	editor_pointer_release
	mov	eax,[cm_button]
	cmp	eax,1
	je	editor_left_press
	cmp	eax,3
	je	editor_right_press
	mov	[fe_message],FM_MOUSEWHEEL
	mov	[fe_wparam],120
	cmp	eax,4
	je	editor_pointer_message
	mov	[fe_wparam],-120
	cmp	eax,5
	je	editor_pointer_message
	mov	[fe_message],FM_MOUSEHWHEEL
	mov	[fe_wparam],-1
	cmp	eax,6
	je	editor_pointer_message
	mov	[fe_wparam],1
	cmp	eax,7
	je	editor_pointer_message
    editor_pointer_done:
	retn
    editor_right_press:
	mov	eax,[pointer_editor]
	test	dword [eax+editor_style-editor_data],FES_MAIN
	jz	editor_pointer_done
	jmp	editor_context_menu
    editor_left_press:
	mov	eax,[pointer_editor]
	mov	ebp,[eax+editor_owner-editor_data]
	test	ebp,ebp
	jz	editor_click_focus_done
	xor	ecx,ecx
    find_editor_control:
	cmp	ecx,[ebp+D_COUNT]
	jae	editor_click_focus_done
	mov	ebx,[ebp+D_CONTROLS+ecx*4]
	cmp	dword [ebx+C_TYPE],CT_FEDIT
	jne	next_editor_control
	cmp	[ebx+F_EDITOR],eax
	je	editor_control_found
    next_editor_control:
	inc	ecx
	jmp	find_editor_control
    editor_control_found:
	mov	[keyboard_focus_change],0
	call	set_dialog_focus
    editor_click_focus_done:
	mov	[fe_message],FM_LBUTTONDOWN
	mov	eax,[last_event_time]
	mov	edx,eax
	xchg	eax,[last_click_time]
	sub	edx,eax
	cmp	edx,DOUBLE_CLICK_TIME
	ja	editor_single_click
	mov	eax,[pointer_editor]
	cmp	eax,[last_click_control]
	jne	editor_single_click
	mov	eax,[fe_wparam]
	sub	eax,[last_click_x]
	cdq
	xor	eax,edx
	sub	eax,edx
	cmp	eax,DOUBLE_CLICK_DISTANCE
	ja	editor_single_click
	mov	eax,[fe_lparam]
	sub	eax,[last_click_y]
	cdq
	xor	eax,edx
	sub	eax,edx
	cmp	eax,DOUBLE_CLICK_DISTANCE
	ja	editor_single_click
	mov	[last_click_control],0
	mov	[fe_message],FM_LBUTTONDBLCLK
	jmp	editor_pointer_message
    editor_single_click:
	mov	eax,[pointer_editor]
	mov	[last_click_control],eax
	mov	eax,[fe_wparam]
	mov	[last_click_x],eax
	mov	eax,[fe_lparam]
	mov	[last_click_y],eax
	jmp	editor_pointer_message
    editor_pointer_motion:
	test	[cm_state],X11_Button1Mask
	jz	editor_pointer_done
	mov	[fe_message],FM_MOUSEMOVE
	jmp	editor_pointer_message
    editor_pointer_release:
	cmp	[cm_button],1
	jne	editor_pointer_done
	mov	[fe_message],FM_LBUTTONUP
    editor_pointer_message:
	mov	eax,[pointer_editor]
	mov	[fedit_handle],eax
	call	fedit_proc
	jmp	reset_blink

; send_editor: eax = editor handle, edx = message, returns eax = result

send_editor:
	mov	[fedit_handle],eax
	mov	[fe_message],edx
	jmp	fedit_proc
