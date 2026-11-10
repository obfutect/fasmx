
; fasmx - flat assembler IDE for Linux/X11
; user interface toolkit: windows registry, drawing, controls, dialogs

; kinds of registered windows

WK_MAIN       = 1
WK_EDITOR     = 2
WK_CONTROL    = 3
WK_DIALOG     = 4
WK_MENU       = 5

WINDOW_TABLE_LENGTH = 1024

; control structure

C_TYPE	      = 0
C_WINDOW      = 4
C_DIALOG      = 8
C_ID	      = 12
C_X	      = 16
C_Y	      = 20
C_WIDTH       = 24
C_HEIGHT      = 28
C_STYLE       = 32
C_STATE       = 36
C_TEXT	      = 40
C_PROC	      = 44
C_NOTIFY      = 48
C_NOTIFY_DATA = 52
C_DATA	      = 56
CONTROL_SIZE  = 128

; edit control (also the edit part of combo box)
E_CELLS       = 56
E_LENGTH      = 60
E_CAPACITY    = 64
E_CARET       = 68
E_ANCHOR      = 72
E_SCROLL      = 76
E_TEXT_X      = 80
E_TEXT_WIDTH  = 84
E_MEMORY      = 88
E_DRAG	      = 92
; combo box
CB_LIST       = 96
CB_DROPPED    = 100
CB_BUTTON_X   = 104
; list box
L_STRINGS     = 56
L_STRINGS_SIZE = 60
L_STRINGS_USED = 64
L_ITEMS       = 68
L_ITEMS_SIZE  = 72
L_COUNT       = 76
L_SELECTION   = 80
L_TOP	      = 84
L_SCROLLBAR   = 88
L_ANCHOR      = 92
L_COMBO       = 96
L_ITEM_HEIGHT = 100
L_VISIBLE     = 104
; scroll bar
S_MIN	      = 56
S_MAX	      = 60
S_PAGE	      = 64
S_POS	      = 68
S_TRACK       = 72
S_DRAG	      = 76
S_DRAG_OFFSET = 80
S_REPEAT_CODE = 84
S_VERTICAL    = 88
; progress bar
P_POS	      = 56
P_MAX	      = 60
; editor in dialog
F_EDITOR      = 56
; icon
I_KIND	      = 56

; control types

CT_STATIC    = 1
CT_BUTTON    = 2
CT_CHECKBOX  = 3
CT_RADIO     = 4
CT_EDIT      = 5
CT_LISTBOX   = 6
CT_COMBOBOX  = 7
CT_PROGRESS  = 8
CT_ETCHED    = 9
CT_ICON      = 10
CT_SCROLLBAR = 11
CT_FEDIT     = 12
CT_CUSTOM    = 13
CT_FONTPREVIEW = 14
CT_COLORGRID = 15
CT_COLORSAMPLE = 16

; control styles (names follow Win32 styles used by FASMW templates)

WS_TABSTOP	  = 1
WS_DISABLED	  = 2
BS_DEFPUSHBUTTON  = 4
BS_LEFTTEXT	  = 8
SS_RIGHT	  = 10h
SS_CENTER	  = 20h
ES_NUMBER	  = 40h
ES_READONLY	  = 80h
ES_MULTILINE	  = 100h
CBS_DROPDOWN	  = 200h
CBS_DROPDOWNLIST  = 400h
LBS_EXTENDEDSEL   = 800h
BS_AUTOCHECKBOX   = 1000h
BS_AUTORADIOBUTTON = 2000h
SS_ETCHEDHORZ	  = 4000h
SS_ICON 	  = 8000h
FES_SYNTAX	  = 10000h
SS_SUNKEN	  = 20000h
WS_BORDER	  = 40000h
CS_COMBO_EDIT	  = 80000h
WS_VISIBLE	  = 0
WS_VSCROLL	  = 0
WS_CAPTION	  = 0
WS_POPUP	  = 0
WS_SYSMENU	  = 0
DS_MODALFRAME	  = 0
BS_PUSHBUTTON	  = 0
BS_RIGHT	  = 0
SS_LEFT 	  = 0
SS_LEFTNOWORDWRAP = 0
ES_AUTOHSCROLL	  = 0
ES_AUTOVSCROLL	  = 0
CBS_AUTOHSCROLL   = 0
LBS_NOTIFY	  = 0
ES_NOHIDESEL	  = 0

; control states

CS_CHECKED  = 1
CS_PRESSED  = 2
CS_FOCUSED  = 4
CS_DISABLED = 8
CS_HOT	    = 10h

; control messages (eax), parameters in cm_x, cm_y, cm_button, cm_keysym,
; cm_character, cm_state

CM_PAINT	= 1
CM_PRESS	= 2
CM_RELEASE	= 3
CM_MOTION	= 4
CM_DOUBLECLICK	= 5
CM_KEY		= 6
CM_FOCUS	= 7
CM_UNFOCUS	= 8
CM_TIMER	= 9
CM_DESTROY	= 10
CM_BLINK	= 11
CM_WHEEL	= 12

; notification codes for dialog procedures

BN_CLICKED     = 0
EN_CHANGE      = 300h
CBN_SELCHANGE  = 1
CBN_EDITCHANGE = 5
LBN_SELCHANGE  = 1
LBN_DBLCLK     = 2

; dialog messages

DM_INIT    = 1
DM_COMMAND = 2
DM_CLOSE   = 3

IDOK	 = 1
IDCANCEL = 2
IDYES	 = 6
IDNO	 = 7

; message box flags

MB_OK		  = 0
MB_OKCANCEL	  = 1
MB_YESNOCANCEL	  = 3
MB_YESNO	  = 4
MB_ICONINFORMATION = 10h
MB_ICONQUESTION   = 20h
MB_ICONERROR	  = 30h
MB_ICONWARNING	  = 40h
MB_DEFBUTTON2	  = 100h

ICON_INFORMATION = 1
ICON_QUESTION	 = 2
ICON_ERROR	 = 3
ICON_WARNING	 = 4

; scroll codes

SB_LINEUP	= 0
SB_LINEDOWN	= 1
SB_PAGEUP	= 2
SB_PAGEDOWN	= 3
SB_THUMBTRACK	= 5

SCROLLBAR_SIZE = 16

; dialog structure

D_WINDOW      = 0
D_PROC	      = 4
D_RESULT      = 8
D_ENDED       = 12
D_FOCUS       = 16
D_COUNT       = 20
D_PARAM       = 24
D_WIDTH       = 28
D_HEIGHT      = 32
D_PARENT      = 36
D_ARENA       = 40
D_ARENA_END   = 44
D_MAPPED      = 48
D_HANDLE      = 52
D_FOCUSED     = 56
D_TEMPLATE    = 60
D_DATA	      = 64
D_CONTROLS    = 128
DIALOG_MAX_CONTROLS = 64
DIALOG_MEMORY = 40000h

DT_PIXELS = 1

; dialog templates

macro dialog name,title,x,y,cx,cy,style
{ name dd name#.title,cx,cy,0
  name#.title db title,0
  align 4 }

macro dialogitem class,title,id,x,y,cx,cy,style
{ local next,type,text_ptr,istyle
  istyle = style
  if class eq 'STATIC'
   if istyle and SS_ETCHEDHORZ
    type = CT_ETCHED
   else if istyle and SS_ICON
    type = CT_ICON
   else
    type = CT_STATIC
   end if
  else if class eq 'BUTTON'
   if istyle and BS_AUTOCHECKBOX
    type = CT_CHECKBOX
   else if istyle and BS_AUTORADIOBUTTON
    type = CT_RADIO
   else
    type = CT_BUTTON
   end if
  else if class eq 'EDIT'
   if istyle and ES_MULTILINE
    type = CT_FEDIT
   else
    type = CT_EDIT
   end if
  else if class eq 'COMBOBOX'
   type = CT_COMBOBOX
  else if class eq 'LISTBOX'
   type = CT_LISTBOX
  else if class eq 'PROGRESS'
   type = CT_PROGRESS
  else if class eq 'FEDIT'
   type = CT_FEDIT
  else if class eq 'FONTPREVIEW'
   type = CT_FONTPREVIEW
  else if class eq 'COLORGRID'
   type = CT_COLORGRID
  else if class eq 'COLORSAMPLE'
   type = CT_COLORSAMPLE
  else
   type = CT_CUSTOM
  end if
  dd next,type,id,x,y,cx,cy,istyle
  db title,0
  align 4
  next: }

macro enddialog
{ dd 0 }

; menus

MF_SEPARATOR = 1
MF_POPUP     = 2

MIS_GRAYED  = 1
MIS_CHECKED = 2

; item texts are collected and emitted by menu_texts after all menus,
; so that the item records stay MENU_ITEM_SIZE apart

macro menu_texts {}

macro menuitem text,accel,id
{ local label
  dd id,label,0,0
  macro menu_texts
  \{ menu_texts
     if ~ accel eq
      label db text,9,accel,0
     else
      label db text,0
     end if \} }

macro menuseparator
{ dd 0,0,MF_SEPARATOR,0 }

macro endmenu
{ dd -1,0,0,0 }

MENU_ITEM_SIZE = 16

; ui_init: opens the font of user interface and prepares colors

ui_init:
	mov	esi,_ui_font
	call	x11_open_font
	jnc	ui_font_opened
	mov	eax,[editor_font_record+FONT_ID]
	mov	[ui_font],eax
	mov	eax,[editor_font_record+FONT_WIDTH]
	mov	[ui_font_width],eax
	mov	eax,[editor_font_record+FONT_HEIGHT]
	mov	[ui_font_height],eax
	mov	eax,[editor_font_record+FONT_ASCENT]
	mov	[ui_font_ascent],eax
	mov	al,byte [editor_font_record+FONT_16BIT]
	mov	[ui_font_16bit],al
	jmp	ui_font_ready
    ui_font_opened:
	mov	[ui_font],eax
	movsx	eax,word [x11_packet+28]
	mov	[ui_font_width],eax
	movsx	eax,word [x11_packet+52]
	mov	[ui_font_ascent],eax
	movsx	edx,word [x11_packet+54]
	add	eax,edx
	mov	[ui_font_height],eax
	mov	al,[x11_packet+50]
	mov	[ui_font_16bit],al
    ui_font_ready:
	mov	esi,ui_colors
	mov	edi,ui_pixels
	mov	ecx,UI_COLORS_COUNT
    convert_ui_colors:
	lodsd
	call	x11_rgb_to_pixel
	stosd
	loop	convert_ui_colors
	mov	eax,[ui_font]
	mov	[gc_values],eax
	mov	dword [gc_values+4],0
	mov	eax,[main_window]
	mov	ecx,4000h+10000h
	mov	esi,gc_values
	call	x11_create_gc
	mov	[ui_gc],eax
	mov	[ui_gc_foreground],-1
	mov	[ui_gc_background],-1
	retn

; register_window: eax = window, ecx = kind, edx = object, esi = owner dialog

register_window:
	push	edi
	mov	edi,window_table
	push	ecx
	mov	ecx,WINDOW_TABLE_LENGTH
    find_free_window_entry:
	cmp	dword [edi],0
	je	free_window_entry
	add	edi,16
	loop	find_free_window_entry
	pop	ecx
	pop	edi
	retn
    free_window_entry:
	pop	ecx
	mov	[edi],eax
	mov	[edi+4],ecx
	mov	[edi+8],edx
	mov	[edi+12],esi
	pop	edi
	retn

; unregister_window: eax = window

unregister_window:
	push	edi
	call	find_window
	jc	window_unregistered
	mov	dword [edi],0
    window_unregistered:
	pop	edi
	retn

; find_window: eax = window, returns CF=0 and edi = entry

find_window:
	test	eax,eax
	jz	window_not_found
	push	ecx
	mov	edi,window_table
	mov	ecx,WINDOW_TABLE_LENGTH
    search_window_table:
	cmp	[edi],eax
	je	window_found
	add	edi,16
	loop	search_window_table
	pop	ecx
    window_not_found:
	stc
	retn
    window_found:
	pop	ecx
	clc
	retn

; ui_set_colors: ecx = foreground pixel, edx = background pixel

ui_set_colors:
	cmp	ecx,[ui_gc_foreground]
	jne	change_ui_colors
	cmp	edx,[ui_gc_background]
	je	ui_colors_set
    change_ui_colors:
	mov	[ui_gc_foreground],ecx
	mov	[ui_gc_background],edx
	push	eax
	mov	eax,[ui_gc]
	call	x11_set_gc_colors
	pop	eax
    ui_colors_set:
	retn

; ui_fill: eax = window, ebx = pixel, ecx = x, edx = y, esi = width,
; edi = height

ui_fill:
	push	ebx ecx edx
	push	ecx edx
	mov	ecx,ebx
	mov	edx,ebx
	call	ui_set_colors
	pop	edx ecx
	mov	ebx,[ui_gc]
	call	x11_fill_rectangle
	pop	edx ecx ebx
	retn

; ui_line: eax = window, ebx = pixel, ecx = x1, edx = y1, esi = x2, edi = y2

ui_line:
	push	ebx ecx edx
	push	ecx edx
	mov	ecx,ebx
	mov	edx,ebx
	call	ui_set_colors
	pop	edx ecx
	mov	ebx,[ui_gc]
	call	x11_draw_line
	pop	edx ecx ebx
	retn

; ui_frame: eax = window, ebx = frame style, ecx = x, edx = y, esi = width,
; edi = height

FRAME_RAISED   = 0
FRAME_SUNKEN   = 1
FRAME_PRESSED  = 2
FRAME_THIN_SUNKEN = 3
FRAME_THIN_RAISED = 4
FRAME_BLACK    = 5

ui_frame:
	push	ebx ecx edx esi edi
	mov	[frame_x],ecx
	mov	[frame_y],edx
	lea	ecx,[ecx+esi-1]
	mov	[frame_right],ecx
	lea	edx,[edx+edi-1]
	mov	[frame_bottom],edx
	cmp	ebx,FRAME_SUNKEN
	je	sunken_frame
	cmp	ebx,FRAME_PRESSED
	je	pressed_frame
	cmp	ebx,FRAME_THIN_SUNKEN
	je	thin_sunken_frame
	cmp	ebx,FRAME_THIN_RAISED
	je	thin_raised_frame
	cmp	ebx,FRAME_BLACK
	je	black_frame
	mov	ebx,[px_light]
	mov	ecx,[px_dark]
	call	frame_lines
	mov	ebx,[px_face]
	mov	ecx,[px_shadow]
	jmp	inner_frame_lines
    sunken_frame:
	mov	ebx,[px_shadow]
	mov	ecx,[px_light]
	call	frame_lines
	mov	ebx,[px_dark]
	mov	ecx,[px_face]
	jmp	inner_frame_lines
    pressed_frame:
	mov	ebx,[px_dark]
	mov	ecx,[px_dark]
	call	frame_lines
	mov	ebx,[px_shadow]
	mov	ecx,[px_face]
	jmp	inner_frame_lines
    thin_sunken_frame:
	mov	ebx,[px_shadow]
	mov	ecx,[px_light]
	call	frame_lines
	jmp	frame_done
    thin_raised_frame:
	mov	ebx,[px_light]
	mov	ecx,[px_shadow]
	call	frame_lines
	jmp	frame_done
    black_frame:
	mov	ebx,[px_dark]
	mov	ecx,[px_dark]
	call	frame_lines
	jmp	frame_done
    inner_frame_lines:
	inc	[frame_x]
	inc	[frame_y]
	dec	[frame_right]
	dec	[frame_bottom]
	call	frame_lines
    frame_done:
	pop	edi esi edx ecx ebx
	retn
    frame_lines:
	push	ecx
	mov	ecx,[frame_x]
	mov	edx,[frame_y]
	mov	esi,[frame_right]
	mov	edi,edx
	call	ui_line
	mov	esi,ecx
	mov	edi,[frame_bottom]
	call	ui_line
	pop	ebx
	mov	ecx,[frame_right]
	mov	edx,[frame_y]
	mov	esi,ecx
	mov	edi,[frame_bottom]
	call	ui_line
	mov	ecx,[frame_x]
	mov	edx,edi
	call	ui_line
	retn

; utf8_decode_character: esi = zero-terminated UTF-8 string
; returns eax = character (0 at end, esi then stays at the terminator),
; esi advanced

utf8_decode_character:
	movzx	eax,byte [esi]
	test	eax,eax
	jz	character_decoded
	inc	esi
	cmp	eax,0C0h
	jb	character_decoded
	push	ecx edx
	mov	ecx,1
	and	eax,1Fh
	cmp	byte [esi-1],0E0h
	jb	continuation_count_ok
	mov	ecx,2
	and	eax,0Fh
	cmp	byte [esi-1],0F0h
	jb	continuation_count_ok
	mov	ecx,3
	and	eax,7
    continuation_count_ok:
	movzx	edx,byte [esi]
	mov	dh,dl
	and	dh,0C0h
	cmp	dh,80h
	jne	continuation_missing
	and	edx,3Fh
	shl	eax,6
	or	eax,edx
	inc	esi
	loop	continuation_count_ok
    continuation_missing:
	pop	edx ecx
    character_decoded:
	retn

; ui_glyph: eax = character, returns eax = glyph of interface font

ui_glyph:
	cmp	eax,10000h
	jb	ui_glyph_range_ok
	mov	eax,0FFFDh
    ui_glyph_range_ok:
	cmp	[ui_font_16bit],0
	jne	ui_glyph_ok
	cmp	eax,100h
	jb	ui_glyph_ok
	mov	eax,'?'
    ui_glyph_ok:
	retn

; ui_draw_text: eax = window, ecx = x, edx = top y, esi = UTF-8 text,
; ebx = foreground pixel, [ui_text_background] = background pixel,
; [ui_text_mnemonics] nonzero to interpret & characters
; stops at end of string or line break; returns ecx = x after text,
; esi = after the drawn line

ui_draw_text:
	push	eax ebx edx edi
	mov	[text_window],eax
	mov	[text_x],ecx
	mov	[text_y],edx
	mov	[text_underline],-1
	mov	[text_foreground],ebx
	mov	edi,char2b_buffer
	xor	ebx,ebx
    collect_text_characters:
	call	utf8_decode_character
	test	eax,eax
	jz	text_collected
	cmp	eax,0Ah
	je	text_line_end
	cmp	eax,0Dh
	je	text_carriage_return
	cmp	eax,9
	jne	text_tab_ok
	mov	eax,20h
    text_tab_ok:
	cmp	[ui_text_mnemonics],0
	je	store_text_glyph
	cmp	eax,'&'
	jne	store_text_glyph
	cmp	byte [esi],'&'
	jne	text_mnemonic
	inc	esi
	jmp	store_text_glyph
    text_mnemonic:
	mov	[text_underline],ebx
	jmp	collect_text_characters
    store_text_glyph:
	call	ui_glyph
	xchg	al,ah
	stosw
	inc	ebx
	cmp	ebx,255
	jb	collect_text_characters
	call	flush_text_glyphs
	jmp	collect_text_characters
    text_carriage_return:
	cmp	byte [esi],0Ah
	jne	text_line_end
	inc	esi
	jmp	text_line_end
    text_collected:
    text_line_end:
	call	flush_text_glyphs
	mov	ecx,[text_x]
	pop	edi edx ebx eax
	retn
    flush_text_glyphs:
	test	ebx,ebx
	jz	text_glyphs_flushed
	push	esi
	mov	ecx,[text_foreground]
	mov	edx,[ui_text_background]
	call	ui_set_colors
	mov	eax,[text_window]
	mov	ecx,[text_x]
	mov	edx,[text_y]
	add	edx,[ui_font_ascent]
	mov	esi,char2b_buffer
	mov	edi,ebx
	push	ebx
	mov	ebx,[ui_gc]
	call	x11_image_text16
	pop	ebx
	mov	eax,[text_underline]
	cmp	eax,0
	jl	text_underline_done
	imul	eax,[ui_font_width]
	add	eax,[text_x]
	mov	ecx,eax
	mov	esi,eax
	add	esi,[ui_font_width]
	dec	esi
	mov	edx,[text_y]
	add	edx,[ui_font_ascent]
	inc	edx
	mov	edi,edx
	mov	eax,[text_window]
	push	ebx
	mov	ebx,[text_foreground]
	call	ui_line
	pop	ebx
	mov	[text_underline],-1
    text_underline_done:
	mov	eax,ebx
	imul	eax,[ui_font_width]
	add	[text_x],eax
	pop	esi
	xor	ebx,ebx
	mov	edi,char2b_buffer
    text_glyphs_flushed:
	retn

; ui_text_width: esi = UTF-8 text (one line), returns eax = width in pixels
; (& characters are not counted when [ui_text_mnemonics] is set)

ui_text_width:
	push	ecx esi
	xor	ecx,ecx
    measure_text:
	call	utf8_decode_character
	test	eax,eax
	jz	text_measured
	cmp	eax,0Ah
	je	text_measured
	cmp	eax,0Dh
	je	text_measured
	cmp	[ui_text_mnemonics],0
	je	count_text_character
	cmp	eax,'&'
	jne	count_text_character
	cmp	byte [esi],'&'
	jne	measure_text
	inc	esi
    count_text_character:
	inc	ecx
	jmp	measure_text
    text_measured:
	mov	eax,ecx
	imul	eax,[ui_font_width]
	pop	esi ecx
	retn

; text_mnemonic: esi = UTF-8 text, returns eax = lower case mnemonic (0 if none)

text_mnemonic_character:
	push	esi
    find_mnemonic:
	lodsb
	test	al,al
	jz	no_mnemonic
	cmp	al,'&'
	jne	find_mnemonic
	lodsb
	cmp	al,'&'
	je	find_mnemonic
	movzx	eax,al
	cmp	eax,'A'
	jb	mnemonic_found
	cmp	eax,'Z'
	ja	mnemonic_found
	add	eax,20h
    mnemonic_found:
	pop	esi
	retn
    no_mnemonic:
	xor	eax,eax
	pop	esi
	retn

; ui_alloc: ecx = size, returns eax = zeroed memory (from the arena of
; dialog being built, or a separate allocation)

ui_alloc:
	push	ebx edx
	add	ecx,3
	and	ecx,not 3
	mov	edx,[building_dialog]
	test	edx,edx
	jz	separate_allocation
	mov	eax,[edx+D_ARENA]
	add	ecx,eax
	cmp	ecx,[edx+D_ARENA_END]
	ja	separate_allocation_size
	mov	[edx+D_ARENA],ecx
	pop	edx ebx
	retn
    separate_allocation_size:
	sub	ecx,eax
    separate_allocation:
	call	get_memory
	pop	edx ebx
	retn

; ui_store_text: esi = UTF-8 text, returns eax = copy allocated by ui_alloc

ui_store_text:
	push	ecx esi edi
	call	string_length
	inc	ecx
	push	ecx
	call	ui_alloc
	pop	ecx
	test	eax,eax
	jz	ui_text_not_stored
	mov	edi,eax
	rep	movsb
    ui_text_not_stored:
	pop	edi esi ecx
	retn

; create_control: parameters in new_control_* variables
; returns ebx = control (0 on failure)

create_control:
	push	esi edi ebp
	mov	ecx,CONTROL_SIZE
	call	ui_alloc
	test	eax,eax
	jz	control_not_created
	mov	ebx,eax
	mov	eax,[new_control_type]
	mov	[ebx+C_TYPE],eax
	mov	eax,[new_control_id]
	mov	[ebx+C_ID],eax
	mov	eax,[new_control_x]
	mov	[ebx+C_X],eax
	mov	eax,[new_control_y]
	mov	[ebx+C_Y],eax
	mov	eax,[new_control_width]
	mov	[ebx+C_WIDTH],eax
	mov	eax,[new_control_height]
	mov	[ebx+C_HEIGHT],eax
	mov	eax,[new_control_style]
	mov	[ebx+C_STYLE],eax
	test	eax,WS_DISABLED
	jz	control_state_ok
	or	dword [ebx+C_STATE],CS_DISABLED
    control_state_ok:
	mov	eax,[building_dialog]
	mov	[ebx+C_DIALOG],eax
	mov	eax,[new_control_notify]
	mov	[ebx+C_NOTIFY],eax
	mov	eax,[new_control_notify_data]
	mov	[ebx+C_NOTIFY_DATA],eax
	mov	esi,[new_control_text]
	test	esi,esi
	jnz	control_text_given
	mov	esi,_empty_string
    control_text_given:
	call	ui_store_text
	test	eax,eax
	jz	control_text_allocation_failed
	mov	[ebx+C_TEXT],eax
	mov	eax,[ebx+C_TYPE]
	mov	edx,[control_procedures+(eax-1)*4]
	cmp	[new_control_procedure],0
	je	control_procedure_ok
	mov	edx,[new_control_procedure]
	mov	[new_control_procedure],0
    control_procedure_ok:
	mov	[ebx+C_PROC],edx
	mov	eax,[px_face]
	mov	[window_values],eax
	mov	eax,X11_ExposureMask+X11_ButtonPressMask+X11_ButtonReleaseMask+X11_ButtonMotionMask
	cmp	dword [ebx+C_TYPE],CT_CUSTOM
	jne	control_event_mask_ok
	or	eax,X11_PointerMotionMask
    control_event_mask_ok:
	mov	[window_values+4],eax
	mov	[window_value_mask],2+800h
	cmp	dword [ebx+C_TYPE],CT_EDIT
	je	control_with_text_cursor
	cmp	dword [ebx+C_TYPE],CT_COMBOBOX
	jne	control_attributes_ok
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	control_attributes_ok
    control_with_text_cursor:
	mov	eax,[text_cursor]
	test	eax,eax
	jz	control_attributes_ok
	mov	[window_values+8],eax
	or	[window_value_mask],4000h
    control_attributes_ok:
	mov	eax,[new_control_parent]
	mov	ecx,[ebx+C_X]
	mov	edx,[ebx+C_Y]
	mov	esi,[ebx+C_WIDTH]
	mov	edi,[ebx+C_HEIGHT]
	test	esi,esi
	jnz	control_width_ok
	inc	esi
    control_width_ok:
	test	edi,edi
	jnz	control_height_ok
	inc	edi
    control_height_ok:
	call	x11_create_window
	mov	[ebx+C_WINDOW],eax
	mov	ecx,WK_CONTROL
	mov	edx,ebx
	mov	esi,[building_dialog]
	call	register_window
	mov	eax,CM_CREATE
	call	control_message
	mov	eax,[ebx+C_WINDOW]
	call	x11_map_window
	pop	ebp edi esi
	retn
    control_text_allocation_failed:
	mov	edx,[building_dialog]
	test	edx,edx
	jz	free_unfinished_control
	lea	eax,[edx+1000h]
	cmp	ebx,eax
	jb	free_unfinished_control
	cmp	ebx,[edx+D_ARENA_END]
	jae	free_unfinished_control
	; The failed text allocation left this as the last arena allocation.
	mov	[edx+D_ARENA],ebx
	jmp	control_not_created
    free_unfinished_control:
	call	release_memory
    control_not_created:
	mov	[new_control_procedure],0
	xor	ebx,ebx
	pop	ebp edi esi
	retn

CM_CREATE = 13

; control_message: ebx = control, eax = message

control_message:
	push	ebx esi edi ebp
	call	dword [ebx+C_PROC]
	pop	ebp edi esi ebx
	retn

; control_redraw: ebx = control

control_redraw:
	push	eax
	mov	eax,CM_PAINT
	call	control_message
	pop	eax
	retn

; control_notify: ebx = control, edx = notification code
; sends command to dialog procedure or calls notification procedure

control_notify:
	push	eax ecx edx esi edi ebp
	cmp	dword [ebx+C_NOTIFY],0
	jne	notify_procedure
	mov	ebp,[ebx+C_DIALOG]
	test	ebp,ebp
	jz	control_notified
	mov	ecx,edx
	mov	edx,[ebx+C_ID]
	call	dialog_command
	jmp	control_notified
    notify_procedure:
	mov	eax,edx
	push	ebx
	call	dword [ebx+C_NOTIFY]
	pop	ebx
    control_notified:
	pop	ebp edi esi edx ecx eax
	retn

; dialog_command: ebp = dialog, edx = id, ecx = notification code

dialog_command:
	push	eax ebx ecx edx esi edi ebp
	mov	eax,[current_dialog]
	push	eax
	mov	[current_dialog],ebp
	mov	eax,DM_COMMAND
	call	dword [ebp+D_PROC]
	pop	eax
	mov	[current_dialog],eax
	pop	ebp edi esi edx ecx ebx eax
	retn

; destroy_control: ebx = control

destroy_control:
	mov	eax,CM_DESTROY
	call	control_message
	mov	eax,[ebx+C_WINDOW]
	call	unregister_window
	retn

; move_control: ebx = control, ecx = x, edx = y, esi = width, edi = height

move_control:
	mov	[ebx+C_X],ecx
	mov	[ebx+C_Y],edx
	mov	[ebx+C_WIDTH],esi
	mov	[ebx+C_HEIGHT],edi
	test	esi,esi
	jnz	moved_width_ok
	inc	esi
    moved_width_ok:
	test	edi,edi
	jnz	moved_height_ok
	inc	edi
    moved_height_ok:
	mov	eax,[ebx+C_WINDOW]
	call	x11_move_resize_window
	mov	eax,CM_MOVE
	call	control_message
	retn

CM_MOVE = 14

; event_loop: esi = pointer to flag, returns when the flag becomes nonzero

event_loop:
	push	esi
    event_loop_iteration:
	mov	esi,[esp]
	cmp	dword [esi],0
	jne	event_loop_done
	call	x11_flush
	call	get_next_event
	jc	event_loop_iteration
	call	dispatch_event
	jmp	event_loop_iteration
    event_loop_done:
	pop	esi
	retn

; get_next_event: returns CF=0 and esi = event, or CF=1 if no event was
; read (timers and other sources were processed)

get_next_event:
	call	x11_dequeue_event
	jnc	next_event_ready
	mov	eax,[x11_in_end]
	sub	eax,[x11_in_start]
	cmp	eax,32
	jae	read_next_event
	call	get_time
	mov	ebx,eax
	or	edx,-1
	cmp	[blink_active],0
	je	blink_timeout_ok
	mov	edx,[blink_time]
	sub	edx,ebx
	jns	blink_timeout_ok
	xor	edx,edx
    blink_timeout_ok:
	cmp	[repeat_control],0
	je	repeat_timeout_ok
	mov	ecx,[repeat_time]
	sub	ecx,ebx
	jns	repeat_timeout_positive
	xor	ecx,ecx
    repeat_timeout_positive:
	cmp	ecx,edx
	jae	repeat_timeout_ok
	mov	edx,ecx
    repeat_timeout_ok:
	mov	eax,[x11_socket]
	mov	[poll_descriptors],eax
	mov	dword [poll_descriptors+4],POLLIN
	mov	ecx,1
	mov	eax,[compile_pipe]
	test	eax,eax
	js	poll_descriptors_ready
	mov	[poll_descriptors+8],eax
	mov	dword [poll_descriptors+12],POLLIN
	inc	ecx
    poll_descriptors_ready:
	mov	eax,SYS_POLL
	mov	ebx,poll_descriptors
	int	80h
	test	eax,eax
	js	no_event_read
	jz	process_timers
	cmp	[compile_pipe],0
	jl	check_server_input
	test	word [poll_descriptors+14],0FFFFh
	jz	check_server_input
	call	compile_pipe_input
    check_server_input:
	test	word [poll_descriptors+6],0FFFFh
	jz	process_timers
	call	x11_fill
    process_timers:
	call	get_time
	mov	ebx,eax
	cmp	[blink_active],0
	je	blink_timer_done
	mov	eax,ebx
	sub	eax,[blink_time]
	js	blink_timer_done
	lea	eax,[ebx+BLINK_TIME]
	mov	[blink_time],eax
	call	blink_caret
    blink_timer_done:
	mov	eax,[repeat_control]
	test	eax,eax
	jz	no_event_read
	mov	ecx,ebx
	sub	ecx,[repeat_time]
	js	no_event_read
	add	ebx,REPEAT_INTERVAL
	mov	[repeat_time],ebx
	mov	ebx,eax
	mov	eax,CM_TIMER
	call	control_message
    no_event_read:
	stc
	retn
    read_next_event:
	call	x11_read_packet
	cmp	al,1
	je	no_event_read
	test	al,al
	jnz	copy_next_event
	call	x11_report_error
	jmp	no_event_read
    copy_next_event:
	mov	edi,x11_event
	mov	ecx,8
	rep	movsd
	mov	esi,x11_event
    next_event_ready:
	clc
	retn

BLINK_TIME	= 530
REPEAT_DELAY	= 400
REPEAT_INTERVAL = 50
DOUBLE_CLICK_TIME = 400
DOUBLE_CLICK_DISTANCE = 4

reset_blink:
	push	eax
	call	get_time
	add	eax,BLINK_TIME
	mov	[blink_time],eax
	pop	eax
	retn

; start_repeat: ebx = control

start_repeat:
	push	eax
	mov	[repeat_control],ebx
	call	get_time
	add	eax,REPEAT_DELAY
	mov	[repeat_time],eax
	pop	eax
	retn

stop_control_repeat:
	mov	[repeat_control],0
	retn

; blink_caret: toggles caret of the focused editor or edit control

blink_caret:
	mov	ebp,[current_dialog]
	test	ebp,ebp
	jz	blink_main_caret
	cmp	dword [ebp+D_FOCUSED],0
	je	caret_blinked
	mov	ebx,[ebp+D_FOCUS]
	test	ebx,ebx
	jz	caret_blinked
	mov	eax,CM_BLINK
	jmp	control_message
    blink_main_caret:
	jmp	main_blink
    caret_blinked:
	retn

; dispatch_event: esi = event

dispatch_event:
	mov	al,[esi]
	and	al,7Fh
	cmp	al,X11_KeyPress
	je	key_event
	cmp	al,X11_ButtonPress
	je	pointer_event
	cmp	al,X11_ButtonRelease
	je	pointer_event
	cmp	al,X11_MotionNotify
	je	pointer_event
	cmp	al,X11_Expose
	je	expose_event
	cmp	al,X11_FocusIn
	je	focus_event
	cmp	al,X11_FocusOut
	je	focus_event
	cmp	al,X11_ConfigureNotify
	je	configure_event
	cmp	al,X11_MapNotify
	je	map_event
	cmp	al,X11_ClientMessage
	je	client_message_event
	cmp	al,X11_MappingNotify
	je	mapping_event
	cmp	al,X11_SelectionRequest
	je	selection_request_event
	cmp	al,X11_SelectionNotify
	je	selection_notify_event
	cmp	al,X11_SelectionClear
	je	selection_clear_event
	cmp	al,X11_PropertyNotify
	je	property_event
	retn

X11_PointerMotionMask = 40h
X11_ButtonMotionMask  = 2000h
X11_PropertyChangeMask = 400000h
X11_MapNotify	      = 19
X11_PropertyNotify    = 28
X11_SelectionClear    = 29
X11_SelectionRequest  = 30
X11_SelectionNotify   = 31

mapping_event:
	cmp	byte [esi+4],1
	ja	mapping_done
	call	x11_load_keyboard_mapping
    mapping_done:
	retn

; key_event: decodes the key and sends it to the menu, dialog or main window

key_event:
	mov	eax,[esi+4]
	mov	[last_event_time],eax
	movzx	eax,byte [esi+1]
	movzx	edx,word [esi+28]
	mov	[cm_state],edx
	push	eax
	call	lookup_keysym
	mov	[cm_keysym],eax
	pop	eax
	mov	edx,[cm_state]
	call	lookup_base_keysym
	mov	[cm_base_keysym],eax
	mov	eax,[cm_keysym]
	cmp	eax,XK_KP_Home
	jb	key_keysym_ready
	cmp	eax,XK_KP_End
	ja	key_other_keypad
	sub	eax,XK_KP_Home-XK_Home
	jmp	key_keysym_normalized
    key_other_keypad:
	cmp	eax,XK_KP_Insert
	je	key_keypad_insert
	cmp	eax,XK_KP_Delete
	jne	key_keysym_ready
	mov	eax,XK_Delete
	jmp	key_keysym_normalized
    key_keypad_insert:
	mov	eax,XK_Insert
    key_keysym_normalized:
	mov	[cm_keysym],eax
    key_keysym_ready:
	mov	eax,[cm_keysym]
	call	keysym_to_character
	mov	[cm_character],eax
	mov	eax,[cm_state]
	xor	dl,dl
	test	eax,X11_ShiftMask
	jz	key_shift_ok
	or	dl,KS_SHIFT
    key_shift_ok:
	test	eax,X11_ControlMask
	jz	key_control_ok
	or	dl,KS_CONTROL
    key_control_ok:
	test	eax,[x11_alt_mask]
	jz	key_alt_ok
	or	dl,KS_ALT
    key_alt_ok:
	mov	[cm_flags],dl
	cmp	[menu_active],0
	jne	menu_key
	cmp	[dropped_combo],0
	jne	dropped_combo_key
	mov	ebp,[current_dialog]
	test	ebp,ebp
	jnz	dialog_key
	jmp	main_key

; pointer_event: esi = event

pointer_event:
	mov	eax,[esi+4]
	mov	[last_event_time],eax
	movsx	eax,word [esi+24]
	mov	[cm_x],eax
	movsx	eax,word [esi+26]
	mov	[cm_y],eax
	movsx	eax,word [esi+20]
	mov	[cm_root_x],eax
	movsx	eax,word [esi+22]
	mov	[cm_root_y],eax
	movzx	eax,byte [esi+1]
	mov	[cm_button],eax
	movzx	eax,word [esi+28]
	mov	[cm_state],eax
	mov	al,[esi]
	and	al,7Fh
	mov	[cm_event],al
	mov	eax,[esi+12]
	call	find_window
	jc	pointer_outside
	mov	ecx,[edi+4]
	cmp	[menu_active],0
	je	pointer_no_menu
	cmp	ecx,WK_MENU
	je	menu_pointer_event
	cmp	[cm_event],X11_ButtonPress
	jne	pointer_ignored
	jmp	close_menu
    pointer_no_menu:
	mov	eax,[dropped_combo]
	test	eax,eax
	jz	pointer_no_dropped_combo
	cmp	ecx,WK_CONTROL
	jne	pointer_close_combo
	mov	edx,[edi+8]
	cmp	edx,[eax+CB_LIST]
	je	pointer_owner_ok
    pointer_close_combo:
	cmp	[cm_event],X11_ButtonPress
	jne	pointer_ignored
	jmp	close_combo_list
    pointer_no_dropped_combo:
	mov	eax,[edi+12]
	cmp	eax,[current_dialog]
	jne	pointer_ignored
    pointer_owner_ok:
	cmp	ecx,WK_CONTROL
	je	control_pointer_event
	cmp	ecx,WK_EDITOR
	je	editor_pointer_event
	cmp	ecx,WK_MENU
	je	menu_pointer_event
    pointer_ignored:
	retn
    pointer_outside:
	cmp	[menu_active],0
	je	pointer_outside_combo
	cmp	[cm_event],X11_ButtonPress
	je	close_menu
    pointer_outside_combo:
	cmp	[dropped_combo],0
	je	pointer_ignored
	cmp	[cm_event],X11_ButtonPress
	je	close_combo_list
	retn

; control_pointer_event: edi = window entry

control_pointer_event:
	mov	ebx,[edi+8]
	test	dword [ebx+C_STATE],CS_DISABLED
	jnz	pointer_ignored
	mov	al,[cm_event]
	cmp	al,X11_MotionNotify
	je	control_motion
	cmp	al,X11_ButtonRelease
	je	control_release
	mov	eax,[cm_button]
	cmp	eax,4
	jb	control_press
	cmp	eax,5
	ja	pointer_ignored
	mov	eax,CM_WHEEL
	jmp	control_message
    control_press:
	call	reset_blink
	mov	eax,[last_click_time]
	mov	edx,[last_event_time]
	mov	[last_click_time],edx
	sub	edx,eax
	cmp	edx,DOUBLE_CLICK_TIME
	ja	control_single_click
	cmp	ebx,[last_click_control]
	jne	control_single_click
	cmp	[cm_button],1
	jne	control_single_click
	mov	[last_click_control],0
	mov	eax,CM_DOUBLECLICK
	jmp	control_message
    control_single_click:
	mov	[last_click_control],ebx
	call	focus_control_on_click
	mov	eax,CM_PRESS
	jmp	control_message
    control_motion:
	mov	eax,CM_MOTION
	jmp	control_message
    control_release:
	call	stop_control_repeat
	mov	eax,CM_RELEASE
	jmp	control_message

; focus_control_on_click: ebx = control (sets focus within dialog)

focus_control_on_click:
	mov	ebp,[ebx+C_DIALOG]
	test	ebp,ebp
	jz	click_focus_done
	cmp	[cm_button],1
	jne	click_focus_done
	mov	eax,[ebx+C_TYPE]
	cmp	eax,CT_STATIC
	je	click_focus_done
	cmp	eax,CT_ICON
	je	click_focus_done
	cmp	eax,CT_ETCHED
	je	click_focus_done
	cmp	eax,CT_PROGRESS
	je	click_focus_done
	cmp	eax,CT_SCROLLBAR
	je	click_focus_done
	cmp	eax,CT_LISTBOX
	jne	click_focus_set
	mov	eax,[ebx+L_COMBO]
	test	eax,eax
	jnz	click_focus_done
    click_focus_set:
	mov	[keyboard_focus_change],0
	call	set_dialog_focus
    click_focus_done:
	retn

; set_dialog_focus: ebp = dialog, ebx = control to focus

set_dialog_focus:
	push	eax ebx
	mov	eax,[ebp+D_FOCUS]
	cmp	eax,ebx
	je	dialog_focus_set
	mov	[ebp+D_FOCUS],ebx
	test	eax,eax
	jz	old_focus_cleared
	push	ebx
	mov	ebx,eax
	and	dword [ebx+C_STATE],not CS_FOCUSED
	cmp	dword [ebp+D_FOCUSED],0
	je	old_focus_repaint
	mov	eax,CM_UNFOCUS
	call	control_message
    old_focus_repaint:
	call	control_redraw
	pop	ebx
    old_focus_cleared:
	test	ebx,ebx
	jz	dialog_focus_set
	or	dword [ebx+C_STATE],CS_FOCUSED
	cmp	dword [ebp+D_FOCUSED],0
	je	new_focus_repaint
	mov	eax,CM_FOCUS
	call	control_message
    new_focus_repaint:
	call	control_redraw
	call	reset_blink
    dialog_focus_set:
	pop	ebx eax
	retn

; expose_event: esi = event

expose_event:
	mov	eax,[esi+4]
	call	find_window
	jc	expose_done
	mov	ecx,[edi+4]
	cmp	ecx,WK_EDITOR
	je	editor_expose
	cmp	word [esi+16],0
	jne	expose_done
	cmp	ecx,WK_CONTROL
	je	control_expose
	cmp	ecx,WK_MENU
	je	menu_expose
    expose_done:
	retn
    control_expose:
	mov	ebx,[edi+8]
	jmp	control_redraw

; focus_event: esi = event

focus_event:
	mov	eax,[esi+4]
	call	find_window
	jc	focus_done
	mov	ecx,[edi+4]
	cmp	ecx,WK_MAIN
	je	main_focus_event
	cmp	ecx,WK_DIALOG
	jne	focus_done
	mov	ebp,[edi+8]
	cmp	byte [esi+1],2
	je	focus_done
	xor	eax,eax
	cmp	byte [esi],X11_FocusIn
	jne	dialog_focus_flag
	inc	eax
    dialog_focus_flag:
	cmp	eax,[ebp+D_FOCUSED]
	je	focus_done
	mov	[ebp+D_FOCUSED],eax
	mov	ebx,[ebp+D_FOCUS]
	test	ebx,ebx
	jz	focus_done
	mov	eax,CM_UNFOCUS
	cmp	dword [ebp+D_FOCUSED],0
	je	dialog_focus_message
	mov	eax,CM_FOCUS
	call	reset_blink
    dialog_focus_message:
	call	control_message
	jmp	control_redraw
    focus_done:
	retn

; configure_event: esi = event

configure_event:
	mov	eax,[esi+8]
	call	find_window
	jc	configure_done
	cmp	dword [edi+4],WK_MAIN
	je	main_configure_event
    configure_done:
	retn

; map_event: esi = event

map_event:
	mov	eax,[esi+8]
	call	find_window
	jc	map_done
	cmp	dword [edi+4],WK_DIALOG
	jne	map_done
	mov	ebp,[edi+8]
	cmp	dword [ebp+D_MAPPED],0
	jne	map_done
	mov	dword [ebp+D_MAPPED],1
	mov	eax,[ebp+D_WINDOW]
	xor	ecx,ecx
	call	x11_set_input_focus
    map_done:
	retn

; client_message_event: esi = event

client_message_event:
	mov	eax,[esi+8]
	cmp	eax,[atom_wm_protocols]
	je	protocol_message
	jmp	dnd_message
    protocol_message:
	mov	eax,[esi+12]
	cmp	eax,[atom_wm_delete_window]
	jne	client_message_done
	mov	eax,[esi+4]
	call	find_window
	jc	client_message_done
	mov	ecx,[edi+4]
	cmp	ecx,WK_MAIN
	je	main_close_event
	cmp	ecx,WK_DIALOG
	jne	client_message_done
	mov	ebp,[edi+8]
	cmp	ebp,[current_dialog]
	jne	client_message_done
	mov	eax,DM_CLOSE
	push	ebp
	call	dword [ebp+D_PROC]
	pop	ebp
    client_message_done:
	retn

; dialog_box: esi = template, edi = dialog procedure, eax = parameter
; returns eax = result of dialog

dialog_box:
	push	ebx ecx edx esi edi ebp
	mov	[new_dialog_template],esi
	mov	[new_dialog_procedure],edi
	mov	[new_dialog_parameter],eax
	cmp	[menu_active],0
	je	dialog_menu_closed
	call	close_menu
    dialog_menu_closed:
	cmp	[dropped_combo],0
	je	dialog_combo_closed
	call	close_combo_list
    dialog_combo_closed:
	mov	ecx,DIALOG_MEMORY
	call	get_memory
	test	eax,eax
	jz	dialog_failed
	mov	ebp,eax
	mov	[ebp+D_HANDLE],ebx
	mov	eax,[new_dialog_template]
	mov	[ebp+D_TEMPLATE],eax
	mov	eax,[new_dialog_procedure]
	mov	[ebp+D_PROC],eax
	mov	eax,[new_dialog_parameter]
	mov	[ebp+D_PARAM],eax
	lea	eax,[ebp+1000h]
	mov	[ebp+D_ARENA],eax
	lea	eax,[ebp+DIALOG_MEMORY]
	mov	[ebp+D_ARENA_END],eax
	mov	eax,[current_dialog]
	mov	[ebp+D_PARENT],eax
	mov	esi,[ebp+D_TEMPLATE]
	mov	eax,[esi+4]
	mov	edx,[esi+8]
	test	dword [esi+12],DT_PIXELS
	jnz	dialog_size_ok
	call	horizontal_dialog_units
	xchg	eax,edx
	call	vertical_dialog_units
	xchg	eax,edx
    dialog_size_ok:
	mov	[ebp+D_WIDTH],eax
	mov	[ebp+D_HEIGHT],edx
	mov	eax,[main_window]
	xor	ecx,ecx
	xor	edx,edx
	call	x11_translate_coordinates
	mov	eax,[main_width]
	sub	eax,[ebp+D_WIDTH]
	sar	eax,1
	add	ecx,eax
	jns	dialog_x_ok
	xor	ecx,ecx
    dialog_x_ok:
	mov	eax,[main_height]
	sub	eax,[ebp+D_HEIGHT]
	sar	eax,1
	add	edx,eax
	jns	dialog_y_ok
	xor	edx,edx
    dialog_y_ok:
	mov	[dialog_x],ecx
	mov	[dialog_y],edx
	mov	eax,[px_face]
	mov	[window_values],eax
	mov	[window_values+4],X11_KeyPressMask+X11_StructureNotifyMask+X11_FocusChangeMask+X11_ExposureMask
	mov	[window_value_mask],2+800h
	mov	eax,[x11_root]
	mov	esi,[ebp+D_WIDTH]
	mov	edi,[ebp+D_HEIGHT]
	call	x11_create_window
	mov	[ebp+D_WINDOW],eax
	mov	ecx,WK_DIALOG
	mov	edx,ebp
	mov	esi,ebp
	call	register_window
	call	set_dialog_properties
	mov	eax,[building_dialog]
	push	eax
	mov	[building_dialog],ebp
	mov	esi,[ebp+D_TEMPLATE]
	add	esi,16
	mov	edx,[esi-16]
	mov	esi,edx
    skip_dialog_title:
	lodsb
	test	al,al
	jnz	skip_dialog_title
	add	esi,3
	and	esi,not 3
    create_dialog_items:
	cmp	dword [esi],0
	je	dialog_items_created
	cmp	dword [ebp+D_COUNT],DIALOG_MAX_CONTROLS
	jae	dialog_items_created
	push	esi
	mov	eax,[esi+4]
	mov	[new_control_type],eax
	mov	eax,[esi+8]
	mov	[new_control_id],eax
	mov	eax,[esi+28]
	mov	[new_control_style],eax
	mov	eax,[esi+12]
	mov	edx,[esi+16]
	mov	ebx,[ebp+D_TEMPLATE]
	test	dword [ebx+12],DT_PIXELS
	jnz	item_position_ok
	call	horizontal_dialog_units
	xchg	eax,edx
	call	vertical_dialog_units
	xchg	eax,edx
    item_position_ok:
	mov	[new_control_x],eax
	mov	[new_control_y],edx
	mov	eax,[esi+20]
	mov	edx,[esi+24]
	test	dword [ebx+12],DT_PIXELS
	jnz	item_size_ok
	call	horizontal_dialog_units
	xchg	eax,edx
	call	vertical_dialog_units
	xchg	eax,edx
    item_size_ok:
	mov	[new_control_width],eax
	mov	[new_control_height],edx
	lea	eax,[esi+32]
	mov	[new_control_text],eax
	mov	eax,[ebp+D_WINDOW]
	mov	[new_control_parent],eax
	mov	[new_control_notify],0
	call	create_control
	pop	esi
	test	ebx,ebx
	jz	next_dialog_item
	mov	eax,[ebp+D_COUNT]
	mov	[ebp+D_CONTROLS+eax*4],ebx
	inc	dword [ebp+D_COUNT]
    next_dialog_item:
	mov	esi,[esi]
	jmp	create_dialog_items
    dialog_items_created:
	pop	eax
	mov	[building_dialog],eax
	mov	eax,[current_dialog]
	push	eax
	mov	[current_dialog],ebp
	push	ebp
	mov	[building_dialog],ebp
	mov	eax,DM_INIT
	call	dword [ebp+D_PROC]
	pop	ebp
	mov	[building_dialog],0
	cmp	dword [ebp+D_FOCUS],0
	jne	initial_focus_ok
	xor	ebx,ebx
	call	next_focus_control
	test	ebx,ebx
	jz	initial_focus_ok
	mov	[keyboard_focus_change],1
	call	set_dialog_focus
    initial_focus_ok:
	cmp	dword [ebp+D_ENDED],0
	jne	dialog_finished
	mov	eax,[ebp+D_WINDOW]
	call	x11_map_window
	lea	esi,[ebp+D_ENDED]
	call	event_loop
    dialog_finished:
	call	stop_control_repeat
	cmp	[dropped_combo],0
	je	dialog_closing
	call	close_combo_list
    dialog_closing:
	xor	ecx,ecx
    destroy_dialog_controls:
	cmp	ecx,[ebp+D_COUNT]
	jae	dialog_controls_destroyed
	push	ecx
	mov	ebx,[ebp+D_CONTROLS+ecx*4]
	call	destroy_control
	pop	ecx
	inc	ecx
	jmp	destroy_dialog_controls
    dialog_controls_destroyed:
	mov	eax,[ebp+D_WINDOW]
	call	unregister_window
	call	x11_destroy_window
	call	unregister_dialog_windows
	pop	eax
	mov	[current_dialog],eax
	mov	esi,[ebp+D_RESULT]
	mov	ebx,[ebp+D_HANDLE]
	call	release_memory
	mov	eax,[current_dialog]
	test	eax,eax
	jz	focus_main_after_dialog
	mov	eax,[eax+D_WINDOW]
	jmp	restore_focus_after_dialog
    focus_main_after_dialog:
	mov	eax,[main_window]
    restore_focus_after_dialog:
	xor	ecx,ecx
	call	x11_set_input_focus
	mov	eax,esi
	pop	ebp edi esi edx ecx ebx
	retn
    dialog_failed:
	or	eax,-1
	pop	ebp edi esi edx ecx ebx
	retn

; unregister_dialog_windows: ebp = dialog, removes remaining windows owned by it

unregister_dialog_windows:
	push	ecx edi
	mov	edi,window_table
	mov	ecx,WINDOW_TABLE_LENGTH
    unregister_owned_windows:
	cmp	[edi+12],ebp
	jne	next_owned_window
	mov	dword [edi],0
	mov	dword [edi+12],0
    next_owned_window:
	add	edi,16
	loop	unregister_owned_windows
	pop	edi ecx
	retn

horizontal_dialog_units:
	push	ecx edx
	imul	eax,[ui_font_width]
	imul	eax,13
	cdq
	mov	ecx,40
	idiv	ecx
	pop	edx ecx
	retn
vertical_dialog_units:
	imul	eax,[ui_font_height]
	sar	eax,3
	retn

; set_dialog_properties: ebp = dialog

set_dialog_properties:
	mov	esi,[ebp+D_TEMPLATE]
	mov	esi,[esi]
	call	string_length
	mov	edi,ecx
	mov	eax,[ebp+D_WINDOW]
	mov	ebx,X11_WM_NAME
	mov	ecx,X11_STRING
	mov	edx,8
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
	mov	ebx,X11_WM_TRANSIENT_FOR
	mov	ecx,X11_WINDOW
	mov	esi,main_window
	call	x11_change_property
	mov	ebx,[atom_net_wm_window_type]
	mov	ecx,X11_ATOM
	mov	esi,atom_net_wm_window_type_dialog
	call	x11_change_property
	mov	ebx,X11_WM_HINTS
	mov	ecx,X11_WM_HINTS
	mov	esi,wm_hints
	mov	edi,9
	call	x11_change_property
	mov	edi,size_hints
	mov	dword [edi],1+4+8+16+32
	mov	ecx,[dialog_x]
	mov	[edi+4],ecx
	mov	ecx,[dialog_y]
	mov	[edi+8],ecx
	mov	ecx,[ebp+D_WIDTH]
	mov	[edi+12],ecx
	mov	[edi+20],ecx
	mov	[edi+28],ecx
	mov	ecx,[ebp+D_HEIGHT]
	mov	[edi+16],ecx
	mov	[edi+24],ecx
	mov	[edi+32],ecx
	mov	ebx,X11_WM_NORMAL_HINTS
	mov	ecx,X11_WM_SIZE_HINTS
	mov	esi,size_hints
	mov	edi,18
	call	x11_change_property
	retn

X11_WINDOW	     = 33
X11_WM_NORMAL_HINTS  = 40
X11_WM_SIZE_HINTS    = 41
X11_WM_TRANSIENT_FOR = 68

; end_dialog: eax = result (ends [current_dialog])

end_dialog:
	push	ebp
	mov	ebp,[current_dialog]
	test	ebp,ebp
	jz	dialog_end_set
	mov	[ebp+D_RESULT],eax
	mov	dword [ebp+D_ENDED],1
    dialog_end_set:
	pop	ebp
	retn

; dialog_item: eax = id, returns ebx = control of [current_dialog] (0 if none)

dialog_item:
	push	ecx ebp
	mov	ebp,[current_dialog]
	xor	ebx,ebx
	test	ebp,ebp
	jz	dialog_item_found
	xor	ecx,ecx
    search_dialog_item:
	cmp	ecx,[ebp+D_COUNT]
	jae	dialog_item_missing
	mov	ebx,[ebp+D_CONTROLS+ecx*4]
	cmp	[ebx+C_ID],eax
	je	dialog_item_found
	inc	ecx
	jmp	search_dialog_item
    dialog_item_missing:
	xor	ebx,ebx
    dialog_item_found:
	pop	ebp ecx
	retn

; next_focus_control: ebp = dialog, ebx = current control (0 for start)
; returns ebx = next control accepting focus (0 if none);
; [focus_backward] selects direction

next_focus_control:
	push	eax ecx edx
	mov	edx,[ebp+D_COUNT]
	test	edx,edx
	jz	no_focus_control
	or	ecx,-1
	test	ebx,ebx
	jz	focus_search_start
	xor	ecx,ecx
    find_current_focus:
	cmp	ecx,edx
	jae	focus_search_from_start
	cmp	[ebp+D_CONTROLS+ecx*4],ebx
	je	focus_search_start
	inc	ecx
	jmp	find_current_focus
    focus_search_from_start:
	or	ecx,-1
    focus_search_start:
	push	edx
    try_next_focus:
	cmp	[focus_backward],0
	jne	previous_focus_index
	inc	ecx
	cmp	ecx,edx
	jb	focus_index_ok
	xor	ecx,ecx
	jmp	focus_index_ok
    previous_focus_index:
	dec	ecx
	jns	focus_index_ok
	lea	ecx,[edx-1]
    focus_index_ok:
	mov	eax,[ebp+D_CONTROLS+ecx*4]
	test	dword [eax+C_STYLE],WS_TABSTOP
	jz	focus_candidate_rejected
	test	dword [eax+C_STATE],CS_DISABLED
	jnz	focus_candidate_rejected
	cmp	dword [eax+C_TYPE],CT_RADIO
	jne	focus_candidate_found
	test	dword [eax+C_STATE],CS_CHECKED
	jnz	focus_candidate_found
	call	radio_group_has_checked
	jnc	focus_candidate_found
    focus_candidate_rejected:
	dec	dword [esp]
	jnz	try_next_focus
	pop	edx
    no_focus_control:
	xor	ebx,ebx
	pop	edx ecx eax
	retn
    focus_candidate_found:
	pop	edx
	mov	ebx,eax
	pop	edx ecx eax
	retn

; radio_group_has_checked: eax = radio control, ebp = dialog
; returns CF=1 when another radio button of its group is checked

radio_group_has_checked:
	push	ebx ecx edx
	call	radio_group_bounds
    check_radio_group:
	cmp	ecx,edx
	ja	radio_group_unchecked
	mov	ebx,[ebp+D_CONTROLS+ecx*4]
	test	dword [ebx+C_STATE],CS_CHECKED
	jnz	radio_group_checked
	inc	ecx
	jmp	check_radio_group
    radio_group_unchecked:
	pop	edx ecx ebx
	clc
	retn
    radio_group_checked:
	pop	edx ecx ebx
	stc
	retn

; radio_group_bounds: eax = radio control, ebp = dialog
; returns ecx = index of first, edx = index of last radio in group

radio_group_bounds:
	push	ebx
	xor	ecx,ecx
    find_radio_index:
	cmp	[ebp+D_CONTROLS+ecx*4],eax
	je	radio_index_found
	inc	ecx
	cmp	ecx,[ebp+D_COUNT]
	jb	find_radio_index
	xor	ecx,ecx
	xor	edx,edx
	pop	ebx
	retn
    radio_index_found:
	mov	edx,ecx
    find_group_start:
	test	ecx,ecx
	jz	find_group_end
	mov	ebx,[ebp+D_CONTROLS+(ecx-1)*4]
	cmp	dword [ebx+C_TYPE],CT_RADIO
	je	group_start_extends
	mov	ebx,[ebp+D_CONTROLS+(ecx-1)*4]
	cmp	dword [ebx+C_TYPE],CT_EDIT
	jne	find_group_end
	cmp	ecx,1
	jbe	find_group_end
	mov	ebx,[ebp+D_CONTROLS+(ecx-2)*4]
	cmp	dword [ebx+C_TYPE],CT_RADIO
	jne	find_group_end
	dec	ecx
    group_start_extends:
	dec	ecx
	jmp	find_group_start
    find_group_end:
	lea	ebx,[edx+1]
	cmp	ebx,[ebp+D_COUNT]
	jae	radio_group_found
	mov	ebx,[ebp+D_CONTROLS+(edx+1)*4]
	cmp	dword [ebx+C_TYPE],CT_RADIO
	je	group_end_extends
	cmp	dword [ebx+C_TYPE],CT_EDIT
	jne	radio_group_found
	lea	ebx,[edx+2]
	cmp	ebx,[ebp+D_COUNT]
	jae	radio_group_found
	mov	ebx,[ebp+D_CONTROLS+(edx+2)*4]
	cmp	dword [ebx+C_TYPE],CT_RADIO
	jne	radio_group_found
	inc	edx
    group_end_extends:
	inc	edx
	jmp	find_group_end
    radio_group_found:
	pop	ebx
	retn

; control_text_color: ebx = control, returns ecx = pixel for text

control_text_color:
	mov	ecx,[px_text]
	test	dword [ebx+C_STATE],CS_DISABLED
	jz	text_color_ok
	mov	ecx,[px_gray]
    text_color_ok:
	retn

; fill_control: ebx = control, eax = pixel

fill_control:
	push	eax ebx ecx edx esi edi
	mov	edx,ebx
	mov	ebx,eax
	mov	eax,[edx+C_WINDOW]
	xor	ecx,ecx
	mov	esi,[edx+C_WIDTH]
	mov	edi,[edx+C_HEIGHT]
	xor	edx,edx
	call	ui_fill
	pop	edi esi edx ecx ebx eax
	retn

; frame_control: ebx = control, eax = frame style

frame_control:
	push	eax ebx ecx edx esi edi
	mov	edx,ebx
	mov	ebx,eax
	mov	eax,[edx+C_WINDOW]
	xor	ecx,ecx
	mov	esi,[edx+C_WIDTH]
	mov	edi,[edx+C_HEIGHT]
	xor	edx,edx
	call	ui_frame
	pop	edi esi edx ecx ebx eax
	retn

; set_control_text: ebx = control, esi = UTF-8 text

set_control_text:
	push	eax
	mov	eax,[ebx+C_TYPE]
	cmp	eax,CT_EDIT
	je	set_edit_text
	cmp	eax,CT_COMBOBOX
	je	set_combo_text
	cmp	eax,CT_FEDIT
	je	set_fedit_text
	mov	eax,[building_dialog]
	push	eax
	mov	eax,[ebx+C_DIALOG]
	mov	[building_dialog],eax
	call	ui_store_text
	test	eax,eax
	jz	keep_control_text
	mov	[ebx+C_TEXT],eax
    keep_control_text:
	pop	eax
	mov	[building_dialog],eax
	call	control_redraw
	pop	eax
	retn
    set_combo_text:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	control_text_set
    set_edit_text:
	call	edit_set_text
    control_text_set:
	pop	eax
	retn
    set_fedit_text:
	call	fedit_control_set_text
	pop	eax
	retn

; get_control_text: ebx = control, edi = buffer, ecx = size
; returns eax = length of UTF-8 text

get_control_text:
	mov	eax,[ebx+C_TYPE]
	cmp	eax,CT_EDIT
	je	edit_get_text
	cmp	eax,CT_COMBOBOX
	jne	get_plain_control_text
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jnz	edit_get_text
	mov	eax,[ebx+CB_LIST]
	push	ebx
	mov	ebx,eax
	mov	eax,[ebx+L_SELECTION]
	call	list_get_string
	pop	ebx
	jnc	copy_control_text
	mov	byte [edi],0
	xor	eax,eax
	retn
    get_plain_control_text:
	mov	esi,[ebx+C_TEXT]
    copy_control_text:
	push	edi
	jecxz	control_text_copied
	dec	ecx
    copy_text_byte:
	lodsb
	test	al,al
	jz	control_text_copied
	stosb
	loop	copy_text_byte
    control_text_copied:
	mov	byte [edi],0
	mov	eax,edi
	pop	edi
	sub	eax,edi
	retn

; static control

static_proc:
	cmp	eax,CM_PAINT
	jne	static_done
	mov	eax,[px_face]
	call	fill_control
	mov	[ui_text_mnemonics],1
	mov	eax,[px_face]
	mov	[ui_text_background],eax
	call	control_text_color
	mov	[static_color],ecx
	mov	esi,[ebx+C_TEXT]
	xor	edx,edx
    paint_static_line:
	cmp	byte [esi],0
	je	static_done
	call	ui_text_width
	xor	ecx,ecx
	test	dword [ebx+C_STYLE],SS_RIGHT
	jz	static_not_right
	mov	ecx,[ebx+C_WIDTH]
	sub	ecx,eax
	jmp	static_x_ok
    static_not_right:
	test	dword [ebx+C_STYLE],SS_CENTER
	jz	static_x_ok
	mov	ecx,[ebx+C_WIDTH]
	sub	ecx,eax
	sar	ecx,1
    static_x_ok:
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[static_color]
	call	ui_draw_text
	pop	ebx
	add	edx,[ui_font_height]
	jmp	paint_static_line
    static_done:
	retn

; push button

button_proc:
	cmp	eax,CM_PAINT
	je	paint_button
	cmp	eax,CM_PRESS
	je	button_press
	cmp	eax,CM_MOTION
	je	button_motion
	cmp	eax,CM_RELEASE
	je	button_release
	cmp	eax,CM_KEY
	je	button_key
	cmp	eax,CM_DOUBLECLICK
	je	button_press
	retn
    paint_button:
	mov	eax,[px_face]
	call	fill_control
	mov	eax,[ebx+C_WINDOW]
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,[ebx+C_WIDTH]
	mov	edi,[ebx+C_HEIGHT]
	test	dword [ebx+C_STYLE],BS_DEFPUSHBUTTON
	jnz	default_button_frame
	test	dword [ebx+C_STATE],CS_FOCUSED
	jz	button_frame
    default_button_frame:
	push	ebx
	mov	ebx,FRAME_BLACK
	call	ui_frame
	pop	ebx
	inc	ecx
	inc	edx
	sub	esi,2
	sub	edi,2
    button_frame:
	push	ebx
	test	dword [ebx+C_STATE],CS_PRESSED
	mov	ebx,FRAME_RAISED
	jz	button_frame_style_ok
	mov	ebx,FRAME_PRESSED
    button_frame_style_ok:
	call	ui_frame
	pop	ebx
	mov	[ui_text_mnemonics],1
	mov	eax,[px_face]
	mov	[ui_text_background],eax
	mov	esi,[ebx+C_TEXT]
	call	ui_text_width
	mov	ecx,[ebx+C_WIDTH]
	sub	ecx,eax
	sar	ecx,1
	mov	edx,[ebx+C_HEIGHT]
	sub	edx,[ui_font_height]
	sar	edx,1
	test	dword [ebx+C_STATE],CS_PRESSED
	jz	button_text_position_ok
	inc	ecx
	inc	edx
    button_text_position_ok:
	push	ecx edx
	push	ebx
	call	control_text_color
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,ecx
	mov	ecx,[esp+8]
	call	ui_draw_text
	pop	ebx
	pop	edx ecx
	test	dword [ebx+C_STATE],CS_FOCUSED
	jz	button_painted
	mov	eax,[ebx+C_WINDOW]
	mov	ecx,4
	mov	edx,4
	mov	esi,[ebx+C_WIDTH]
	sub	esi,8
	mov	edi,[ebx+C_HEIGHT]
	sub	edi,8
	push	ebx
	mov	ebx,FRAME_THIN_SUNKEN
	call	ui_frame
	pop	ebx
    button_painted:
	retn
    button_press:
	cmp	[cm_button],1
	jne	button_done
	or	dword [ebx+C_STATE],CS_PRESSED
	mov	dword [ebx+E_DRAG],1
	jmp	control_redraw
    button_motion:
	cmp	dword [ebx+E_DRAG],0
	je	button_done
	call	pointer_inside_control
	mov	eax,[ebx+C_STATE]
	jc	button_pointer_outside
	or	dword [ebx+C_STATE],CS_PRESSED
	jmp	button_state_changed
    button_pointer_outside:
	and	dword [ebx+C_STATE],not CS_PRESSED
    button_state_changed:
	cmp	eax,[ebx+C_STATE]
	jne	control_redraw
    button_done:
	retn
    button_release:
	cmp	dword [ebx+E_DRAG],0
	je	button_done
	mov	dword [ebx+E_DRAG],0
	test	dword [ebx+C_STATE],CS_PRESSED
	jz	button_done
	and	dword [ebx+C_STATE],not CS_PRESSED
	call	control_redraw
	jmp	click_control
    button_key:
	mov	eax,[cm_keysym]
	cmp	eax,20h
	jne	button_done
    click_control:
	mov	eax,[ebx+C_TYPE]
	cmp	eax,CT_CHECKBOX
	je	toggle_checkbox
	cmp	eax,CT_RADIO
	je	check_radio
	mov	edx,BN_CLICKED
	jmp	control_notify
    toggle_checkbox:
	xor	dword [ebx+C_STATE],CS_CHECKED
	call	control_redraw
	mov	edx,BN_CLICKED
	jmp	control_notify
    check_radio:
	call	select_radio
	mov	edx,BN_CLICKED
	jmp	control_notify

; pointer_inside_control: ebx = control, returns CF=1 if [cm_x],[cm_y] outside

pointer_inside_control:
	mov	eax,[cm_x]
	cmp	eax,0
	jl	pointer_outside_control
	cmp	eax,[ebx+C_WIDTH]
	jge	pointer_outside_control
	mov	eax,[cm_y]
	cmp	eax,0
	jl	pointer_outside_control
	cmp	eax,[ebx+C_HEIGHT]
	jge	pointer_outside_control
	clc
	retn
    pointer_outside_control:
	stc
	retn

; check box

checkbox_proc:
	cmp	eax,CM_PAINT
	je	paint_checkbox
	cmp	eax,CM_KEY
	je	button_key
	cmp	eax,CM_PRESS
	je	button_press
	cmp	eax,CM_MOTION
	je	button_motion
	cmp	eax,CM_RELEASE
	je	checkbox_release
	cmp	eax,CM_DOUBLECLICK
	je	button_press
	retn
    checkbox_release:
	cmp	dword [ebx+E_DRAG],0
	je	checkbox_done
	mov	dword [ebx+E_DRAG],0
	test	dword [ebx+C_STATE],CS_PRESSED
	jz	checkbox_done
	and	dword [ebx+C_STATE],not CS_PRESSED
	jmp	click_control
    checkbox_done:
	retn
    paint_checkbox:
	mov	eax,[px_face]
	call	fill_control
	call	check_mark_position
	mov	eax,[ebx+C_WINDOW]
	mov	esi,13
	mov	edi,13
	push	ebx
	mov	ebx,FRAME_SUNKEN
	call	ui_frame
	pop	ebx
	push	ecx edx
	add	ecx,2
	add	edx,2
	mov	esi,9
	mov	edi,9
	push	ebx
	test	dword [ebx+C_STATE],CS_DISABLED
	mov	ebx,[px_window]
	jz	check_box_background_ok
	mov	ebx,[px_face]
    check_box_background_ok:
	call	ui_fill
	pop	ebx
	pop	edx ecx
	test	dword [ebx+C_STATE],CS_CHECKED
	jz	check_mark_done
	push	ebx
	call	control_text_color
	mov	ebx,ecx
	pop	ecx
	push	ecx
	mov	ecx,[check_x]
	mov	edx,[check_y]
	add	ecx,3
	add	edx,5
	mov	[check_line],3
    draw_check_mark:
	lea	esi,[ecx+2]
	lea	edi,[edx+2]
	call	ui_line
	lea	ecx,[esi]
	lea	edx,[edi]
	add	esi,4
	sub	edi,4
	call	ui_line
	sub	ecx,2
	sub	edx,1
	dec	[check_line]
	jnz	draw_check_mark
	pop	ebx
    check_mark_done:
	jmp	paint_check_text

; check_mark_position: ebx = control, returns ecx, edx = position of box

check_mark_position:
	mov	edx,[ebx+C_HEIGHT]
	sub	edx,13
	sar	edx,1
	xor	ecx,ecx
	test	dword [ebx+C_STYLE],BS_LEFTTEXT
	jz	check_position_ok
	mov	ecx,[ebx+C_WIDTH]
	sub	ecx,13
    check_position_ok:
	mov	[check_x],ecx
	mov	[check_y],edx
	retn

; paint_check_text: ebx = control (check box or radio button)

paint_check_text:
	mov	[ui_text_mnemonics],1
	mov	eax,[px_face]
	mov	[ui_text_background],eax
	mov	esi,[ebx+C_TEXT]
	mov	ecx,17
	test	dword [ebx+C_STYLE],BS_LEFTTEXT
	jz	check_text_x_ok
	call	ui_text_width
	mov	ecx,[ebx+C_WIDTH]
	sub	ecx,17
	sub	ecx,eax
    check_text_x_ok:
	mov	edx,[ebx+C_HEIGHT]
	sub	edx,[ui_font_height]
	sar	edx,1
	push	ecx
	push	ebx
	call	control_text_color
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,ecx
	mov	ecx,[esp+4]
	call	ui_draw_text
	pop	ebx
	pop	eax
	test	dword [ebx+C_STATE],CS_FOCUSED
	jz	check_text_painted
	mov	esi,ecx
	sub	esi,eax
	add	esi,2
	lea	ecx,[eax-1]
	dec	edx
	mov	edi,[ui_font_height]
	add	edi,2
	mov	eax,[ebx+C_WINDOW]
	push	ebx
	mov	ebx,FRAME_THIN_SUNKEN
	call	ui_frame
	pop	ebx
    check_text_painted:
	retn

; radio button

radio_proc:
	cmp	eax,CM_PAINT
	je	paint_radio
	cmp	eax,CM_KEY
	je	radio_key
	cmp	eax,CM_PRESS
	je	button_press
	cmp	eax,CM_MOTION
	je	button_motion
	cmp	eax,CM_RELEASE
	je	checkbox_release
	retn
    paint_radio:
	mov	eax,[px_face]
	call	fill_control
	call	check_mark_position
	mov	eax,[ebx+C_WINDOW]
	mov	esi,12
	mov	edi,12
	push	ebx
	test	dword [ebx+C_STATE],CS_DISABLED
	mov	ebx,[px_window]
	jz	radio_background_ok
	mov	ebx,[px_face]
    radio_background_ok:
	call	ui_set_fill_color
	call	x11_fill_arc
	mov	ebx,[px_shadow]
	call	ui_set_fill_color
	call	x11_draw_arc
	pop	ebx
	test	dword [ebx+C_STATE],CS_CHECKED
	jz	paint_check_text
	add	ecx,3
	add	edx,3
	mov	esi,6
	mov	edi,6
	push	ebx
	call	control_text_color
	mov	ebx,ecx
	call	ui_set_fill_color
	mov	ecx,[check_x]
	add	ecx,3
	call	x11_fill_arc
	pop	ebx
	jmp	paint_check_text
    radio_key:
	mov	eax,[cm_keysym]
	cmp	eax,20h
	je	button_key
	cmp	eax,XK_Up
	je	previous_radio
	cmp	eax,XK_Left
	je	previous_radio
	cmp	eax,XK_Down
	je	next_radio
	cmp	eax,XK_Right
	je	next_radio
	retn
    previous_radio:
	mov	ebp,[ebx+C_DIALOG]
	mov	eax,ebx
	call	radio_group_bounds
	mov	esi,ecx
	call	radio_index
	dec	ecx
	cmp	ecx,esi
	jge	select_radio_index
	mov	ecx,edx
	jmp	select_radio_index
    next_radio:
	mov	ebp,[ebx+C_DIALOG]
	mov	eax,ebx
	call	radio_group_bounds
	mov	esi,ecx
	call	radio_index
    find_next_radio:
	inc	ecx
	cmp	ecx,edx
	jbe	check_next_radio_type
	mov	ecx,esi
    check_next_radio_type:
	mov	eax,[ebp+D_CONTROLS+ecx*4]
	cmp	dword [eax+C_TYPE],CT_RADIO
	jne	find_next_radio
	jmp	radio_selected_by_key
    select_radio_index:
	mov	eax,[ebp+D_CONTROLS+ecx*4]
	cmp	dword [eax+C_TYPE],CT_RADIO
	je	radio_selected_by_key
	cmp	ecx,esi
	jle	radio_selected_by_key
	dec	ecx
	jmp	select_radio_index
    radio_selected_by_key:
	mov	ebx,eax
	mov	[keyboard_focus_change],1
	call	set_dialog_focus
	jmp	click_control

; ui_set_fill_color: ebx = pixel, returns ebx = gc with this color

ui_set_fill_color:
	push	ecx edx
	mov	ecx,ebx
	mov	edx,ebx
	call	ui_set_colors
	pop	edx ecx
	mov	ebx,[ui_gc]
	retn

; radio_index: ebx = control, ebp = dialog, returns ecx = index

radio_index:
	xor	ecx,ecx
    find_control_index:
	cmp	[ebp+D_CONTROLS+ecx*4],ebx
	je	control_index_found
	inc	ecx
	cmp	ecx,[ebp+D_COUNT]
	jb	find_control_index
    control_index_found:
	retn

; select_radio: ebx = radio button, checks it and unchecks others in group

select_radio:
	push	ebx
	mov	ebp,[ebx+C_DIALOG]
	test	ebp,ebp
	jz	radio_group_updated
	mov	eax,ebx
	call	radio_group_bounds
    uncheck_radio_group:
	cmp	ecx,edx
	ja	radio_group_updated
	mov	ebx,[ebp+D_CONTROLS+ecx*4]
	cmp	dword [ebx+C_TYPE],CT_RADIO
	jne	next_radio_in_group
	test	dword [ebx+C_STATE],CS_CHECKED
	jz	next_radio_in_group
	and	dword [ebx+C_STATE],not CS_CHECKED
	call	control_redraw
    next_radio_in_group:
	inc	ecx
	jmp	uncheck_radio_group
    radio_group_updated:
	pop	ebx
	or	dword [ebx+C_STATE],CS_CHECKED
	jmp	control_redraw

; etched line

etched_proc:
	cmp	eax,CM_PAINT
	jne	etched_done
	mov	eax,[px_face]
	call	fill_control
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	esi,[ebx+C_WIDTH]
	dec	esi
	mov	edx,[ebx+C_HEIGHT]
	sar	edx,1
	mov	edi,edx
	xor	ecx,ecx
	mov	ebx,[px_shadow]
	call	ui_line
	inc	edx
	inc	edi
	mov	ebx,[px_light]
	call	ui_line
	pop	ebx
    etched_done:
	retn

; progress bar

progress_proc:
	cmp	eax,CM_PAINT
	jne	progress_done
	mov	eax,FRAME_THIN_SUNKEN
	call	frame_control
	push	ebx
	mov	eax,[ebx+C_WIDTH]
	sub	eax,2
	mov	ecx,[ebx+P_MAX]
	test	ecx,ecx
	jz	progress_empty
	mul	dword [ebx+P_POS]
	div	ecx
	jmp	progress_width_ok
    progress_empty:
	xor	eax,eax
    progress_width_ok:
	mov	esi,eax
	mov	edi,[ebx+C_HEIGHT]
	sub	edi,2
	mov	eax,[ebx+C_WINDOW]
	mov	ecx,1
	mov	edx,1
	push	esi
	mov	ebx,[px_selection]
	test	esi,esi
	jz	progress_bar_drawn
	call	ui_fill
    progress_bar_drawn:
	pop	esi
	mov	ebx,[esp]
	add	ecx,esi
	neg	esi
	add	esi,[ebx+C_WIDTH]
	sub	esi,2
	mov	ebx,[px_window]
	jle	progress_rest_drawn
	call	ui_fill
    progress_rest_drawn:
	pop	ebx
    progress_done:
	retn

; set_progress: ebx = control, eax = position, edx = maximum

set_progress:
	mov	[ebx+P_POS],eax
	mov	[ebx+P_MAX],edx
	jmp	control_redraw

; icon

icon_proc:
	cmp	eax,CM_PAINT
	jne	icon_done
	mov	eax,[px_face]
	call	fill_control
	mov	eax,[ebx+C_STYLE]
	shr	eax,24
	cmp	eax,ICON_ERROR
	je	error_icon
	cmp	eax,ICON_WARNING
	je	warning_icon
	mov	edx,[px_blue]
	mov	esi,_icon_information
	cmp	eax,ICON_QUESTION
	jne	draw_round_icon
	mov	esi,_icon_question
	jmp	draw_round_icon
    error_icon:
	mov	edx,[px_red]
	mov	esi,_icon_error
	jmp	draw_round_icon
    warning_icon:
	mov	edx,[px_yellow]
	mov	esi,_icon_warning
    draw_round_icon:
	push	ebx esi
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,edx
	call	ui_set_fill_color
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,31
	mov	edi,31
	call	x11_fill_arc
	pop	esi ebx
	mov	[ui_text_mnemonics],0
	mov	eax,[ui_gc_background]
	mov	[ui_text_background],eax
	call	ui_text_width
	mov	ecx,32
	sub	ecx,eax
	sar	ecx,1
	mov	edx,32
	sub	edx,[ui_font_height]
	sar	edx,1
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[px_white]
	call	ui_draw_text
	pop	ebx
    icon_done:
	retn

; scroll bar

scrollbar_proc:
	cmp	eax,CM_PAINT
	je	paint_scrollbar
	cmp	eax,CM_PRESS
	je	scrollbar_press
	cmp	eax,CM_DOUBLECLICK
	je	scrollbar_press
	cmp	eax,CM_MOTION
	je	scrollbar_motion
	cmp	eax,CM_RELEASE
	je	scrollbar_release
	cmp	eax,CM_TIMER
	je	scrollbar_repeat
	cmp	eax,CM_CREATE
	je	scrollbar_create
	retn
    scrollbar_create:
	mov	eax,[ebx+C_HEIGHT]
	cmp	eax,[ebx+C_WIDTH]
	seta	al
	movzx	eax,al
	mov	[ebx+S_VERTICAL],eax
	retn
    paint_scrollbar:
	call	scrollbar_geometry
	mov	eax,[ebx+C_WINDOW]
	push	ebx
	mov	ebx,[px_trough]
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,[ebx_control_width]
	mov	edi,[ebx_control_height]
	call	ui_fill
	pop	ebx
	xor	ecx,ecx
	call	paint_scroll_arrow
	mov	ecx,1
	call	paint_scroll_arrow
	cmp	[scroll_thumb_size],0
	je	scrollbar_painted
	mov	ecx,[scroll_thumb_start]
	mov	esi,[scroll_thumb_size]
	call	scroll_rectangle
	mov	eax,[ebx+C_WINDOW]
	push	ebx
	push	ecx edx esi edi
	mov	ebx,[px_face]
	call	ui_fill
	pop	edi esi edx ecx
	mov	ebx,FRAME_RAISED
	call	ui_frame
	pop	ebx
    scrollbar_painted:
	retn

; scrollbar_geometry: ebx = scroll bar, computes thumb position and size

scrollbar_geometry:
	mov	eax,[ebx+C_WIDTH]
	mov	[ebx_control_width],eax
	mov	edx,[ebx+C_HEIGHT]
	mov	[ebx_control_height],edx
	cmp	dword [ebx+S_VERTICAL],0
	jne	vertical_geometry
	xchg	eax,edx
    vertical_geometry:
	mov	[scroll_arrow_size],eax
	sub	edx,eax
	sub	edx,eax
	mov	[scroll_trough_size],edx
	mov	[scroll_thumb_size],0
	mov	ecx,[ebx+S_MAX]
	sub	ecx,[ebx+S_MIN]
	inc	ecx
	mov	[scroll_range],ecx
	cmp	ecx,[ebx+S_PAGE]
	jle	geometry_done
	cmp	edx,8
	jl	geometry_done
	mov	eax,edx
	mul	dword [ebx+S_PAGE]
	div	ecx
	cmp	eax,8
	jge	thumb_size_ok
	mov	eax,8
    thumb_size_ok:
	mov	[scroll_thumb_size],eax
	mov	ecx,[scroll_trough_size]
	sub	ecx,eax
	mov	[scroll_thumb_free],ecx
	mov	eax,[ebx+S_POS]
	cmp	dword [ebx+S_DRAG],0
	je	thumb_position_value
	mov	eax,[ebx+S_TRACK]
    thumb_position_value:
	sub	eax,[ebx+S_MIN]
	jns	thumb_offset_positive
	xor	eax,eax
    thumb_offset_positive:
	mul	ecx
	mov	ecx,[scroll_range]
	sub	ecx,[ebx+S_PAGE]
	jle	thumb_at_start
	div	ecx
	cmp	eax,[scroll_thumb_free]
	jbe	thumb_offset_ok
	mov	eax,[scroll_thumb_free]
	jmp	thumb_offset_ok
    thumb_at_start:
	xor	eax,eax
    thumb_offset_ok:
	add	eax,[scroll_arrow_size]
	mov	[scroll_thumb_start],eax
    geometry_done:
	retn

; scroll_rectangle: ebx = scroll bar, ecx = start along bar, esi = length
; returns ecx, edx, esi, edi = rectangle

scroll_rectangle:
	cmp	dword [ebx+S_VERTICAL],0
	je	horizontal_scroll_rectangle
	mov	edx,ecx
	xor	ecx,ecx
	mov	edi,esi
	mov	esi,[ebx+C_WIDTH]
	retn
    horizontal_scroll_rectangle:
	xor	edx,edx
	mov	edi,[ebx+C_HEIGHT]
	retn

; paint_scroll_arrow: ebx = scroll bar, ecx = 0 for first, 1 for second arrow

paint_scroll_arrow:
	mov	[scroll_arrow_index],ecx
	xor	eax,eax
	jecxz	arrow_start_ok
	mov	eax,[scroll_arrow_size]
	add	eax,[scroll_trough_size]
    arrow_start_ok:
	mov	ecx,eax
	mov	esi,[scroll_arrow_size]
	call	scroll_rectangle
	mov	[arrow_x],ecx
	mov	[arrow_y],edx
	mov	[arrow_width],esi
	mov	[arrow_height],edi
	mov	eax,[ebx+C_WINDOW]
	mov	[arrow_window],eax
	push	ebx
	mov	ebx,[px_face]
	call	ui_fill
	pop	ebx
	mov	eax,FRAME_RAISED
	cmp	[repeat_control],ebx
	jne	arrow_style_ok
	mov	edx,[ebx+S_REPEAT_CODE]
	cmp	edx,[scroll_arrow_index]
	jne	arrow_style_ok
	mov	eax,FRAME_PRESSED
    arrow_style_ok:
	push	ebx
	mov	ebx,eax
	mov	eax,[arrow_window]
	mov	ecx,[arrow_x]
	mov	edx,[arrow_y]
	mov	esi,[arrow_width]
	mov	edi,[arrow_height]
	call	ui_frame
	pop	ebx
	mov	ecx,[arrow_width]
	shr	ecx,1
	add	ecx,[arrow_x]
	mov	edx,[arrow_height]
	shr	edx,1
	add	edx,[arrow_y]
	mov	eax,[ebx+S_VERTICAL]
	shl	eax,1
	or	eax,[scroll_arrow_index]
	mov	[arrow_direction],eax
	push	ebx
	mov	ebx,[px_text]
	cmp	[scroll_thumb_size],0
	jne	arrow_color_ok
	mov	ebx,[px_gray]
    arrow_color_ok:
	call	draw_arrow_triangle
	pop	ebx
	retn

; draw_arrow_triangle: [arrow_window] = window, ecx, edx = center,
; ebx = pixel, [arrow_direction] = 0 left, 1 right, 2 up, 3 down

draw_arrow_triangle:
	push	ecx edx esi edi
	mov	[arrow_center_x],ecx
	mov	[arrow_center_y],edx
	xor	eax,eax
    triangle_line:
	mov	[triangle_step],eax
	mov	ecx,[arrow_direction]
	cmp	ecx,2
	jb	horizontal_arrow_line
	mov	edx,[arrow_center_y]
	cmp	ecx,2
	jne	down_arrow_line
	lea	edx,[edx+eax-2]
	jmp	arrow_line_y_ok
    down_arrow_line:
	add	edx,2
	sub	edx,eax
    arrow_line_y_ok:
	mov	edi,edx
	mov	ecx,[arrow_center_x]
	sub	ecx,eax
	mov	esi,[arrow_center_x]
	add	esi,eax
	jmp	draw_triangle_line
    horizontal_arrow_line:
	mov	ecx,[arrow_center_x]
	test	[arrow_direction],1
	jnz	right_arrow_line
	lea	ecx,[ecx+eax-2]
	jmp	arrow_line_x_ok
    right_arrow_line:
	add	ecx,2
	sub	ecx,eax
    arrow_line_x_ok:
	mov	esi,ecx
	mov	edx,[arrow_center_y]
	sub	edx,eax
	mov	edi,[arrow_center_y]
	add	edi,eax
    draw_triangle_line:
	mov	eax,[arrow_window]
	call	ui_line
	mov	eax,[triangle_step]
	inc	eax
	cmp	eax,4
	jb	triangle_line
	pop	edi esi edx ecx
	retn

; scroll bar interaction

    scrollbar_press:
	mov	eax,[cm_button]
	cmp	eax,1
	jne	scrollbar_wheel
	call	scrollbar_geometry
	mov	eax,[cm_y]
	cmp	dword [ebx+S_VERTICAL],0
	jne	scroll_press_position_ok
	mov	eax,[cm_x]
    scroll_press_position_ok:
	cmp	eax,[scroll_arrow_size]
	jl	scroll_line_up
	mov	ecx,[scroll_arrow_size]
	add	ecx,[scroll_trough_size]
	cmp	eax,ecx
	jge	scroll_line_down
	cmp	[scroll_thumb_size],0
	je	scrollbar_done
	cmp	eax,[scroll_thumb_start]
	jl	scroll_page_up
	mov	ecx,[scroll_thumb_start]
	add	ecx,[scroll_thumb_size]
	cmp	eax,ecx
	jge	scroll_page_down
	sub	eax,[scroll_thumb_start]
	mov	[ebx+S_DRAG_OFFSET],eax
	mov	dword [ebx+S_DRAG],1
	mov	eax,[ebx+S_POS]
	mov	[ebx+S_TRACK],eax
    scrollbar_done:
	retn
    scroll_line_up:
	mov	eax,SB_LINEUP
	jmp	scroll_repeat_start
    scroll_line_down:
	mov	eax,SB_LINEDOWN
	jmp	scroll_repeat_start
    scroll_page_up:
	mov	eax,SB_PAGEUP
	jmp	scroll_repeat_start
    scroll_page_down:
	mov	eax,SB_PAGEDOWN
    scroll_repeat_start:
	mov	[ebx+S_REPEAT_CODE],eax
	call	start_repeat
	call	control_redraw
	mov	eax,[ebx+S_REPEAT_CODE]
	jmp	scroll_notify
    scrollbar_repeat:
	mov	eax,[ebx+S_REPEAT_CODE]
	cmp	eax,-1
	je	scrollbar_done
	jmp	scroll_notify
    scrollbar_wheel:
	cmp	eax,4
	je	scroll_wheel_up
	cmp	eax,5
	jne	scrollbar_done
	mov	eax,SB_LINEDOWN
	jmp	scroll_wheel
    scroll_wheel_up:
	mov	eax,SB_LINEUP
    scroll_wheel:
	push	eax
	call	scroll_notify
	mov	eax,[esp]
	call	scroll_notify
	pop	eax
	jmp	scroll_notify
    scrollbar_motion:
	cmp	dword [ebx+S_DRAG],0
	je	scrollbar_done
	call	scrollbar_geometry
	mov	eax,[cm_y]
	cmp	dword [ebx+S_VERTICAL],0
	jne	drag_position_ok
	mov	eax,[cm_x]
    drag_position_ok:
	sub	eax,[ebx+S_DRAG_OFFSET]
	sub	eax,[scroll_arrow_size]
	jns	drag_offset_positive
	xor	eax,eax
    drag_offset_positive:
	mov	ecx,[scroll_thumb_free]
	test	ecx,ecx
	jz	scrollbar_done
	cmp	eax,ecx
	jbe	drag_offset_ok
	mov	eax,ecx
    drag_offset_ok:
	mov	edx,[scroll_range]
	sub	edx,[ebx+S_PAGE]
	mul	edx
	div	ecx
	add	eax,[ebx+S_MIN]
	cmp	eax,[ebx+S_TRACK]
	je	scrollbar_done
	mov	[ebx+S_TRACK],eax
	call	control_redraw
	mov	edx,[ebx+S_TRACK]
	mov	eax,SB_THUMBTRACK
	jmp	scroll_notify
    scrollbar_release:
	cmp	dword [ebx+S_DRAG],0
	je	scroll_repeat_released
	mov	dword [ebx+S_DRAG],0
	mov	eax,[ebx+S_TRACK]
	mov	[ebx+S_POS],eax
	jmp	control_redraw
    scroll_repeat_released:
	mov	dword [ebx+S_REPEAT_CODE],-1
	jmp	control_redraw

; scroll_notify: ebx = scroll bar, eax = code, edx = track position

scroll_notify:
	mov	[scroll_track_position],edx
	mov	edx,eax
	jmp	control_notify

; set_scroll_info: ebx = scroll bar, eax = minimum, ecx = maximum,
; edx = page, esi = position

set_scroll_info:
	test	ebx,ebx
	jz	scroll_info_set
	cmp	dword [ebx+S_DRAG],0
	jne	scroll_info_set
	cmp	eax,[ebx+S_MIN]
	jne	update_scroll_info
	cmp	ecx,[ebx+S_MAX]
	jne	update_scroll_info
	cmp	edx,[ebx+S_PAGE]
	jne	update_scroll_info
	cmp	esi,[ebx+S_POS]
	je	scroll_info_set
    update_scroll_info:
	mov	[ebx+S_MIN],eax
	mov	[ebx+S_MAX],ecx
	mov	[ebx+S_PAGE],edx
	mov	[ebx+S_POS],esi
	mov	[ebx+S_TRACK],esi
	call	control_redraw
    scroll_info_set:
	retn

; edit control

EDIT_CAPACITY = 4096

edit_proc:
	cmp	eax,CM_PAINT
	je	paint_edit
	cmp	eax,CM_KEY
	je	edit_key
	cmp	eax,CM_PRESS
	je	edit_press
	cmp	eax,CM_MOTION
	je	edit_motion
	cmp	eax,CM_RELEASE
	je	edit_release
	cmp	eax,CM_DOUBLECLICK
	je	edit_double_click
	cmp	eax,CM_FOCUS
	je	edit_focus
	cmp	eax,CM_BLINK
	je	edit_blink
	cmp	eax,CM_CREATE
	je	edit_create
	cmp	eax,CM_DESTROY
	je	edit_destroy
	cmp	eax,CM_MOVE
	je	edit_layout
	retn
    edit_create:
	push	ebx
	mov	ecx,EDIT_CAPACITY*4
	call	get_memory
	pop	ebx
	mov	[ebx+E_CELLS],eax
	mov	dword [ebx+E_CAPACITY],EDIT_CAPACITY
	call	edit_layout
	mov	esi,[ebx+C_TEXT]
	jmp	edit_set_text_only
    edit_destroy:
	push	ebx
	mov	ebx,[ebx+E_CELLS]
	call	release_memory
	pop	ebx
	retn
    edit_layout:
	mov	eax,[ebx+C_WIDTH]
	cmp	dword [ebx+C_TYPE],CT_COMBOBOX
	je	combo_edit_layout
	test	dword [ebx+C_STYLE],WS_BORDER
	jz	borderless_edit_layout
	mov	dword [ebx+E_TEXT_X],3
	sub	eax,6
	jmp	edit_layout_done
    combo_edit_layout:
	mov	dword [ebx+E_TEXT_X],3
	sub	eax,6+SCROLLBAR_SIZE
	jmp	edit_layout_done
    borderless_edit_layout:
	mov	dword [ebx+E_TEXT_X],1
	sub	eax,2
    edit_layout_done:
	jns	edit_text_width_ok
	xor	eax,eax
    edit_text_width_ok:
	mov	[ebx+E_TEXT_WIDTH],eax
	retn

; edit_set_text: ebx = edit control (or editable combo box), esi = UTF-8 text

edit_set_text:
	call	edit_set_text_only
	call	control_redraw
	retn
    edit_set_text_only:
	push	eax ecx esi edi
	mov	edi,[ebx+E_CELLS]
	mov	ecx,[ebx+E_CAPACITY]
	dec	ecx
	xor	edx,edx
    store_edit_characters:
	call	utf8_decode_character
	test	eax,eax
	jz	edit_characters_stored
	cmp	eax,0Ah
	je	edit_characters_stored
	cmp	eax,0Dh
	je	edit_characters_stored
	stosd
	inc	edx
	loop	store_edit_characters
    edit_characters_stored:
	mov	[ebx+E_LENGTH],edx
	xor	eax,eax
	mov	[ebx+E_CARET],eax
	mov	[ebx+E_ANCHOR],eax
	mov	[ebx+E_SCROLL],eax
	pop	edi esi ecx eax
	retn

; edit_get_text: ebx = edit control, edi = buffer, ecx = size
; returns eax = length of UTF-8 text

edit_get_text:
	push	ecx esi edi
	lea	edx,[edi+ecx-5]
	mov	esi,[ebx+E_CELLS]
	mov	ecx,[ebx+E_LENGTH]
	jecxz	edit_text_copied
    copy_edit_cell:
	cmp	edi,edx
	jae	edit_text_copied
	lodsd
	call	utf8_encode
	loop	copy_edit_cell
    edit_text_copied:
	mov	byte [edi],0
	mov	eax,edi
	pop	edi esi ecx
	sub	eax,edi
	retn

; edit_get_cells: ebx = edit control, edi = buffer for zero-terminated cells

edit_get_cells:
	push	ecx esi edi
	mov	esi,[ebx+E_CELLS]
	mov	ecx,[ebx+E_LENGTH]
	rep	movsd
	xor	eax,eax
	stosd
	pop	edi esi ecx
	retn

    paint_edit:
	mov	eax,[px_window]
	test	dword [ebx+C_STYLE],ES_READONLY
	jnz	edit_face_background
	test	dword [ebx+C_STATE],CS_DISABLED
	jz	edit_background_ok
    edit_face_background:
	mov	eax,[px_face]
    edit_background_ok:
	mov	[edit_background],eax
	call	fill_control
	test	dword [ebx+C_STYLE],WS_BORDER
	jz	paint_edit_text
	mov	eax,FRAME_SUNKEN
	call	frame_control

; paint_edit_text: ebx = control, [edit_background] = background pixel

paint_edit_text:
	mov	eax,[ebx+E_TEXT_WIDTH]
	xor	edx,edx
	div	[ui_font_width]
	mov	[edit_visible],eax
	mov	eax,[ebx+C_HEIGHT]
	sub	eax,[ui_font_height]
	sar	eax,1
	mov	[edit_text_y],eax
	mov	eax,[ebx+E_ANCHOR]
	mov	edx,[ebx+E_CARET]
	cmp	eax,edx
	jbe	edit_selection_ordered
	xchg	eax,edx
    edit_selection_ordered:
	test	dword [ebx+C_STATE],CS_FOCUSED
	jnz	edit_selection_ok
	mov	edx,eax
    edit_selection_ok:
	mov	[edit_selection_start],eax
	mov	[edit_selection_end],edx
	mov	eax,[ebx+E_SCROLL]
	mov	edx,eax
	add	edx,[edit_visible]
	cmp	edx,[ebx+E_LENGTH]
	jbe	edit_visible_end_ok
	mov	edx,[ebx+E_LENGTH]
    edit_visible_end_ok:
	mov	[edit_visible_end],edx
	mov	edx,[edit_selection_start]
	cmp	edx,[edit_visible_end]
	jbe	first_edit_run_end_ok
	mov	edx,[edit_visible_end]
    first_edit_run_end_ok:
	xor	ecx,ecx
	call	draw_edit_run
	mov	eax,[edit_selection_start]
	cmp	eax,[ebx+E_SCROLL]
	jae	selected_run_start_ok
	mov	eax,[ebx+E_SCROLL]
    selected_run_start_ok:
	mov	edx,[edit_selection_end]
	cmp	edx,[edit_visible_end]
	jbe	selected_run_end_ok
	mov	edx,[edit_visible_end]
    selected_run_end_ok:
	mov	ecx,1
	call	draw_edit_run
	mov	eax,[edit_selection_end]
	cmp	eax,[ebx+E_SCROLL]
	jae	last_edit_run_start_ok
	mov	eax,[ebx+E_SCROLL]
    last_edit_run_start_ok:
	mov	edx,[edit_visible_end]
	xor	ecx,ecx
	call	draw_edit_run
	test	dword [ebx+C_STATE],CS_FOCUSED
	jz	edit_painted
	cmp	dword [edit_caret_on],0
	je	edit_painted
	mov	eax,[ebx+E_CARET]
	sub	eax,[ebx+E_SCROLL]
	jb	edit_painted
	cmp	eax,[edit_visible]
	ja	edit_painted
	imul	eax,[ui_font_width]
	add	eax,[ebx+E_TEXT_X]
	mov	ecx,eax
	mov	esi,eax
	mov	edx,[edit_text_y]
	mov	edi,edx
	add	edi,[ui_font_height]
	dec	edi
	mov	eax,[ebx+C_WINDOW]
	push	ebx
	mov	ebx,[px_text]
	call	ui_line
	pop	ebx
    edit_painted:
	retn

; draw_edit_run: ebx = control, eax = first cell, edx = end, ecx = 1 if selected

draw_edit_run:
	cmp	eax,edx
	jae	edit_run_drawn
	push	eax edx
	mov	[edit_run_start],eax
	sub	edx,eax
	mov	[edit_run_length],edx
	push	ebx
	test	ecx,ecx
	jz	edit_run_normal_colors
	mov	ecx,[px_selection_text]
	mov	edx,[px_selection]
	jmp	edit_run_colors_ok
    edit_run_normal_colors:
	call	control_text_color
	mov	edx,[edit_background]
    edit_run_colors_ok:
	call	ui_set_colors
	pop	ebx
	mov	esi,[ebx+E_CELLS]
	mov	eax,[edit_run_start]
	lea	esi,[esi+eax*4]
	mov	edi,char2b_buffer
	mov	ecx,[edit_run_length]
	cmp	ecx,255
	jbe	edit_run_length_ok
	mov	ecx,255
	mov	[edit_run_length],ecx
    edit_run_length_ok:
	lodsd
	call	ui_glyph
	xchg	al,ah
	stosw
	loop	edit_run_length_ok
	push	ebx
	mov	eax,[edit_run_start]
	sub	eax,[ebx+E_SCROLL]
	imul	eax,[ui_font_width]
	add	eax,[ebx+E_TEXT_X]
	mov	ecx,eax
	mov	edx,[edit_text_y]
	add	edx,[ui_font_ascent]
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[ui_gc]
	mov	esi,char2b_buffer
	mov	edi,[edit_run_length]
	call	x11_image_text16
	pop	ebx
	pop	edx eax
    edit_run_drawn:
	retn

    edit_blink:
	xor	dword [edit_caret_on],1
	jmp	control_redraw
    edit_focus:
	mov	dword [edit_caret_on],1
	cmp	[keyboard_focus_change],0
	je	edit_focus_done
	call	edit_select_all
    edit_focus_done:
	retn
    edit_double_click:
	call	edit_select_all
	jmp	control_redraw

edit_select_all:
	mov	dword [ebx+E_ANCHOR],0
	mov	eax,[ebx+E_LENGTH]
	mov	[ebx+E_CARET],eax
	jmp	edit_ensure_visible

; edit_ensure_visible: ebx = control

edit_ensure_visible:
	mov	eax,[ebx+E_TEXT_WIDTH]
	xor	edx,edx
	div	[ui_font_width]
	dec	eax
	jns	edit_visible_count_ok
	xor	eax,eax
    edit_visible_count_ok:
	mov	edx,[ebx+E_CARET]
	cmp	edx,[ebx+E_SCROLL]
	jae	caret_after_scroll
	mov	[ebx+E_SCROLL],edx
	retn
    caret_after_scroll:
	sub	edx,eax
	jbe	edit_visible_ok
	cmp	edx,[ebx+E_SCROLL]
	jbe	edit_visible_ok
	mov	[ebx+E_SCROLL],edx
    edit_visible_ok:
	retn

; edit_position_from_pointer: ebx = control, returns eax = cell index

edit_position_from_pointer:
	mov	eax,[cm_x]
	sub	eax,[ebx+E_TEXT_X]
	mov	edx,[ui_font_width]
	shr	edx,1
	add	eax,edx
	cdq
	idiv	[ui_font_width]
	add	eax,[ebx+E_SCROLL]
	jns	pointer_position_positive
	xor	eax,eax
    pointer_position_positive:
	cmp	eax,[ebx+E_LENGTH]
	jbe	pointer_position_ok
	mov	eax,[ebx+E_LENGTH]
    pointer_position_ok:
	retn

    edit_press:
	cmp	[cm_button],1
	jne	edit_pointer_done
	call	edit_position_from_pointer
	mov	[ebx+E_CARET],eax
	test	[cm_state],X11_ShiftMask
	jnz	edit_press_anchor_ok
	mov	[ebx+E_ANCHOR],eax
    edit_press_anchor_ok:
	mov	dword [ebx+E_DRAG],1
	mov	dword [edit_caret_on],1
	call	edit_ensure_visible
	jmp	control_redraw
    edit_motion:
	cmp	dword [ebx+E_DRAG],0
	je	edit_pointer_done
	call	edit_position_from_pointer
	cmp	eax,[ebx+E_CARET]
	je	edit_pointer_done
	mov	[ebx+E_CARET],eax
	call	edit_ensure_visible
	jmp	control_redraw
    edit_release:
	mov	dword [ebx+E_DRAG],0
    edit_pointer_done:
	retn

    edit_key:
	mov	dword [edit_caret_on],1
	mov	eax,[cm_keysym]
	test	[cm_flags],KS_CONTROL
	jz	edit_plain_key
	mov	eax,[cm_base_keysym]
	or	eax,20h
	cmp	eax,'a'
	je	edit_key_select_all
	cmp	eax,'c'
	je	edit_copy
	cmp	eax,'x'
	je	edit_cut
	cmp	eax,'v'
	je	edit_paste
	mov	eax,[cm_keysym]
	cmp	eax,XK_Insert
	je	edit_copy
	cmp	eax,XK_Left
	je	edit_left
	cmp	eax,XK_Right
	je	edit_right
	cmp	eax,XK_Home
	je	edit_home
	cmp	eax,XK_End
	je	edit_end
	retn
    edit_key_select_all:
	call	edit_select_all
	jmp	control_redraw
    edit_plain_key:
	cmp	eax,XK_Left
	je	edit_left
	cmp	eax,XK_Right
	je	edit_right
	cmp	eax,XK_Home
	je	edit_home
	cmp	eax,XK_End
	je	edit_end
	cmp	eax,XK_BackSpace
	je	edit_backspace
	cmp	eax,XK_Delete
	je	edit_delete
	cmp	eax,XK_Insert
	jne	edit_character
	test	[cm_flags],KS_SHIFT
	jnz	edit_paste
	retn
    edit_character:
	test	[cm_flags],KS_ALT
	jnz	edit_key_done
	mov	eax,[cm_character]
	cmp	eax,20h
	jb	edit_key_done
	test	dword [ebx+C_STYLE],ES_READONLY
	jnz	edit_key_done
	test	dword [ebx+C_STYLE],ES_NUMBER
	jz	edit_character_ok
	cmp	eax,'0'
	jb	edit_rejected
	cmp	eax,'9'
	ja	edit_rejected
    edit_character_ok:
	mov	[edit_inserted_cell],eax
	mov	esi,edit_inserted_cell
	mov	ecx,1
	call	edit_insert_cells
	jmp	edit_changed
    edit_rejected:
	call	x11_bell
    edit_key_done:
	retn
    edit_left:
	mov	eax,[ebx+E_CARET]
	test	eax,eax
	jz	edit_moved
	dec	eax
	jmp	edit_moved
    edit_right:
	mov	eax,[ebx+E_CARET]
	cmp	eax,[ebx+E_LENGTH]
	jae	edit_moved
	inc	eax
	jmp	edit_moved
    edit_home:
	xor	eax,eax
	jmp	edit_moved
    edit_end:
	mov	eax,[ebx+E_LENGTH]
    edit_moved:
	mov	[ebx+E_CARET],eax
	test	[cm_flags],KS_SHIFT
	jnz	edit_moved_selection
	mov	[ebx+E_ANCHOR],eax
    edit_moved_selection:
	call	edit_ensure_visible
	jmp	control_redraw
    edit_backspace:
	test	dword [ebx+C_STYLE],ES_READONLY
	jnz	edit_key_done
	mov	eax,[ebx+E_ANCHOR]
	cmp	eax,[ebx+E_CARET]
	jne	edit_delete_selection
	test	eax,eax
	jz	edit_key_done
	dec	dword [ebx+E_ANCHOR]
	jmp	edit_delete_selection
    edit_delete:
	test	dword [ebx+C_STYLE],ES_READONLY
	jnz	edit_key_done
	test	[cm_flags],KS_SHIFT
	jnz	edit_cut
	mov	eax,[ebx+E_ANCHOR]
	cmp	eax,[ebx+E_CARET]
	jne	edit_delete_selection
	cmp	eax,[ebx+E_LENGTH]
	jae	edit_key_done
	inc	dword [ebx+E_ANCHOR]
    edit_delete_selection:
	call	edit_delete_selected
	jmp	edit_changed
    edit_copy:
	call	edit_copy_selection
	retn
    edit_cut:
	test	dword [ebx+C_STYLE],ES_READONLY
	jnz	edit_copy
	call	edit_copy_selection
	mov	eax,[ebx+E_ANCHOR]
	cmp	eax,[ebx+E_CARET]
	je	edit_key_done
	jmp	edit_delete_selection
    edit_paste:
	test	dword [ebx+C_STYLE],ES_READONLY
	jnz	edit_key_done
	push	ebx
	call	clipboard_get_cells
	pop	ebx
	test	eax,eax
	jz	edit_key_done
	mov	[edit_paste_memory],eax
	mov	esi,eax
	xor	ecx,ecx
    count_pasted_cells:
	mov	eax,[esi+ecx*4]
	test	eax,eax
	jz	pasted_cells_counted
	cmp	eax,0Ah
	je	pasted_cells_counted
	cmp	eax,0Dh
	je	pasted_cells_counted
	cmp	eax,9
	jne	pasted_cell_ok
	mov	dword [esi+ecx*4],20h
    pasted_cell_ok:
	inc	ecx
	jmp	count_pasted_cells
    pasted_cells_counted:
	call	edit_insert_cells
	push	ebx
	mov	ebx,[edit_paste_memory]
	call	release_memory
	pop	ebx
    edit_changed:
	call	edit_ensure_visible
	call	control_redraw
	mov	edx,EN_CHANGE
	cmp	dword [ebx+C_TYPE],CT_COMBOBOX
	jne	edit_notify
	mov	edx,CBN_EDITCHANGE
    edit_notify:
	jmp	control_notify

; edit_copy_selection: ebx = edit control

edit_copy_selection:
	mov	eax,[ebx+E_ANCHOR]
	mov	edx,[ebx+E_CARET]
	cmp	eax,edx
	je	edit_nothing_copied
	jb	edit_copy_range_ok
	xchg	eax,edx
    edit_copy_range_ok:
	sub	edx,eax
	mov	esi,[ebx+E_CELLS]
	lea	esi,[esi+eax*4]
	mov	ecx,edx
	push	ebx
	call	clipboard_set_cells
	pop	ebx
    edit_nothing_copied:
	retn

; edit_delete_selected: ebx = edit control

edit_delete_selected:
	mov	eax,[ebx+E_ANCHOR]
	mov	edx,[ebx+E_CARET]
	cmp	eax,edx
	je	nothing_selected_to_delete
	jb	delete_range_ordered
	xchg	eax,edx
    delete_range_ordered:
	mov	edi,[ebx+E_CELLS]
	lea	esi,[edi+edx*4]
	lea	edi,[edi+eax*4]
	mov	ecx,[ebx+E_LENGTH]
	sub	ecx,edx
	sub	edx,eax
	sub	[ebx+E_LENGTH],edx
	rep	movsd
	mov	[ebx+E_CARET],eax
	mov	[ebx+E_ANCHOR],eax
    nothing_selected_to_delete:
	retn

; edit_insert_cells: ebx = edit control, esi = cells, ecx = count

edit_insert_cells:
	push	esi ecx
	call	edit_delete_selected
	pop	ecx esi
	mov	eax,[ebx+E_CAPACITY]
	dec	eax
	sub	eax,[ebx+E_LENGTH]
	cmp	ecx,eax
	jbe	inserted_count_ok
	mov	ecx,eax
    inserted_count_ok:
	jecxz	cells_inserted
	push	esi ecx
	mov	edx,[ebx+E_CELLS]
	mov	eax,[ebx+E_LENGTH]
	lea	esi,[edx+eax*4-4]
	lea	edi,[esi+ecx*4]
	mov	ecx,eax
	sub	ecx,[ebx+E_CARET]
	std
	rep	movsd
	cld
	pop	ecx esi
	mov	eax,[ebx+E_CARET]
	lea	edi,[edx+eax*4]
	add	[ebx+E_LENGTH],ecx
	add	[ebx+E_CARET],ecx
	rep	movsd
	mov	eax,[ebx+E_CARET]
	mov	[ebx+E_ANCHOR],eax
    cells_inserted:
	retn

; list box

LIST_ITEM_SIZE = 12
LIST_INITIAL_ITEMS = 400h
LIST_INITIAL_STRINGS = 10000h

listbox_proc:
	cmp	eax,CM_PAINT
	je	paint_listbox
	cmp	eax,CM_PRESS
	je	listbox_press
	cmp	eax,CM_MOTION
	je	listbox_motion
	cmp	eax,CM_RELEASE
	je	listbox_release
	cmp	eax,CM_DOUBLECLICK
	je	listbox_double_click
	cmp	eax,CM_KEY
	je	listbox_key
	cmp	eax,CM_WHEEL
	je	listbox_wheel
	cmp	eax,CM_CREATE
	je	listbox_create
	cmp	eax,CM_DESTROY
	je	listbox_destroy
	retn
    listbox_create:
	call	listbox_init
	push	ebx
	mov	[new_control_type],CT_SCROLLBAR
	mov	[new_control_id],0
	mov	eax,[ebx+C_WIDTH]
	sub	eax,SCROLLBAR_SIZE+2
	mov	[new_control_x],eax
	mov	[new_control_y],2
	mov	[new_control_width],SCROLLBAR_SIZE
	mov	eax,[ebx+C_HEIGHT]
	sub	eax,4
	mov	[new_control_height],eax
	mov	[new_control_style],0
	mov	[new_control_text],0
	mov	eax,[ebx+C_WINDOW]
	mov	[new_control_parent],eax
	mov	[new_control_notify],listbox_scrolled
	mov	[new_control_notify_data],ebx
	mov	eax,[building_dialog]
	push	eax
	mov	eax,[ebx+C_DIALOG]
	mov	[building_dialog],eax
	call	create_control
	pop	eax
	mov	[building_dialog],eax
	mov	eax,ebx
	pop	ebx
	mov	[ebx+L_SCROLLBAR],eax
	jmp	listbox_layout
    listbox_destroy:
	mov	eax,[ebx+L_SCROLLBAR]
	test	eax,eax
	jz	listbox_memory_release
	push	ebx
	mov	ebx,eax
	call	destroy_control
	pop	ebx
    listbox_memory_release:
	push	ebx
	mov	eax,[ebx+L_ITEMS]
	mov	ebx,[ebx+L_STRINGS]
	call	release_memory
	mov	ebx,eax
	call	release_memory
	pop	ebx
	retn

; listbox_init: ebx = list control, prepares storage of items

listbox_init:
	push	ebx
	mov	ecx,LIST_INITIAL_STRINGS
	call	get_memory
	pop	ebx
	mov	[ebx+L_STRINGS],eax
	mov	dword [ebx+L_STRINGS_SIZE],LIST_INITIAL_STRINGS
	push	ebx
	mov	ecx,LIST_INITIAL_ITEMS*LIST_ITEM_SIZE
	call	get_memory
	pop	ebx
	mov	[ebx+L_ITEMS],eax
	mov	dword [ebx+L_ITEMS_SIZE],LIST_INITIAL_ITEMS
	xor	eax,eax
	mov	[ebx+L_STRINGS_USED],eax
	mov	[ebx+L_COUNT],eax
	mov	[ebx+L_TOP],eax
	mov	[ebx+L_ANCHOR],eax
	dec	eax
	mov	[ebx+L_SELECTION],eax
	mov	eax,[ui_font_height]
	add	eax,2
	mov	[ebx+L_ITEM_HEIGHT],eax

; listbox_layout: ebx = list control, computes number of visible rows

listbox_layout:
	mov	eax,[ebx+C_HEIGHT]
	sub	eax,4
	cmp	dword [ebx+L_COMBO],0
	je	list_layout_height_ok
	add	eax,2
    list_layout_height_ok:
	xor	edx,edx
	div	dword [ebx+L_ITEM_HEIGHT]
	test	eax,eax
	jnz	list_visible_ok
	inc	eax
    list_visible_ok:
	mov	[ebx+L_VISIBLE],eax
	retn

; list_add_string: ebx = list, esi = UTF-8 string, returns eax = index

list_add_string:
	mov	eax,[ebx+L_COUNT]

; list_insert_string: ebx = list, eax = index, esi = string

list_insert_string:
	push	ecx edx esi edi
	cmp	eax,[ebx+L_COUNT]
	jbe	list_insert_index_ok
	mov	eax,[ebx+L_COUNT]
    list_insert_index_ok:
	push	eax
	call	string_length
	inc	ecx
	mov	eax,[ebx+L_STRINGS_USED]
	add	eax,ecx
	cmp	eax,[ebx+L_STRINGS_SIZE]
	jbe	list_strings_space_ok
	push	ebx ecx esi
	mov	ecx,[ebx+L_STRINGS_SIZE]
	add	ecx,ecx
	add	ecx,eax
	; Commit the proposed capacity only after allocation succeeds.
	push	ecx
	mov	eax,ebx
	mov	ebx,[eax+L_STRINGS]
	push	eax
	call	resize_memory
	pop	edx
	pop	ecx
	test	eax,eax
	jz	list_resize_failed
	mov	[edx+L_STRINGS],eax
	mov	[edx+L_STRINGS_SIZE],ecx
	pop	esi ecx ebx
	jmp	list_strings_space_ok
    list_resize_failed:
	pop	esi ecx ebx
	pop	eax
	or	eax,-1
	pop	edi esi edx ecx
	retn
    list_strings_space_ok:
	mov	eax,[ebx+L_COUNT]
	cmp	eax,[ebx+L_ITEMS_SIZE]
	jb	list_items_space_ok
	push	ebx ecx esi
	mov	ecx,[ebx+L_ITEMS_SIZE]
	add	ecx,ecx
	push	ecx
	imul	ecx,LIST_ITEM_SIZE
	mov	eax,ebx
	mov	ebx,[eax+L_ITEMS]
	push	eax
	call	resize_memory
	pop	edx
	pop	ecx
	test	eax,eax
	jz	list_resize_failed
	mov	[edx+L_ITEMS],eax
	mov	[edx+L_ITEMS_SIZE],ecx
	pop	esi ecx ebx
    list_items_space_ok:
	mov	edi,[ebx+L_STRINGS]
	add	edi,[ebx+L_STRINGS_USED]
	mov	edx,[ebx+L_STRINGS_USED]
	add	[ebx+L_STRINGS_USED],ecx
	rep	movsb
	pop	eax
	push	eax
	mov	esi,[ebx+L_ITEMS]
	mov	ecx,[ebx+L_COUNT]
	sub	ecx,eax
	imul	ecx,LIST_ITEM_SIZE
	imul	eax,LIST_ITEM_SIZE
	add	esi,eax
	push	esi
	mov	edi,esi
	add	edi,LIST_ITEM_SIZE
	add	esi,ecx
	add	edi,ecx
	sub	esi,4
	sub	edi,4
	shr	ecx,2
	std
	rep	movsd
	cld
	pop	edi
	mov	[edi],edx
	mov	dword [edi+4],0
	mov	dword [edi+8],0
	inc	dword [ebx+L_COUNT]
	pop	eax
	mov	edx,[ebx+L_SELECTION]
	cmp	edx,eax
	jl	list_selection_after_insert_ok
	inc	dword [ebx+L_SELECTION]
    list_selection_after_insert_ok:
	pop	edi esi edx ecx
	retn

; list_delete_string: ebx = list, eax = index

list_delete_string:
	cmp	eax,[ebx+L_COUNT]
	jae	list_string_deleted
	push	ecx esi edi
	mov	edi,[ebx+L_ITEMS]
	imul	ecx,eax,LIST_ITEM_SIZE
	add	edi,ecx
	lea	esi,[edi+LIST_ITEM_SIZE]
	mov	ecx,[ebx+L_COUNT]
	sub	ecx,eax
	dec	ecx
	imul	ecx,LIST_ITEM_SIZE/4
	rep	movsd
	dec	dword [ebx+L_COUNT]
	cmp	eax,[ebx+L_SELECTION]
	jg	list_deleted_selection_ok
	jl	list_selection_moved
	or	dword [ebx+L_SELECTION],-1
	jmp	list_deleted_selection_ok
    list_selection_moved:
	dec	dword [ebx+L_SELECTION]
    list_deleted_selection_ok:
	pop	edi esi ecx
    list_string_deleted:
	retn

; list_reset: ebx = list

list_reset:
	xor	eax,eax
	mov	[ebx+L_COUNT],eax
	mov	[ebx+L_STRINGS_USED],eax
	mov	[ebx+L_TOP],eax
	mov	[ebx+L_ANCHOR],eax
	dec	eax
	mov	[ebx+L_SELECTION],eax
	retn

; list_get_string: ebx = list, eax = index, returns CF=0 and esi = string

list_get_string:
	cmp	eax,[ebx+L_COUNT]
	jae	list_index_invalid
	push	eax
	imul	eax,LIST_ITEM_SIZE
	add	eax,[ebx+L_ITEMS]
	mov	esi,[eax]
	add	esi,[ebx+L_STRINGS]
	pop	eax
	clc
	retn
    list_index_invalid:
	stc
	retn

; list_item: ebx = list, eax = index, returns edx = address of item

list_item:
	imul	edx,eax,LIST_ITEM_SIZE
	add	edx,[ebx+L_ITEMS]
	retn

; list_find_string: ebx = list, esi = string, returns eax = index (-1 if none)

list_find_string:
	push	ecx edx esi edi
	mov	edi,esi
	xor	ecx,ecx
    find_list_string:
	cmp	ecx,[ebx+L_COUNT]
	jae	list_string_not_found
	mov	eax,ecx
	call	list_get_string
	call	compare_strings_nocase
	je	list_string_found
	inc	ecx
	jmp	find_list_string
    list_string_not_found:
	or	ecx,-1
    list_string_found:
	mov	eax,ecx
	pop	edi esi edx ecx
	retn

; list_set_selection: ebx = list, eax = index

list_set_selection:
	mov	[ebx+L_SELECTION],eax
	mov	[ebx+L_ANCHOR],eax
	test	dword [ebx+C_STYLE],LBS_EXTENDEDSEL
	jz	list_selection_visible
	call	list_clear_marks
	cmp	eax,[ebx+L_COUNT]
	jae	list_selection_visible
	call	list_item
	or	dword [edx+8],1
    list_selection_visible:
	call	list_ensure_visible
	cmp	dword [ebx+C_WINDOW],0
	je	list_selection_set
	call	control_redraw
    list_selection_set:
	retn

; list_clear_marks: ebx = list (extended selection)

list_clear_marks:
	push	eax ecx edx
	xor	eax,eax
    clear_list_mark:
	cmp	eax,[ebx+L_COUNT]
	jae	list_marks_cleared
	call	list_item
	and	dword [edx+8],0
	inc	eax
	jmp	clear_list_mark
    list_marks_cleared:
	pop	edx ecx eax
	retn

; list_ensure_visible: ebx = list

list_ensure_visible:
	push	eax
	mov	eax,[ebx+L_SELECTION]
	cmp	eax,0
	jl	list_visible_done
	cmp	eax,[ebx+L_TOP]
	jge	list_selection_below_top
	mov	[ebx+L_TOP],eax
	jmp	list_visible_done
    list_selection_below_top:
	sub	eax,[ebx+L_VISIBLE]
	inc	eax
	cmp	eax,[ebx+L_TOP]
	jle	list_visible_done
	mov	[ebx+L_TOP],eax
    list_visible_done:
	pop	eax
	retn

; list_is_selected: ebx = list, eax = index, returns CF=1 if selected

list_is_selected:
	push	edx
	test	dword [ebx+C_STYLE],LBS_EXTENDEDSEL
	jnz	list_marked_selection
	cmp	eax,[ebx+L_SELECTION]
	je	list_item_selected
	pop	edx
	clc
	retn
    list_marked_selection:
	call	list_item
	test	dword [edx+8],1
	jnz	list_item_selected
	pop	edx
	clc
	retn
    list_item_selected:
	pop	edx
	stc
	retn

    paint_listbox:
	cmp	dword [ebx+C_WINDOW],0
	je	listbox_painted
	mov	eax,[px_window]
	call	fill_control
	mov	eax,FRAME_SUNKEN
	mov	ecx,2
	cmp	dword [ebx+L_COMBO],0
	je	list_frame_style_ok
	mov	eax,FRAME_BLACK
	mov	ecx,1
    list_frame_style_ok:
	mov	[list_border],ecx
	call	frame_control
	mov	eax,[ebx+C_WIDTH]
	sub	eax,ecx
	sub	eax,ecx
	cmp	dword [ebx+L_SCROLLBAR],0
	je	list_row_width_ok
	sub	eax,SCROLLBAR_SIZE
    list_row_width_ok:
	mov	[list_row_width],eax
	xor	ecx,ecx
    paint_list_row:
	cmp	ecx,[ebx+L_VISIBLE]
	jae	list_rows_painted
	mov	eax,[ebx+L_TOP]
	add	eax,ecx
	cmp	eax,[ebx+L_COUNT]
	jae	list_rows_painted
	push	ecx
	mov	edx,ecx
	imul	edx,[ebx+L_ITEM_HEIGHT]
	add	edx,[list_border]
	mov	[list_row_y],edx
	call	list_is_selected
	mov	ecx,[px_text]
	mov	edx,[px_window]
	jnc	list_row_colors_ok
	mov	ecx,[px_selection_text]
	mov	edx,[px_selection]
    list_row_colors_ok:
	mov	[ui_text_background],edx
	mov	[list_row_color],ecx
	push	eax
	cmp	edx,[px_window]
	je	list_row_background_ok
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,edx
	mov	ecx,[list_border]
	mov	edx,[list_row_y]
	mov	esi,[list_row_width]
	mov	edi,[esp]
	mov	edi,[edi+L_ITEM_HEIGHT]
	call	ui_fill
	pop	ebx
    list_row_background_ok:
	pop	eax
	call	list_get_string
	mov	[ui_text_mnemonics],0
	mov	ecx,[list_border]
	add	ecx,2
	mov	edx,[list_row_y]
	inc	edx
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	ebx,[list_row_color]
	call	ui_draw_text
	pop	ebx
	pop	ecx
	inc	ecx
	jmp	paint_list_row
    list_rows_painted:
	call	listbox_update_scrollbar
    listbox_painted:
	retn

; listbox_update_scrollbar: ebx = list

listbox_update_scrollbar:
	push	ebx
	mov	esi,[ebx+L_TOP]
	mov	edx,[ebx+L_VISIBLE]
	mov	ecx,[ebx+L_COUNT]
	dec	ecx
	mov	ebx,[ebx+L_SCROLLBAR]
	xor	eax,eax
	call	set_scroll_info
	pop	ebx
	retn

; listbox_scrolled: ebx = scroll bar, eax = scroll code

listbox_scrolled:
	mov	ebx,[ebx+C_NOTIFY_DATA]

; list_scroll: ebx = list, eax = scroll code

list_scroll:
	mov	edx,[ebx+L_TOP]
	cmp	eax,SB_LINEUP
	je	list_scroll_up
	cmp	eax,SB_LINEDOWN
	je	list_scroll_down
	cmp	eax,SB_PAGEUP
	je	list_scroll_page_up
	cmp	eax,SB_PAGEDOWN
	je	list_scroll_page_down
	cmp	eax,SB_THUMBTRACK
	jne	list_scrolled
	mov	edx,[scroll_track_position]
	jmp	list_scroll_set
    list_scroll_up:
	dec	edx
	jmp	list_scroll_set
    list_scroll_down:
	inc	edx
	jmp	list_scroll_set
    list_scroll_page_up:
	sub	edx,[ebx+L_VISIBLE]
	jmp	list_scroll_set
    list_scroll_page_down:
	add	edx,[ebx+L_VISIBLE]
    list_scroll_set:
	mov	eax,[ebx+L_COUNT]
	sub	eax,[ebx+L_VISIBLE]
	jns	list_scroll_limit_ok
	xor	eax,eax
    list_scroll_limit_ok:
	cmp	edx,eax
	jle	list_scroll_below_limit
	mov	edx,eax
    list_scroll_below_limit:
	cmp	edx,0
	jge	list_scroll_position_ok
	xor	edx,edx
    list_scroll_position_ok:
	cmp	edx,[ebx+L_TOP]
	je	list_scrolled
	mov	[ebx+L_TOP],edx
	call	control_redraw
    list_scrolled:
	retn

; listbox_index_at_pointer: ebx = list, returns eax = index (CF=1 if none)

listbox_index_at_pointer:
	mov	eax,[cm_y]
	sub	eax,2
	cmp	dword [ebx+L_COMBO],0
	je	list_pointer_offset_ok
	inc	eax
    list_pointer_offset_ok:
	cmp	eax,0
	jl	no_list_index
	xor	edx,edx
	div	dword [ebx+L_ITEM_HEIGHT]
	cmp	eax,[ebx+L_VISIBLE]
	jae	no_list_index
	add	eax,[ebx+L_TOP]
	cmp	eax,[ebx+L_COUNT]
	jae	no_list_index
	clc
	retn
    no_list_index:
	stc
	retn

    listbox_press:
	cmp	dword [ebx+L_COMBO],0
	je	listbox_normal_press
	call	pointer_inside_control
	jc	close_combo_list
	jmp	listbox_hover
    listbox_normal_press:
	cmp	[cm_button],1
	jne	listbox_pointer_done
	call	listbox_index_at_pointer
	jc	listbox_pointer_done
    listbox_click_index:
	test	dword [ebx+C_STYLE],LBS_EXTENDEDSEL
	jz	listbox_single_select
	test	[cm_state],X11_ControlMask
	jnz	listbox_toggle_item
	test	[cm_state],X11_ShiftMask
	jnz	listbox_range_select
    listbox_single_select:
	call	list_set_selection
	jmp	listbox_selection_changed
    listbox_toggle_item:
	mov	[ebx+L_SELECTION],eax
	mov	[ebx+L_ANCHOR],eax
	call	list_item
	xor	dword [edx+8],1
	call	control_redraw
	jmp	listbox_selection_changed
    listbox_range_select:
	push	eax
	call	list_clear_marks
	pop	eax
	mov	[ebx+L_SELECTION],eax
	mov	ecx,[ebx+L_ANCHOR]
	cmp	ecx,eax
	jle	listbox_range_ordered
	xchg	ecx,eax
    listbox_range_ordered:
	push	eax
	mov	eax,ecx
	call	list_item
	or	dword [edx+8],1
	pop	eax
	inc	ecx
	cmp	ecx,eax
	jle	listbox_range_ordered
	call	list_ensure_visible
	call	control_redraw
    listbox_selection_changed:
	mov	edx,LBN_SELCHANGE
	jmp	control_notify
    listbox_motion:
	cmp	dword [ebx+L_COMBO],0
	jne	listbox_hover
	test	[cm_state],X11_Button1Mask
	jz	listbox_pointer_done
	test	dword [ebx+C_STYLE],LBS_EXTENDEDSEL
	jnz	listbox_pointer_done
	call	listbox_index_at_pointer
	jc	listbox_pointer_done
	cmp	eax,[ebx+L_SELECTION]
	je	listbox_pointer_done
	jmp	listbox_single_select
    listbox_hover:
	call	listbox_index_at_pointer
	jc	listbox_pointer_done
	cmp	eax,[ebx+L_SELECTION]
	je	listbox_pointer_done
	jmp	list_set_selection
    listbox_release:
	cmp	dword [ebx+L_COMBO],0
	je	listbox_pointer_done
	call	pointer_inside_control
	jc	listbox_pointer_done
	call	listbox_index_at_pointer
	jc	listbox_pointer_done
	mov	ebx,[ebx+L_COMBO]
	jmp	combo_accept
    listbox_double_click:
	cmp	dword [ebx+L_COMBO],0
	jne	listbox_release
	call	listbox_index_at_pointer
	jc	listbox_pointer_done
	mov	edx,LBN_DBLCLK
	jmp	control_notify
    listbox_pointer_done:
	retn
    listbox_wheel:
	mov	eax,SB_LINEUP
	cmp	[cm_button],4
	je	listbox_wheel_scroll
	mov	eax,SB_LINEDOWN
    listbox_wheel_scroll:
	mov	[list_wheel_code],eax
	mov	ecx,3
    listbox_wheel_step:
	push	ecx
	mov	eax,[list_wheel_code]
	call	list_scroll
	pop	ecx
	loop	listbox_wheel_step
	retn
    listbox_key:
	mov	eax,[ebx+L_SELECTION]
	mov	edx,[cm_keysym]
	cmp	edx,XK_Up
	je	list_key_up
	cmp	edx,XK_Down
	je	list_key_down
	cmp	edx,XK_Prior
	je	list_key_page_up
	cmp	edx,XK_Next
	je	list_key_page_down
	cmp	edx,XK_Home
	je	list_key_home
	cmp	edx,XK_End
	je	list_key_end
	cmp	edx,20h
	jne	listbox_pointer_done
	test	dword [ebx+C_STYLE],LBS_EXTENDEDSEL
	jz	listbox_pointer_done
	cmp	eax,0
	jl	listbox_pointer_done
	jmp	listbox_toggle_item
    list_key_up:
	dec	eax
	jmp	list_key_index
    list_key_down:
	inc	eax
	jmp	list_key_index
    list_key_page_up:
	sub	eax,[ebx+L_VISIBLE]
	jmp	list_key_index
    list_key_page_down:
	add	eax,[ebx+L_VISIBLE]
	jmp	list_key_index
    list_key_home:
	xor	eax,eax
	jmp	list_key_index
    list_key_end:
	mov	eax,[ebx+L_COUNT]
	dec	eax
    list_key_index:
	mov	edx,[ebx+L_COUNT]
	dec	edx
	cmp	eax,edx
	jle	list_key_below_end
	mov	eax,edx
    list_key_below_end:
	cmp	eax,0
	jge	list_key_index_ok
	xor	eax,eax
    list_key_index_ok:
	cmp	dword [ebx+L_COUNT],0
	je	listbox_pointer_done
	cmp	eax,[ebx+L_SELECTION]
	je	listbox_pointer_done
	cmp	dword [ebx+L_COMBO],0
	jne	list_set_selection
	jmp	listbox_single_select

; combo box

combobox_proc:
	cmp	eax,CM_PAINT
	je	paint_combobox
	cmp	eax,CM_PRESS
	je	combobox_press
	cmp	eax,CM_DOUBLECLICK
	je	combobox_double_click
	cmp	eax,CM_MOTION
	je	combobox_motion
	cmp	eax,CM_RELEASE
	je	combobox_release
	cmp	eax,CM_KEY
	je	combobox_key
	cmp	eax,CM_FOCUS
	je	combobox_focus
	cmp	eax,CM_BLINK
	je	combobox_blink
	cmp	eax,CM_WHEEL
	je	combobox_wheel
	cmp	eax,CM_CREATE
	je	combobox_create
	cmp	eax,CM_DESTROY
	je	combobox_destroy
	retn
    combobox_create:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	combo_list_create
	push	ebx
	mov	ecx,EDIT_CAPACITY*4
	call	get_memory
	pop	ebx
	mov	[ebx+E_CELLS],eax
	mov	dword [ebx+E_CAPACITY],EDIT_CAPACITY
	call	edit_layout
	mov	esi,[ebx+C_TEXT]
	call	edit_set_text_only
    combo_list_create:
	mov	eax,[ebx+C_WIDTH]
	sub	eax,2+SCROLLBAR_SIZE
	mov	[ebx+CB_BUTTON_X],eax
	mov	ecx,CONTROL_SIZE
	push	ebx
	mov	eax,[building_dialog]
	push	eax
	mov	eax,[ebx+C_DIALOG]
	mov	[building_dialog],eax
	call	ui_alloc
	pop	edx
	mov	[building_dialog],edx
	pop	ebx
	mov	[ebx+CB_LIST],eax
	mov	edx,ebx
	mov	ebx,eax
	mov	dword [ebx+C_TYPE],CT_LISTBOX
	mov	dword [ebx+C_PROC],listbox_proc
	mov	eax,[edx+C_DIALOG]
	mov	[ebx+C_DIALOG],eax
	mov	[ebx+L_COMBO],edx
	mov	eax,[edx+C_WIDTH]
	mov	[ebx+C_WIDTH],eax
	mov	dword [ebx+C_HEIGHT],20
	mov	dword [ebx+C_TEXT],_empty_string
	push	edx
	call	listbox_init
	pop	ebx
	retn
    combobox_destroy:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	combo_list_destroy
	call	edit_destroy
    combo_list_destroy:
	push	ebx
	mov	ebx,[ebx+CB_LIST]
	call	listbox_memory_release
	pop	ebx
	retn
    paint_combobox:
	mov	eax,[px_window]
	test	dword [ebx+C_STATE],CS_DISABLED
	jz	combo_background_ok
	mov	eax,[px_face]
    combo_background_ok:
	mov	[edit_background],eax
	call	fill_control
	mov	eax,FRAME_SUNKEN
	call	frame_control
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	paint_combo_selection
	call	paint_edit_text
	jmp	paint_combo_button
    paint_combo_selection:
	mov	eax,[ebx+CB_LIST]
	mov	eax,[eax+L_SELECTION]
	push	ebx
	mov	ebx,[ebx+CB_LIST]
	call	list_get_string
	pop	ebx
	jc	paint_combo_button
	mov	ecx,[px_text]
	mov	edx,[edit_background]
	test	dword [ebx+C_STATE],CS_FOCUSED
	jz	combo_selection_colors_ok
	cmp	dword [ebx+CB_DROPPED],0
	jne	combo_selection_colors_ok
	mov	ecx,[px_selection_text]
	mov	edx,[px_selection]
	push	ebx ecx edx esi
	mov	eax,[ebx+C_WINDOW]
	mov	esi,[ebx+CB_BUTTON_X]
	sub	esi,4
	mov	edi,[ebx+C_HEIGHT]
	sub	edi,6
	mov	ecx,3
	mov	ebx,edx
	mov	edx,3
	call	ui_fill
	pop	esi edx ecx ebx
    combo_selection_colors_ok:
	mov	[ui_text_background],edx
	mov	[ui_text_mnemonics],0
	push	ebx
	mov	eax,[ebx+C_WINDOW]
	mov	edx,[ebx+C_HEIGHT]
	sub	edx,[ui_font_height]
	sar	edx,1
	mov	ebx,ecx
	mov	ecx,4
	call	ui_draw_text
	pop	ebx
    paint_combo_button:
	mov	eax,[ebx+C_WINDOW]
	mov	ecx,[ebx+CB_BUTTON_X]
	mov	edx,2
	mov	esi,SCROLLBAR_SIZE
	mov	edi,[ebx+C_HEIGHT]
	sub	edi,4
	push	ebx ecx edx esi edi
	mov	ebx,[px_face]
	call	ui_fill
	pop	edi esi edx ecx ebx
	push	ebx
	cmp	dword [ebx+CB_DROPPED],0
	mov	ebx,FRAME_RAISED
	je	combo_button_frame_ok
	mov	ebx,FRAME_PRESSED
    combo_button_frame_ok:
	call	ui_frame
	pop	ebx
	mov	[arrow_window],eax
	mov	[arrow_direction],3
	shr	esi,1
	add	ecx,esi
	shr	edi,1
	add	edx,edi
	push	ebx
	mov	ebx,[px_text]
	call	draw_arrow_triangle
	pop	ebx
	retn
    combobox_press:
	cmp	[cm_button],1
	jne	combo_pointer_done
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	combo_toggle_list
	mov	eax,[cm_x]
	cmp	eax,[ebx+CB_BUTTON_X]
	jl	edit_press
    combo_toggle_list:
	cmp	dword [ebx+CB_DROPPED],0
	jne	close_combo_list
	jmp	combo_drop
    combobox_double_click:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	combo_toggle_list
	mov	eax,[cm_x]
	cmp	eax,[ebx+CB_BUTTON_X]
	jge	combo_toggle_list
	jmp	edit_double_click
    combobox_motion:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jnz	edit_motion
    combo_pointer_done:
	retn
    combobox_release:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jnz	edit_release
	retn
    combobox_focus:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jnz	edit_focus
	retn
    combobox_blink:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	combo_pointer_done
	xor	dword [edit_caret_on],1
	jmp	control_redraw
    combobox_wheel:
	mov	eax,[ebx+CB_LIST]
	mov	eax,[eax+L_SELECTION]
	cmp	[cm_button],4
	je	combo_previous_item
	jmp	combo_next_item
    combobox_key:
	mov	eax,[cm_keysym]
	cmp	eax,XK_F4
	je	combo_toggle_list
	cmp	eax,XK_Down
	jne	combo_not_down
	test	[cm_flags],KS_ALT
	jnz	combo_toggle_list
	mov	eax,[ebx+CB_LIST]
	mov	eax,[eax+L_SELECTION]
    combo_next_item:
	inc	eax
	jmp	combo_select_item
    combo_not_down:
	cmp	eax,XK_Up
	jne	combo_other_key
	mov	eax,[ebx+CB_LIST]
	mov	eax,[eax+L_SELECTION]
    combo_previous_item:
	dec	eax
	cmp	eax,0
	jge	combo_select_item
	xor	eax,eax
    combo_select_item:
	mov	edx,[ebx+CB_LIST]
	mov	ecx,[edx+L_COUNT]
	test	ecx,ecx
	jz	combo_pointer_done
	cmp	eax,ecx
	jb	combo_item_index_ok
	lea	eax,[ecx-1]
    combo_item_index_ok:
	cmp	eax,[edx+L_SELECTION]
	je	combo_pointer_done
	call	combo_set_selection
	mov	edx,CBN_SELCHANGE
	jmp	control_notify
    combo_other_key:
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jnz	edit_key
	mov	eax,[cm_character]
	cmp	eax,'A'
	jb	combo_pointer_done
	cmp	eax,'Z'
	ja	combo_character_case_ok
	add	eax,20h
    combo_character_case_ok:
	mov	[combo_search_character],eax
	mov	edx,[ebx+CB_LIST]
	xor	ecx,ecx
    combo_search_item:
	cmp	ecx,[edx+L_COUNT]
	jae	combo_pointer_done
	push	ebx
	mov	ebx,edx
	mov	eax,ecx
	call	list_get_string
	pop	ebx
	movzx	eax,byte [esi]
	cmp	eax,'A'
	jb	combo_item_character_ok
	cmp	eax,'Z'
	ja	combo_item_character_ok
	add	eax,20h
    combo_item_character_ok:
	cmp	eax,[combo_search_character]
	je	combo_item_matches
	inc	ecx
	jmp	combo_search_item
    combo_item_matches:
	mov	eax,ecx
	jmp	combo_select_item

; combo_set_selection: ebx = combo box, eax = index

combo_set_selection:
	push	ebx
	mov	edx,ebx
	mov	ebx,[ebx+CB_LIST]
	mov	[ebx+L_SELECTION],eax
	call	list_ensure_visible
	call	list_get_string
	pop	ebx
	jc	combo_selection_redraw
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jz	combo_selection_redraw
	call	edit_set_text_only
	call	edit_select_all
    combo_selection_redraw:
	jmp	control_redraw

; combo_drop: ebx = combo box

combo_drop:
	mov	esi,[ebx+CB_LIST]
	mov	eax,[esi+L_COUNT]
	test	eax,eax
	jz	combo_dropped
	cmp	eax,8
	jbe	combo_rows_ok
	mov	eax,8
    combo_rows_ok:
	imul	eax,[esi+L_ITEM_HEIGHT]
	add	eax,2
	mov	[esi+C_HEIGHT],eax
	mov	eax,[ebx+C_WIDTH]
	mov	[esi+C_WIDTH],eax
	push	esi
	mov	eax,[ebx+C_WINDOW]
	xor	ecx,ecx
	mov	edx,[ebx+C_HEIGHT]
	call	x11_translate_coordinates
	pop	esi
	mov	eax,edx
	add	eax,[esi+C_HEIGHT]
	cmp	eax,[x11_screen_height]
	jbe	combo_list_position_ok
	sub	edx,[ebx+C_HEIGHT]
	sub	edx,[esi+C_HEIGHT]
    combo_list_position_ok:
	mov	eax,[px_window]
	mov	[window_values],eax
	mov	[window_values+4],1
	mov	[window_values+8],X11_ExposureMask+X11_ButtonPressMask+X11_ButtonReleaseMask+X11_PointerMotionMask
	mov	[window_value_mask],2+200h+800h
	push	ebx esi
	mov	eax,[x11_root]
	mov	edi,[esi+C_HEIGHT]
	mov	esi,[esi+C_WIDTH]
	call	x11_create_window
	pop	esi ebx
	mov	[esi+C_WINDOW],eax
	push	ebx
	mov	ebx,esi
	call	listbox_layout
	call	list_ensure_visible
	mov	eax,[ebx+C_WINDOW]
	mov	ecx,WK_CONTROL
	mov	edx,ebx
	mov	esi,[ebx+C_DIALOG]
	call	register_window
	call	x11_map_window
	mov	ecx,X11_ButtonPressMask+X11_ButtonReleaseMask+X11_PointerMotionMask
	mov	edx,[last_event_time]
	call	x11_grab_pointer
	pop	ebx
	mov	[dropped_combo],ebx
	mov	dword [ebx+CB_DROPPED],1
	call	control_redraw
    combo_dropped:
	retn

; close_combo_list: closes list of [dropped_combo]

close_combo_list:
	mov	ebx,[dropped_combo]
	test	ebx,ebx
	jz	combo_list_closed
	mov	[dropped_combo],0
	mov	dword [ebx+CB_DROPPED],0
	mov	eax,[last_event_time]
	call	x11_ungrab_pointer
	mov	esi,[ebx+CB_LIST]
	mov	eax,[esi+C_WINDOW]
	mov	dword [esi+C_WINDOW],0
	call	unregister_window
	call	x11_destroy_window
	call	control_redraw
    combo_list_closed:
	retn

; combo_accept: ebx = combo box (selection of its list is accepted)

combo_accept:
	push	ebx
	call	close_combo_list
	pop	ebx
	mov	eax,[ebx+CB_LIST]
	mov	eax,[eax+L_SELECTION]
	cmp	eax,0
	jl	combo_list_closed
	call	combo_set_selection
	mov	edx,CBN_SELCHANGE
	jmp	control_notify

; dropped_combo_key: keyboard while list of combo box is shown

dropped_combo_key:
	mov	ebx,[dropped_combo]
	mov	eax,[cm_keysym]
	cmp	eax,XK_Return
	je	accept_dropped_combo
	cmp	eax,XK_KP_Enter
	je	accept_dropped_combo
	cmp	eax,XK_Escape
	je	close_combo_list
	cmp	eax,XK_F4
	je	close_combo_list
	cmp	eax,XK_Up
	jne	dropped_combo_not_up
	test	[cm_flags],KS_ALT
	jnz	close_combo_list
    dropped_combo_not_up:
	mov	ebx,[ebx+CB_LIST]
	jmp	listbox_key
    accept_dropped_combo:
	jmp	combo_accept

; combo box helpers for dialog procedures

combo_add_string:
	push	ebx
	mov	ebx,[ebx+CB_LIST]
	call	list_add_string
	pop	ebx
	retn

combo_find_string:
	push	ebx
	mov	ebx,[ebx+CB_LIST]
	call	list_find_string
	pop	ebx
	retn

combo_get_selection:
	mov	eax,[ebx+CB_LIST]
	mov	eax,[eax+L_SELECTION]
	retn

; editor inside dialog

fedit_control_proc:
	cmp	eax,CM_PAINT
	je	paint_fedit_control
	cmp	eax,CM_CREATE
	je	fedit_control_create
	cmp	eax,CM_DESTROY
	je	fedit_control_destroy
	cmp	eax,CM_FOCUS
	je	fedit_control_focus
	cmp	eax,CM_UNFOCUS
	je	fedit_control_unfocus
	cmp	eax,CM_KEY
	je	fedit_control_key
	cmp	eax,CM_BLINK
	je	fedit_control_blink
	retn
    paint_fedit_control:
	mov	eax,FRAME_SUNKEN
	jmp	frame_control
    fedit_control_create:
	xor	eax,eax
	test	dword [ebx+C_STYLE],ES_MULTILINE
	jz	fedit_control_scrollbars_ok
	or	eax,FES_SCROLLBARS
    fedit_control_scrollbars_ok:
	test	dword [ebx+C_STYLE],WS_DISABLED
	jz	fedit_control_flags_ok
	or	eax,FES_DISABLED
    fedit_control_flags_ok:
	mov	[fedit_create_flags],eax
	push	ebx
	mov	eax,[ebx+C_DIALOG]
	mov	[building_dialog],eax
	mov	eax,[ebx+C_WINDOW]
	mov	ecx,2
	mov	edx,2
	mov	esi,[ebx+C_WIDTH]
	sub	esi,4
	mov	edi,[ebx+C_HEIGHT]
	sub	edi,4
	call	fedit_create
	pop	ebx
	mov	[ebx+F_EDITOR],eax
	test	eax,eax
	jz	fedit_control_done
	mov	[fe_x],2
	mov	[fe_y],2
	mov	eax,[ebx+C_WIDTH]
	sub	eax,4
	mov	[fe_width],eax
	mov	eax,[ebx+C_HEIGHT]
	sub	eax,4
	mov	[fe_height],eax
	mov	[fe_message],FM_MOVE
	call	fedit_control_send
	mov	[fe_wparam],1
	mov	[fe_message],FM_SHOW
	call	fedit_control_send
	test	dword [ebx+C_STYLE],ES_READONLY
	jz	fedit_control_done
	mov	[fe_message],FM_SETMODE
	mov	[fe_wparam],FEMODE_READONLY
	jmp	fedit_control_send
    fedit_control_destroy:
	mov	[fe_message],FM_DESTROY
	jmp	fedit_control_send
    fedit_control_focus:
	mov	[fe_message],FM_SETFOCUS
	jmp	fedit_control_send
    fedit_control_unfocus:
	mov	[fe_message],FM_KILLFOCUS
	jmp	fedit_control_send
    fedit_control_blink:
	mov	[fe_message],FM_BLINK
	jmp	fedit_control_send
    fedit_control_key:
	mov	eax,[cm_keysym]
	mov	[fe_wparam],eax
	mov	eax,[cm_base_keysym]
	mov	[fe_lparam],eax
	mov	eax,[cm_state]
	mov	[fe_state],eax
	mov	[fe_message],FM_KEYDOWN
    fedit_control_send:
	mov	eax,[ebx+F_EDITOR]
	test	eax,eax
	jz	fedit_control_done
	mov	[fedit_handle],eax
	push	ebx
	call	fedit_proc
	pop	ebx
    fedit_control_done:
	retn

; fedit_control_set_text: ebx = control, esi = UTF-8 text

fedit_control_set_text:
	push	ebx
	call	string_length
	push	esi ecx
	lea	ecx,[ecx*4+4]
	call	get_memory
	pop	ecx esi
	test	eax,eax
	jz	fedit_text_not_set
	mov	[fedit_text_memory],eax
	mov	edi,eax
	call	utf8_to_cells
	mov	ebx,[esp]
	mov	eax,[ebx+F_EDITOR]
	mov	[fedit_handle],eax
	mov	[fe_message],FM_SETTEXT
	mov	eax,[fedit_text_memory]
	mov	[fe_lparam],eax
	call	fedit_proc
	mov	ebx,[fedit_text_memory]
	call	release_memory
    fedit_text_not_set:
	pop	ebx
	retn

; dialog keyboard handling: ebp = dialog

dialog_key:
	mov	eax,[cm_keysym]
	cmp	eax,XK_Tab
	je	dialog_next_control
	cmp	eax,XK_ISO_Left_Tab
	je	dialog_previous_control
	cmp	eax,XK_Escape
	je	dialog_cancel
	cmp	eax,XK_Return
	je	dialog_enter
	cmp	eax,XK_KP_Enter
	je	dialog_enter
	mov	ebx,[ebp+D_FOCUS]
	test	[cm_flags],KS_ALT
	jnz	dialog_mnemonic
	test	ebx,ebx
	jz	dialog_mnemonic
	mov	eax,[ebx+C_TYPE]
	cmp	eax,CT_EDIT
	je	dialog_key_to_control
	cmp	eax,CT_FEDIT
	je	dialog_key_to_control
	cmp	eax,CT_COMBOBOX
	jne	dialog_mnemonic_or_control
	test	dword [ebx+C_STYLE],CBS_DROPDOWN
	jnz	dialog_key_to_control
    dialog_mnemonic_or_control:
	cmp	[cm_character],20h
	jbe	dialog_key_to_control
	cmp	[cm_flags],0
	jne	dialog_key_to_control
	call	dialog_find_mnemonic
	jnc	dialog_mnemonic_found
    dialog_key_to_control:
	test	ebx,ebx
	jz	dialog_key_done
	call	reset_blink
	mov	eax,CM_KEY
	jmp	control_message
    dialog_key_done:
	retn
    dialog_next_control:
	mov	[focus_backward],0
	test	[cm_flags],KS_SHIFT
	jz	dialog_move_focus
    dialog_previous_control:
	mov	[focus_backward],1
    dialog_move_focus:
	mov	ebx,[ebp+D_FOCUS]
	call	next_focus_control
	mov	[focus_backward],0
	test	ebx,ebx
	jz	dialog_key_done
	mov	[keyboard_focus_change],1
	jmp	set_dialog_focus
    dialog_cancel:
	mov	edx,IDCANCEL
	xor	ecx,ecx
	jmp	dialog_command
    dialog_enter:
	mov	ebx,[ebp+D_FOCUS]
	test	ebx,ebx
	jz	dialog_default_button
	cmp	dword [ebx+C_TYPE],CT_BUTTON
	jne	dialog_default_button
	test	dword [ebx+C_STATE],CS_DISABLED
	jnz	dialog_key_done
	jmp	click_control
    dialog_default_button:
	xor	ecx,ecx
    find_default_button:
	cmp	ecx,[ebp+D_COUNT]
	jae	dialog_key_done
	mov	ebx,[ebp+D_CONTROLS+ecx*4]
	cmp	dword [ebx+C_TYPE],CT_BUTTON
	jne	next_default_candidate
	test	dword [ebx+C_STYLE],BS_DEFPUSHBUTTON
	jnz	default_button_found
    next_default_candidate:
	inc	ecx
	jmp	find_default_button
    default_button_found:
	test	dword [ebx+C_STATE],CS_DISABLED
	jnz	dialog_key_done
	jmp	click_control
    dialog_mnemonic:
	mov	eax,[cm_base_keysym]
	cmp	eax,'A'
	jb	dialog_key_to_control
	cmp	eax,'z'
	ja	dialog_key_to_control
	call	dialog_find_mnemonic
	jc	dialog_key_to_control
    dialog_mnemonic_found:
	cmp	dword [ebx+C_TYPE],CT_STATIC
	jne	mnemonic_control_ok
	mov	[focus_backward],0
	call	next_focus_control
	test	ebx,ebx
	jz	dialog_key_done
	mov	[keyboard_focus_change],1
	jmp	set_dialog_focus
    mnemonic_control_ok:
	test	dword [ebx+C_STATE],CS_DISABLED
	jnz	dialog_key_done
	mov	[keyboard_focus_change],1
	call	set_dialog_focus
	mov	eax,[ebx+C_TYPE]
	cmp	eax,CT_BUTTON
	je	click_control
	cmp	eax,CT_CHECKBOX
	je	click_control
	cmp	eax,CT_RADIO
	je	click_control
	retn

; dialog_find_mnemonic: ebp = dialog, [cm_base_keysym] or [cm_character]
; returns CF=0 and ebx = control

dialog_find_mnemonic:
	mov	eax,[cm_base_keysym]
	cmp	eax,'A'
	jb	mnemonic_key_ok
	cmp	eax,'Z'
	ja	mnemonic_key_ok
	add	eax,20h
    mnemonic_key_ok:
	mov	edx,eax
	xor	ecx,ecx
    search_mnemonic:
	cmp	ecx,[ebp+D_COUNT]
	jae	mnemonic_not_found
	mov	ebx,[ebp+D_CONTROLS+ecx*4]
	mov	esi,[ebx+C_TEXT]
	call	text_mnemonic_character
	cmp	eax,edx
	je	mnemonic_matches
	inc	ecx
	jmp	search_mnemonic
    mnemonic_matches:
	clc
	retn
    mnemonic_not_found:
	stc
	retn

; menus

; open_menu: esi = menu items, ecx = x, edx = y (root coordinates),
; [menu_bar_index] = index on menu bar (-1 for context menu)

open_menu:
	mov	[menu_items],esi
	or	[menu_selected],-1
	push	ecx edx
	call	main_init_menu
	mov	esi,[menu_items]
	xor	ecx,ecx
	xor	edx,edx
	xor	ebx,ebx
	mov	[menu_accelerator_width],0
    measure_menu_item:
	cmp	dword [esi],-1
	je	menu_measured
	test	dword [esi+8],MF_SEPARATOR
	jz	measure_menu_text
	add	edx,MENU_SEPARATOR_HEIGHT
	jmp	next_measured_item
    measure_menu_text:
	add	edx,[ui_font_height]
	add	edx,MENU_ITEM_PADDING
	push	esi
	mov	esi,[esi+4]
	mov	[ui_text_mnemonics],1
	call	ui_text_width
	cmp	eax,ecx
	jbe	menu_text_width_ok
	mov	ecx,eax
    menu_text_width_ok:
	call	menu_accelerator_text
	jc	menu_accelerator_measured
	mov	[ui_text_mnemonics],0
	call	ui_text_width
	cmp	eax,[menu_accelerator_width]
	jbe	menu_accelerator_measured
	mov	[menu_accelerator_width],eax
    menu_accelerator_measured:
	pop	esi
    next_measured_item:
	add	esi,MENU_ITEM_SIZE
	inc	ebx
	jmp	measure_menu_item
    menu_measured:
	mov	[menu_count],ebx
	mov	eax,[ui_font_width]
	imul	eax,6
	add	ecx,eax
	cmp	[menu_accelerator_width],0
	je	menu_width_ok
	add	ecx,[menu_accelerator_width]
	add	ecx,[ui_font_width]
	add	ecx,[ui_font_width]
    menu_width_ok:
	mov	[menu_width],ecx
	add	edx,4
	mov	[menu_height],edx
	pop	edx ecx
	mov	eax,ecx
	add	eax,[menu_width]
	cmp	eax,[x11_screen_width]
	jbe	menu_x_ok
	mov	ecx,[x11_screen_width]
	sub	ecx,[menu_width]
	jns	menu_x_ok
	xor	ecx,ecx
    menu_x_ok:
	mov	eax,edx
	add	eax,[menu_height]
	cmp	eax,[x11_screen_height]
	jbe	menu_y_ok
	sub	edx,[menu_height]
	jns	menu_y_ok
	xor	edx,edx
    menu_y_ok:
	mov	[menu_x],ecx
	mov	[menu_y],edx
	mov	eax,[px_face]
	mov	[window_values],eax
	mov	[window_values+4],1
	mov	[window_values+8],X11_ExposureMask+X11_ButtonPressMask+X11_ButtonReleaseMask+X11_PointerMotionMask
	mov	[window_value_mask],2+200h+800h
	mov	eax,[x11_root]
	mov	esi,[menu_width]
	mov	edi,[menu_height]
	call	x11_create_window
	mov	[menu_window],eax
	mov	ecx,WK_MENU
	xor	edx,edx
	xor	esi,esi
	call	register_window
	call	x11_map_window
	cmp	[menu_active],0
	jne	menu_opened
	mov	eax,[main_window]
	mov	ecx,X11_ButtonPressMask+X11_ButtonReleaseMask+X11_PointerMotionMask
	mov	edx,[last_event_time]
	call	x11_grab_pointer
	mov	[menu_active],1
    menu_opened:
	retn

MENU_SEPARATOR_HEIGHT = 7
MENU_ITEM_PADDING = 4

; menu_accelerator_text: esi = item text, returns CF=0 and esi = accelerator

menu_accelerator_text:
	lodsb
	test	al,al
	jz	no_accelerator_text
	cmp	al,9
	jne	menu_accelerator_text
	clc
	retn
    no_accelerator_text:
	stc
	retn

; close_popup: closes the current popup window (menu stays active)

close_popup:
	mov	eax,[menu_window]
	test	eax,eax
	jz	popup_closed
	mov	[menu_window],0
	call	unregister_window
	call	x11_destroy_window
    popup_closed:
	retn

; close_menu: closes popup and ends menu mode

close_menu:
	cmp	[menu_active],0
	je	menu_closed
	call	close_popup
	mov	[menu_active],0
	mov	eax,[last_event_time]
	call	x11_ungrab_pointer
	or	[menu_bar_index],-1
	call	menubar_redraw
    menu_closed:
	retn

; menu_expose: paints the popup window

menu_expose:
	mov	eax,[esi+4]
	cmp	eax,[menu_window]
	jne	menubar_expose
	mov	eax,[menu_window]
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,[menu_width]
	mov	edi,[menu_height]
	mov	ebx,[px_face]
	call	ui_fill
	mov	ebx,FRAME_RAISED
	call	ui_frame
	xor	ecx,ecx
    paint_menu_item:
	cmp	ecx,[menu_count]
	jae	menu_painted
	push	ecx
	call	paint_menu_entry
	pop	ecx
	inc	ecx
	jmp	paint_menu_item
    menu_painted:
	retn
    menubar_expose:
	jmp	menubar_redraw

; menu_item_position: ecx = item index, returns edx = y, eax = height

menu_item_position:
	push	ecx esi
	mov	esi,[menu_items]
	mov	edx,2
    find_menu_item_position:
	test	dword [esi+8],MF_SEPARATOR
	mov	eax,MENU_SEPARATOR_HEIGHT
	jnz	menu_item_height_ok
	mov	eax,[ui_font_height]
	add	eax,MENU_ITEM_PADDING
    menu_item_height_ok:
	jecxz	menu_item_position_found
	add	edx,eax
	add	esi,MENU_ITEM_SIZE
	dec	ecx
	jmp	find_menu_item_position
    menu_item_position_found:
	pop	esi ecx
	retn

; paint_menu_entry: ecx = item index

paint_menu_entry:
	call	menu_item_position
	mov	[menu_entry_y],edx
	mov	[menu_entry_height],eax
	mov	esi,ecx
	shl	esi,4
	add	esi,[menu_items]
	test	dword [esi+8],MF_SEPARATOR
	jz	paint_menu_text_entry
	mov	eax,[menu_window]
	mov	ecx,2
	add	edx,3
	mov	edi,edx
	push	esi
	mov	esi,[menu_width]
	sub	esi,3
	mov	ebx,[px_shadow]
	call	ui_line
	inc	edx
	inc	edi
	mov	ebx,[px_light]
	call	ui_line
	pop	esi
	retn
    paint_menu_text_entry:
	mov	ebx,[px_face]
	mov	edi,[px_text]
	test	dword [esi+12],MIS_GRAYED
	jz	menu_entry_enabled
	mov	edi,[px_gray]
    menu_entry_enabled:
	cmp	ecx,[menu_selected]
	jne	menu_entry_colors_ok
	mov	ebx,[px_selection]
	mov	edi,[px_selection_text]
	test	dword [esi+12],MIS_GRAYED
	jz	menu_entry_colors_ok
	mov	edi,[px_gray]
    menu_entry_colors_ok:
	mov	[menu_entry_color],edi
	mov	[ui_text_background],ebx
	push	esi
	mov	eax,[menu_window]
	mov	ecx,2
	mov	edx,[menu_entry_y]
	mov	esi,[menu_width]
	sub	esi,4
	mov	edi,[menu_entry_height]
	call	ui_fill
	pop	esi
	test	dword [esi+12],MIS_CHECKED
	jz	menu_check_done
	push	esi
	mov	eax,[menu_window]
	mov	ebx,[menu_entry_color]
	mov	ecx,[ui_font_width]
	mov	edx,[menu_entry_height]
	shr	edx,1
	add	edx,[menu_entry_y]
	mov	[check_line],2
    draw_menu_check:
	lea	esi,[ecx+2]
	lea	edi,[edx+2]
	call	ui_line
	mov	ecx,esi
	mov	edx,edi
	add	esi,4
	sub	edi,4
	call	ui_line
	sub	ecx,2
	sub	edx,1
	dec	[check_line]
	jnz	draw_menu_check
	pop	esi
    menu_check_done:
	push	esi
	mov	esi,[esi+4]
	mov	ecx,[ui_font_width]
	imul	ecx,3
	mov	edx,[menu_entry_y]
	add	edx,MENU_ITEM_PADDING/2
	mov	ebx,[menu_entry_color]
	mov	[ui_text_mnemonics],1
	call	ui_draw_text
	pop	esi
	mov	esi,[esi+4]
	call	menu_accelerator_text
	jc	menu_entry_painted
	mov	[ui_text_mnemonics],0
	mov	eax,[menu_window]
	mov	ecx,[menu_width]
	sub	ecx,[menu_accelerator_width]
	sub	ecx,[ui_font_width]
	sub	ecx,[ui_font_width]
	mov	edx,[menu_entry_y]
	add	edx,MENU_ITEM_PADDING/2
	mov	ebx,[menu_entry_color]
	call	ui_draw_text
    menu_entry_painted:
	retn

; menu_item_at_pointer: returns ecx = item index under pointer (-1 if none)

menu_item_at_pointer:
	or	ecx,-1
	mov	eax,[cm_x]
	cmp	eax,0
	jl	menu_pointer_found
	cmp	eax,[menu_width]
	jge	menu_pointer_found
	xor	ecx,ecx
    check_menu_item_area:
	cmp	ecx,[menu_count]
	jae	no_menu_item_at_pointer
	call	menu_item_position
	mov	ebx,[cm_y]
	cmp	ebx,edx
	jl	no_menu_item_at_pointer
	add	edx,eax
	cmp	ebx,edx
	jl	menu_pointer_found
	inc	ecx
	jmp	check_menu_item_area
    no_menu_item_at_pointer:
	or	ecx,-1
    menu_pointer_found:
	retn

; menu_pointer_event: edi = window entry

menu_pointer_event:
	mov	eax,[edi]
	cmp	eax,[menu_window]
	jne	menubar_pointer_event
	call	menu_item_at_pointer
	cmp	[cm_event],X11_ButtonRelease
	je	menu_release
	cmp	[cm_event],X11_ButtonPress
	je	menu_press_inside
	cmp	ecx,-1
	je	menu_pointer_done
	call	select_menu_entry
    menu_pointer_done:
	retn
    menu_press_inside:
	mov	[menu_ignore_release],0
	retn
    menu_release:
	cmp	[menu_ignore_release],0
	jne	menu_release_ignored
	cmp	ecx,-1
	je	menu_pointer_done
	jmp	execute_menu_entry
    menu_release_ignored:
	mov	[menu_ignore_release],0
	retn

; select_menu_entry: ecx = index

select_menu_entry:
	mov	eax,[menu_selected]
	cmp	eax,ecx
	je	menu_entry_selected
	mov	[menu_selected],ecx
	push	ecx
	cmp	eax,-1
	je	previous_entry_repainted
	mov	ecx,eax
	call	paint_menu_entry
    previous_entry_repainted:
	pop	ecx
	cmp	ecx,-1
	je	menu_entry_selected
	call	paint_menu_entry
    menu_entry_selected:
	retn

; execute_menu_entry: ecx = index

execute_menu_entry:
	mov	esi,ecx
	shl	esi,4
	add	esi,[menu_items]
	test	dword [esi+8],MF_SEPARATOR
	jnz	menu_entry_not_executed
	test	dword [esi+12],MIS_GRAYED
	jnz	menu_entry_not_executed
	mov	eax,[esi]
	push	eax
	call	close_menu
	pop	eax
	jmp	main_command
    menu_entry_not_executed:
	retn

; menu_key: keyboard in menu mode

menu_key:
	mov	eax,[cm_keysym]
	cmp	eax,XK_Escape
	je	close_menu
	cmp	eax,XK_Up
	je	menu_key_up
	cmp	eax,XK_Down
	je	menu_key_down
	cmp	eax,XK_Left
	je	menu_key_left
	cmp	eax,XK_Right
	je	menu_key_right
	cmp	eax,XK_Return
	je	menu_key_execute
	cmp	eax,XK_KP_Enter
	je	menu_key_execute
	cmp	eax,20h
	je	menu_key_execute
	mov	eax,[cm_base_keysym]
	cmp	eax,'A'
	jb	menu_key_done
	cmp	eax,'Z'
	ja	menu_key_letter_ok
	add	eax,20h
    menu_key_letter_ok:
	mov	edx,eax
	xor	ecx,ecx
	mov	edi,[menu_items]
    find_menu_mnemonic:
	cmp	ecx,[menu_count]
	jae	menu_key_done
	test	dword [edi+8],MF_SEPARATOR
	jnz	next_menu_mnemonic
	mov	esi,[edi+4]
	call	text_mnemonic_character
	cmp	eax,edx
	je	execute_menu_entry
    next_menu_mnemonic:
	add	edi,MENU_ITEM_SIZE
	inc	ecx
	jmp	find_menu_mnemonic
    menu_key_done:
	retn
    menu_key_execute:
	mov	ecx,[menu_selected]
	cmp	ecx,-1
	je	menu_key_done
	jmp	execute_menu_entry
    menu_key_up:
	mov	ecx,[menu_selected]
	mov	edx,-1
	jmp	menu_key_move
    menu_key_down:
	mov	ecx,[menu_selected]
	mov	edx,1
    menu_key_move:
	push	edx
	mov	ebx,[menu_count]
    menu_key_next_entry:
	add	ecx,[esp]
	cmp	ecx,0
	jge	menu_key_not_before_start
	lea	ecx,[ebx-1]
    menu_key_not_before_start:
	cmp	ecx,ebx
	jb	menu_key_index_ok
	xor	ecx,ecx
    menu_key_index_ok:
	mov	esi,ecx
	shl	esi,4
	add	esi,[menu_items]
	test	dword [esi+8],MF_SEPARATOR
	jnz	menu_key_next_entry
	pop	edx
	jmp	select_menu_entry
    menu_key_left:
	mov	eax,[menu_bar_index]
	cmp	eax,-1
	je	menu_key_done
	dec	eax
	jns	switch_menu_bar_popup
	mov	eax,[menubar_count]
	dec	eax
	jmp	switch_menu_bar_popup
    menu_key_right:
	mov	eax,[menu_bar_index]
	cmp	eax,-1
	je	menu_key_done
	inc	eax
	cmp	eax,[menubar_count]
	jb	switch_menu_bar_popup
	xor	eax,eax

; switch_menu_bar_popup: eax = index of menu bar entry to open

switch_menu_bar_popup:
	push	eax
	call	close_popup
	pop	eax
	jmp	open_menubar_popup

; message_box: esi = text, edi = caption, eax = flags
; returns IDOK, IDCANCEL, IDYES or IDNO

message_box:
	push	ebx ecx edx esi edi ebp
	mov	[message_flags],eax
	mov	[message_text],esi
	mov	edx,edi
	mov	edi,message_template
	lea	eax,[edi+16]
	mov	[edi],eax
	push	edi
	add	edi,16
	mov	esi,edx
	call	copy_text
	inc	edi
	add	edi,3
	and	edi,not 3
	mov	[message_items],edi
	mov	esi,[message_text]
	mov	[ui_text_mnemonics],0
	xor	ebx,ebx
	xor	ecx,ecx
    measure_message_line:
	call	ui_text_width
	cmp	eax,ebx
	jbe	message_line_width_ok
	mov	ebx,eax
    message_line_width_ok:
	inc	ecx
    skip_message_line:
	lodsb
	test	al,al
	jz	message_measured
	cmp	al,0Ah
	jne	skip_message_line
	jmp	measure_message_line
    message_measured:
	imul	ecx,[ui_font_height]
	mov	[message_text_height],ecx
	mov	[message_text_width],ebx
	mov	edi,[message_items]
	mov	eax,12
	mov	ecx,[message_flags]
	and	ecx,0F0h
	jz	message_icon_ok
	shr	ecx,4
	shl	ecx,24
	or	ecx,SS_ICON
	mov	[message_item_style],ecx
	mov	esi,_empty_string
	mov	ecx,CT_ICON
	mov	edx,-1
	mov	[message_item_x],12
	mov	[message_item_y],12
	mov	[message_item_width],32
	mov	[message_item_height],32
	call	add_message_item
	mov	eax,56
    message_icon_ok:
	mov	[message_item_x],eax
	add	eax,[message_text_width]
	add	eax,12
	mov	[message_width],eax
	mov	eax,[message_text_height]
	cmp	eax,32
	jae	message_text_height_ok
	mov	eax,32
    message_text_height_ok:
	add	eax,12
	mov	[message_buttons_y],eax
	mov	eax,[message_text_height]
	mov	[message_item_height],eax
	mov	eax,[message_text_width]
	mov	[message_item_width],eax
	mov	[message_item_y],12
	cmp	[message_text_height],32
	jae	message_text_position_ok
	mov	eax,32
	sub	eax,[message_text_height]
	shr	eax,1
	add	eax,12
	mov	[message_item_y],eax
    message_text_position_ok:
	mov	[message_item_style],0
	mov	esi,[message_text]
	mov	ecx,CT_STATIC
	mov	edx,-1
	call	add_message_item
	mov	eax,[message_flags]
	and	eax,0Fh
	mov	ecx,1
	cmp	eax,MB_YESNOCANCEL
	jne	message_not_three_buttons
	mov	ecx,3
    message_not_three_buttons:
	cmp	eax,MB_OKCANCEL
	je	message_two_buttons
	cmp	eax,MB_YESNO
	jne	message_buttons_counted
    message_two_buttons:
	mov	ecx,2
    message_buttons_counted:
	mov	[message_buttons],ecx
	imul	ecx,MESSAGE_BUTTON_WIDTH+6
	add	ecx,18
	cmp	ecx,[message_width]
	jbe	message_width_ok
	mov	[message_width],ecx
    message_width_ok:
	mov	eax,[message_width]
	sub	eax,ecx
	add	eax,12
	mov	[message_item_x],eax
	mov	eax,[message_buttons_y]
	mov	[message_item_y],eax
	mov	[message_item_width],MESSAGE_BUTTON_WIDTH
	mov	[message_item_height],MESSAGE_BUTTON_HEIGHT
	mov	eax,[message_flags]
	and	eax,0Fh
	mov	esi,message_button_sets
	mov	esi,[esi+eax*4]
	xor	ebx,ebx
    add_message_button:
	cmp	ebx,[message_buttons]
	jae	message_buttons_added
	mov	ecx,WS_TABSTOP
	mov	eax,ebx
	test	[message_flags],MB_DEFBUTTON2
	jz	message_default_index_ok
	dec	eax
    message_default_index_ok:
	test	eax,eax
	jnz	message_button_style_ok
	or	ecx,BS_DEFPUSHBUTTON
    message_button_style_ok:
	mov	[message_item_style],ecx
	mov	edx,[esi]
	push	esi ebx
	mov	esi,[esi+4]
	mov	ecx,CT_BUTTON
	call	add_message_item
	pop	ebx esi
	add	esi,8
	add	[message_item_x],MESSAGE_BUTTON_WIDTH+6
	inc	ebx
	jmp	add_message_button
    message_buttons_added:
	mov	dword [edi],0
	pop	edi
	mov	eax,[message_width]
	mov	[edi+4],eax
	mov	eax,[message_buttons_y]
	add	eax,MESSAGE_BUTTON_HEIGHT+12
	mov	[edi+8],eax
	mov	dword [edi+12],DT_PIXELS
	mov	esi,edi
	mov	edi,message_box_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,-1
	jne	message_box_done
	mov	eax,IDCANCEL
    message_box_done:
	pop	ebp edi esi edx ecx ebx
	retn

MESSAGE_BUTTON_WIDTH = 75
MESSAGE_BUTTON_HEIGHT = 23

; add_message_item: edi = place, ecx = type, edx = id, esi = text,
; position in message_item_* variables

add_message_item:
	push	esi
	mov	[edi+4],ecx
	mov	[edi+8],edx
	mov	eax,[message_item_x]
	mov	[edi+12],eax
	mov	eax,[message_item_y]
	mov	[edi+16],eax
	mov	eax,[message_item_width]
	mov	[edi+20],eax
	mov	eax,[message_item_height]
	mov	[edi+24],eax
	mov	eax,[message_item_style]
	mov	[edi+28],eax
	push	edi
	add	edi,32
	call	copy_text
	inc	edi
	add	edi,3
	and	edi,not 3
	pop	eax
	mov	[eax],edi
	pop	esi
	retn

message_box_procedure:
	cmp	eax,DM_COMMAND
	je	message_box_command
	cmp	eax,DM_CLOSE
	jne	message_box_procedure_done
	mov	eax,[message_flags]
	and	eax,0Fh
	mov	edx,IDOK
	test	eax,eax
	jz	message_box_command
	mov	edx,IDCANCEL
	cmp	eax,MB_YESNO
	jne	message_box_command
	mov	edx,IDNO
    message_box_command:
	mov	eax,edx
	jmp	end_dialog
    message_box_procedure_done:
	retn

; menu bar (window menubar_window in the main window)

; menubar_redraw: paints the menu bar

menubar_redraw:
	mov	eax,[menubar_window]
	test	eax,eax
	jz	menubar_painted
	xor	ecx,ecx
	xor	edx,edx
	mov	esi,[main_width]
	mov	edi,[menubar_height]
	mov	ebx,[px_face]
	call	ui_fill
	mov	esi,menubar_items
	xor	ecx,ecx
	mov	edx,[ui_font_width]
    paint_menubar_item:
	cmp	dword [esi],0
	je	menubar_items_painted
	mov	[menubar_positions+ecx*4],edx
	push	ecx edx esi
	mov	esi,[esi]
	mov	[ui_text_mnemonics],1
	call	ui_text_width
	add	eax,[ui_font_width]
	add	eax,[ui_font_width]
	mov	[menubar_widths+ecx*4],eax
	mov	[menubar_item_width],eax
	mov	ebx,[px_face]
	mov	edi,[px_text]
	cmp	[menu_active],0
	je	menubar_item_colors_ok
	cmp	ecx,[menu_bar_index]
	jne	menubar_item_colors_ok
	mov	ebx,[px_selection]
	mov	edi,[px_selection_text]
    menubar_item_colors_ok:
	mov	[ui_text_background],ebx
	mov	[menubar_item_color],edi
	mov	eax,[menubar_window]
	mov	ecx,edx
	mov	edx,1
	push	esi
	mov	esi,[menubar_item_width]
	mov	edi,[menubar_height]
	sub	edi,2
	call	ui_fill
	pop	esi
	add	ecx,[ui_font_width]
	mov	edx,[menubar_height]
	sub	edx,[ui_font_height]
	shr	edx,1
	mov	ebx,[menubar_item_color]
	call	ui_draw_text
	pop	esi edx ecx
	add	edx,[menubar_widths+ecx*4]
	add	esi,8
	inc	ecx
	jmp	paint_menubar_item
    menubar_items_painted:
	mov	[menubar_count],ecx
    menubar_painted:
	retn

; menubar_item_at_pointer: returns eax = index under pointer (-1 if none)

menubar_item_at_pointer:
	xor	eax,eax
	mov	edx,[cm_x]
    check_menubar_item:
	cmp	eax,[menubar_count]
	jae	no_menubar_item
	mov	ecx,[menubar_positions+eax*4]
	cmp	edx,ecx
	jl	no_menubar_item
	add	ecx,[menubar_widths+eax*4]
	cmp	edx,ecx
	jl	menubar_item_found
	inc	eax
	jmp	check_menubar_item
    no_menubar_item:
	or	eax,-1
    menubar_item_found:
	retn

; menubar_pointer_event: edi = window entry (menu bar window)

menubar_pointer_event:
	mov	eax,[edi]
	cmp	eax,[menubar_window]
	jne	menubar_pointer_done
	mov	eax,[cm_y]
	cmp	eax,0
	jl	menubar_outside
	cmp	eax,[menubar_height]
	jge	menubar_outside
	call	menubar_item_at_pointer
	cmp	[cm_event],X11_ButtonPress
	je	menubar_press
	cmp	[cm_event],X11_MotionNotify
	jne	menubar_pointer_done
	cmp	[menu_active],0
	je	menubar_pointer_done
	cmp	[menu_bar_index],-1
	je	menubar_pointer_done
	cmp	eax,-1
	je	menubar_pointer_done
	cmp	eax,[menu_bar_index]
	je	menubar_pointer_done
	mov	[menu_open_by_keyboard],0
	jmp	switch_menu_bar_popup
    menubar_press:
	cmp	[cm_button],1
	jne	menubar_pointer_done
	cmp	eax,-1
	je	close_menu
	cmp	[menu_active],0
	je	menubar_open_by_click
	cmp	eax,[menu_bar_index]
	je	close_menu
	mov	[menu_open_by_keyboard],0
	mov	[menu_ignore_release],0
	jmp	switch_menu_bar_popup
    menubar_open_by_click:
	mov	[menu_open_by_keyboard],0
	mov	[menu_ignore_release],0
	jmp	open_menubar_popup
    menubar_outside:
	cmp	[cm_event],X11_ButtonPress
	je	close_menu
    menubar_pointer_done:
	retn

; open_menubar_popup: eax = index of menu bar entry

open_menubar_popup:
	cmp	eax,[menubar_count]
	jae	menubar_pointer_done
	mov	[menu_bar_index],eax
	or	[menu_selected],-1
	mov	esi,[menubar_items+eax*8+4]
	mov	ecx,[menubar_positions+eax*4]
	mov	edx,[menubar_height]
	push	esi
	mov	eax,[menubar_window]
	call	x11_translate_coordinates
	pop	esi
	call	open_menu
	call	menubar_redraw
	cmp	[menu_open_by_keyboard],0
	je	menubar_popup_opened
	or	ecx,-1
	mov	edx,1
	push	edx
	mov	ebx,[menu_count]
	jmp	menu_key_next_entry
    menubar_popup_opened:
	retn

; open_menu_by_keyboard: eax = index of menu bar entry

open_menu_by_keyboard:
	mov	[menu_open_by_keyboard],1
	mov	[menu_ignore_release],0
	cmp	[menu_active],0
	je	open_menubar_popup
	jmp	switch_menu_bar_popup

; menubar_mnemonic: eax = lower case letter, returns CF=0 and eax = index

menubar_mnemonic:
	push	ecx edx esi
	mov	edx,eax
	xor	ecx,ecx
    search_menubar_mnemonic:
	cmp	dword [menubar_items+ecx*8],0
	je	menubar_mnemonic_missing
	mov	esi,[menubar_items+ecx*8]
	call	text_mnemonic_character
	cmp	eax,edx
	je	menubar_mnemonic_found
	inc	ecx
	jmp	search_menubar_mnemonic
    menubar_mnemonic_missing:
	pop	esi edx ecx
	stc
	retn
    menubar_mnemonic_found:
	mov	eax,ecx
	pop	esi edx ecx
	clc
	retn

; set_menu_state: eax = command id, ecx = state mask, edx = state bits

set_menu_state:
	push	ebx esi
	mov	ebx,menus_list
    next_menu_for_state:
	mov	esi,[ebx]
	test	esi,esi
	jz	menu_state_set
	add	ebx,4
    check_menu_item_state:
	cmp	dword [esi],-1
	je	next_menu_for_state
	cmp	[esi],eax
	jne	next_item_for_state
	test	dword [esi+8],MF_SEPARATOR
	jnz	next_item_for_state
	push	ecx
	not	ecx
	and	[esi+12],ecx
	pop	ecx
	push	edx
	and	edx,ecx
	or	[esi+12],edx
	pop	edx
    next_item_for_state:
	add	esi,MENU_ITEM_SIZE
	jmp	check_menu_item_state
    menu_state_set:
	pop	esi ebx
	retn
