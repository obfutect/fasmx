
; fasmx - flat assembler IDE for Linux/X11
; keyboard mapping (core protocol keysyms)

XK_BackSpace	= 0FF08h
XK_Tab		= 0FF09h
XK_Return	= 0FF0Dh
XK_Escape	= 0FF1Bh
XK_Home 	= 0FF50h
XK_Left 	= 0FF51h
XK_Up		= 0FF52h
XK_Right	= 0FF53h
XK_Down 	= 0FF54h
XK_Prior	= 0FF55h
XK_Next 	= 0FF56h
XK_End		= 0FF57h
XK_Insert	= 0FF63h
XK_Menu 	= 0FF67h
XK_Mode_switch	= 0FF7Eh
XK_Num_Lock	= 0FF7Fh
XK_KP_Space	= 0FF80h
XK_KP_Tab	= 0FF89h
XK_KP_Enter	= 0FF8Dh
XK_KP_Home	= 0FF95h
XK_KP_Left	= 0FF96h
XK_KP_Up	= 0FF97h
XK_KP_Right	= 0FF98h
XK_KP_Down	= 0FF99h
XK_KP_Prior	= 0FF9Ah
XK_KP_Next	= 0FF9Bh
XK_KP_End	= 0FF9Ch
XK_KP_Begin	= 0FF9Dh
XK_KP_Insert	= 0FF9Eh
XK_KP_Delete	= 0FF9Fh
XK_KP_Equal	= 0FFBDh
XK_KP_Multiply	= 0FFAAh
XK_KP_9 	= 0FFB9h
XK_F6		= 0FFC3h
XK_Meta_L	= 0FFE7h
XK_Meta_R	= 0FFE8h
XK_Alt_L	= 0FFE9h
XK_Alt_R	= 0FFEAh
XK_Delete	= 0FFFFh
XK_ISO_Level3_Shift = 0FE03h
XK_ISO_Left_Tab = 0FE20h
XK_dead_grave	= 0FE50h
XK_dead_ogonek	= 0FE5Ch

X11_ShiftMask	= 1
X11_LockMask	= 2
X11_ControlMask = 4

; x11_load_keyboard_mapping: reads keysyms and modifier assignments

x11_load_keyboard_mapping:
	push	ebx ecx edx esi edi
	mov	ecx,8
	call	x11_begin_request
	mov	dword [edi],101 + 2 shl 16
	mov	dl,[x11_min_keycode]
	mov	[edi+4],dl
	mov	dh,[x11_max_keycode]
	sub	dh,dl
	inc	dh
	mov	[edi+5],dh
	mov	word [edi+6],0
	call	x11_wait_reply
	jc	keyboard_mapping_failed
	movzx	eax,byte [esi+1]
	mov	[x11_keysyms_per_keycode],eax
	mov	ecx,[esi+4]
	cmp	ecx,(X11_PACKET_SIZE-32)/4
	jbe	keysyms_count_ok
	mov	ecx,(X11_PACKET_SIZE-32)/4
    keysyms_count_ok:
	mov	[x11_keysyms_count],ecx
	add	esi,32
	mov	edi,x11_keymap
	rep	movsd
	mov	ecx,4
	call	x11_begin_request
	mov	dword [edi],119 + 1 shl 16
	call	x11_wait_reply
	jc	keyboard_mapping_failed
	xor	eax,eax
	mov	[x11_alt_mask],eax
	mov	[x11_numlock_mask],eax
	mov	[x11_level3_mask],eax
	mov	[x11_mode_switch_mask],eax
	movzx	ebx,byte [esi+1]
	lea	edi,[esi+32]
	xor	edx,edx
    scan_modifiers:
	xor	ecx,ecx
    scan_modifier_keys:
	cmp	ecx,ebx
	jae	next_modifier
	movzx	eax,byte [edi]
	inc	edi
	inc	ecx
	cmp	edx,3
	jb	scan_modifier_keys
	test	eax,eax
	jz	scan_modifier_keys
	call	assign_modifier_key
	jmp	scan_modifier_keys
    next_modifier:
	inc	edx
	cmp	edx,8
	jb	scan_modifiers
	cmp	[x11_alt_mask],0
	jne	keyboard_mapping_ok
	mov	[x11_alt_mask],8
    keyboard_mapping_ok:
	pop	edi esi edx ecx ebx
	clc
	retn
    keyboard_mapping_failed:
	mov	[x11_keysyms_per_keycode],0
	mov	[x11_alt_mask],8
	pop	edi esi edx ecx ebx
	stc
	retn
    assign_modifier_key:
	push	ebx ecx esi
	movzx	ecx,[x11_min_keycode]
	sub	eax,ecx
	jb	modifier_key_assigned
	mov	ecx,[x11_keysyms_per_keycode]
	imul	eax,ecx
	lea	esi,[eax+ecx]
	cmp	esi,[x11_keysyms_count]
	ja	modifier_key_assigned
	lea	esi,[x11_keymap+eax*4]
	mov	ebx,1
	xchg	ecx,edx
	shl	ebx,cl
	xchg	ecx,edx
	jecxz	modifier_key_assigned
    check_modifier_keysym:
	lodsd
	cmp	eax,XK_Mode_switch
	je	mode_switch_modifier
	cmp	eax,XK_ISO_Level3_Shift
	je	level3_modifier
	cmp	eax,XK_Num_Lock
	je	numlock_modifier
	cmp	eax,XK_Alt_L
	je	alt_modifier
	cmp	eax,XK_Alt_R
	je	alt_modifier
	cmp	eax,XK_Meta_L
	je	alt_modifier
	cmp	eax,XK_Meta_R
	je	alt_modifier
    next_modifier_keysym:
	loop	check_modifier_keysym
    modifier_key_assigned:
	pop	esi ecx ebx
	retn
    mode_switch_modifier:
	or	[x11_mode_switch_mask],ebx
	jmp	next_modifier_keysym
    level3_modifier:
	or	[x11_level3_mask],ebx
	jmp	next_modifier_keysym
    numlock_modifier:
	or	[x11_numlock_mask],ebx
	jmp	next_modifier_keysym
    alt_modifier:
	or	[x11_alt_mask],ebx
	jmp	next_modifier_keysym

; lookup_keysym: eax = keycode, edx = state
; returns eax = keysym (0 if none)

lookup_keysym:
	push	ebx ecx edx esi edi
	mov	[lookup_state],edx
	call	get_keysym_list
	jc	no_keysym
	mov	edx,[lookup_state]
	mov	ecx,edx
	shr	ecx,13
	and	ecx,3
	jnz	keyboard_group_ok
	test	edx,[x11_mode_switch_mask]
	jz	keyboard_group_ok
	mov	ecx,1
    keyboard_group_ok:
	cmp	ecx,1
	jbe	keyboard_group_supported
	mov	ecx,1
    keyboard_group_supported:
	shl	ecx,1
	xor	eax,eax
	test	edx,[x11_level3_mask]
	jz	keyboard_level_ok
	mov	eax,4
    keyboard_level_ok:
	mov	[lookup_group_column],ecx
	mov	[lookup_level_column],eax
	add	ecx,eax
	call	get_keysym_pair
	jnc	keysym_pair_found
	cmp	[lookup_level_column],0
	je	try_first_group
	mov	ecx,[lookup_group_column]
	call	get_keysym_pair
	jnc	keysym_pair_found
    try_first_group:
	cmp	[lookup_group_column],0
	je	no_keysym
	mov	ecx,[lookup_level_column]
	call	get_keysym_pair
	jnc	keysym_pair_found
	xor	ecx,ecx
	call	get_keysym_pair
	jc	no_keysym
    keysym_pair_found:
	mov	ecx,[lookup_state]
	test	ecx,[x11_numlock_mask]
	jz	no_keypad_keysym
	cmp	edx,XK_KP_Space
	jb	no_keypad_keysym
	cmp	edx,XK_KP_Equal
	ja	no_keypad_keysym
	test	ecx,X11_ShiftMask
	jnz	first_keysym_selected
	mov	eax,edx
	jmp	keysym_selected
    no_keypad_keysym:
	test	ecx,X11_LockMask
	jz	no_caps_lock
	push	eax edx
	call	keysym_case
	cmp	eax,edx
	pop	edx eax
	je	no_caps_lock
	test	ecx,X11_ShiftMask
	jnz	first_keysym_selected
	mov	eax,edx
	jmp	keysym_selected
    no_caps_lock:
	test	ecx,X11_ShiftMask
	jz	first_keysym_selected
	mov	eax,edx
    first_keysym_selected:
    keysym_selected:
	pop	edi esi edx ecx ebx
	retn
    no_keysym:
	xor	eax,eax
	pop	edi esi edx ecx ebx
	retn

; lookup_base_keysym: eax = keycode, edx = state
; returns eax = keysym from the first group (for keyboard shortcuts)

lookup_base_keysym:
	push	ebx ecx edx esi edi
	mov	[lookup_state],edx
	call	get_keysym_list
	jc	no_keysym
	xor	ecx,ecx
	call	get_keysym_pair
	jc	no_keysym
	test	[lookup_state],X11_ShiftMask
	jz	keysym_selected
	mov	eax,edx
	jmp	keysym_selected

; get_keysym_list: eax = keycode
; returns esi = list of keysyms, ebx = number of keysyms

get_keysym_list:
	movzx	ecx,[x11_min_keycode]
	sub	eax,ecx
	jb	keysym_list_missing
	mov	ebx,[x11_keysyms_per_keycode]
	test	ebx,ebx
	jz	keysym_list_missing
	imul	eax,ebx
	lea	ecx,[eax+ebx]
	cmp	ecx,[x11_keysyms_count]
	ja	keysym_list_missing
	lea	esi,[x11_keymap+eax*4]
	clc
	retn
    keysym_list_missing:
	stc
	retn

; get_keysym_pair: esi = list of keysyms, ebx = number, ecx = column
; returns CF=0 and eax = unshifted keysym, edx = shifted keysym

get_keysym_pair:
	xor	eax,eax
	xor	edx,edx
	cmp	ecx,ebx
	jae	keysym_pair_read
	mov	eax,[esi+ecx*4]
	inc	ecx
	cmp	ecx,ebx
	jae	keysym_pair_read
	mov	edx,[esi+ecx*4]
    keysym_pair_read:
	test	eax,eax
	jnz	keysym_pair_ok
	test	edx,edx
	jz	keysym_pair_empty
	mov	eax,edx
	retn
    keysym_pair_ok:
	test	edx,edx
	jnz	keysym_pair_complete
	call	keysym_case
    keysym_pair_complete:
	clc
	retn
    keysym_pair_empty:
	stc
	retn

; keysym_case: eax = keysym
; returns eax = lower case keysym, edx = upper case keysym

keysym_case:
	mov	edx,eax
	cmp	eax,'A'
	jb	keysym_case_done
	cmp	eax,'Z'
	jbe	upper_latin_keysym
	cmp	eax,'a'
	jb	keysym_case_done
	cmp	eax,'z'
	jbe	lower_latin_keysym
	cmp	eax,0C0h
	jb	keysym_case_done
	cmp	eax,0DEh
	jbe	upper_latin1_keysym
	cmp	eax,0E0h
	jb	keysym_case_done
	cmp	eax,0FEh
	jbe	lower_latin1_keysym
	cmp	eax,6A1h
	jb	keysym_case_done
	cmp	eax,6AFh
	jbe	lower_cyrillic_keysym
	cmp	eax,6B1h
	jb	keysym_case_done
	cmp	eax,6BFh
	jbe	upper_cyrillic_keysym
	cmp	eax,6C0h
	jb	keysym_case_done
	cmp	eax,6DFh
	jbe	lower_cyrillic_keysym
	cmp	eax,6FFh
	jbe	upper_cyrillic_keysym
    keysym_case_done:
	retn
    upper_latin1_keysym:
	cmp	eax,0D7h
	je	keysym_case_done
    upper_latin_keysym:
	add	eax,20h
	retn
    lower_latin1_keysym:
	cmp	eax,0F7h
	je	keysym_case_done
    lower_latin_keysym:
	sub	edx,20h
	retn
    lower_cyrillic_keysym:
	add	edx,10h
	cmp	eax,6C0h
	jb	keysym_case_done
	add	edx,10h
	retn
    upper_cyrillic_keysym:
	sub	eax,10h
	cmp	edx,6E0h
	jb	keysym_case_done
	sub	eax,10h
	retn

; keysym_to_character: eax = keysym
; returns eax = Unicode code point (0 if keysym is not a character)

keysym_to_character:
	cmp	eax,20h
	jb	not_a_character
	cmp	eax,7Eh
	jbe	character_found
	cmp	eax,0A0h
	jb	not_a_character
	cmp	eax,0FFh
	jbe	character_found
	cmp	eax,1A1h
	jb	not_a_character
	cmp	eax,1FFh
	jbe	latin2_keysym
	cmp	eax,6A1h
	jb	not_a_character
	cmp	eax,6FFh
	jbe	cyrillic_keysym
	cmp	eax,20ACh
	je	character_found
	cmp	eax,XK_KP_Space
	je	keypad_space
	cmp	eax,XK_KP_Multiply
	jb	not_a_character
	cmp	eax,XK_KP_9
	jbe	keypad_character
	cmp	eax,XK_KP_Equal
	je	keypad_equal
	mov	edx,eax
	and	edx,0FF000000h
	cmp	edx,01000000h
	jne	not_a_character
	and	eax,0FFFFFFh
	cmp	eax,10FFFFh
	ja	not_a_character
	cmp	eax,0D800h
	jb	character_found
	cmp	eax,0DFFFh
	jbe	not_a_character
    character_found:
	retn
    latin2_keysym:
	movzx	eax,word [latin2_keysyms+(eax-1A1h)*2]
	retn
    cyrillic_keysym:
	movzx	eax,word [cyrillic_keysyms+(eax-6A1h)*2]
	retn
    keypad_space:
	mov	eax,20h
	retn
    keypad_equal:
	mov	eax,'='
	retn
    keypad_character:
	sub	eax,XK_KP_Multiply-'*'
	retn
    not_a_character:
	xor	eax,eax
	retn

; dead_keysym_to_character: eax = keysym
; returns eax = spacing character for a dead key (0 if not a dead key)

dead_keysym_to_character:
	cmp	eax,XK_dead_grave
	jb	not_a_character
	cmp	eax,XK_dead_ogonek
	ja	not_a_character
	movzx	eax,word [dead_keysyms+(eax-XK_dead_grave)*2]
	retn
