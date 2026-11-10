
; flat editor core
; Copyright (c) 1999-2015, Tomasz Grysztar.
; All rights reserved.

; Linux port: the searched text is a zero-terminated string of 32-bit
; code points. The table of character shifts is indexed by the low byte
; of a character (a value of 0 is mapped to 0FFh, because the first
; entry of the table holds the length of the searched text).

find_first:
	mov	[search_flags],eax
	call	release_search_data
	or	esi,esi
	jz	nothing_to_search_for
	mov	edi,esi
	xor	eax,eax
	or	ecx,-1
	repne	scasd
	lea	ebx,[edi-4]
	sub	ebx,esi
	shr	ebx,2
	jz	nothing_to_search_for
	lea	ecx,[ebx*8+256*4]
	push	ebx
	call	get_memory
	or	eax,eax
	jz	not_enough_memory
	mov	[search_data],eax
	mov	[search_handle],ebx
	pop	ebx
	mov	edi,eax
	lea	ecx,[256+ebx]
	mov	eax,ebx
	rep	stosd
	mov	ecx,ebx
	mov	ebx,[search_data]
	mov	edx,ecx
	test	[search_flags],FEFIND_BACKWARD
	jnz	text_for_backward_search
	test	[search_flags],FEFIND_CASESENSITIVE
	jnz	copy_search_text
    convert_search_text:
	lodsd
	call	upper_case_character
	stosd
	loop	convert_search_text
	jmp	make_character_shifts_table
    copy_search_text:
	rep	movsd
	jmp	make_character_shifts_table
    text_for_backward_search:
	lea	edi,[edi+ecx*4]
	mov	edx,ecx
    reverse_search_text:
	lodsd
	test	[search_flags],FEFIND_CASESENSITIVE
	jnz	reverse_store_character
	call	upper_case_character
    reverse_store_character:
	sub	edi,4
	mov	[edi],eax
	dec	ecx
	jnz	reverse_search_text
	lea	edi,[edi+edx*4]
	xor	ecx,ecx
    make_character_shifts_table:
	cmp	edx,ecx
	je	character_shifts_table_ok
	sub	edi,4
	jecxz	character_shift_ok
	mov	eax,[edi]
	call	character_hash
	cmp	[ebx+eax*4],edx
	jne	character_shift_ok
	mov	[ebx+eax*4],ecx
    character_shift_ok:
	inc	ecx
	jmp	make_character_shifts_table
    character_shifts_table_ok:
	lea	edi,[ebx+(256+ecx)*4]
	push	edi
	lea	edi,[edi+ecx*4-8]
	mov	eax,[edi+4]
	call	character_hash
	mov	edx,[ebx+eax*4]
	mov	[ebx+256*4],edx
	cmp	ecx,1
	je	suffix_match_shifts_table_done
	mov	ecx,2
	lea	eax,[edx*4]
	mov	esi,edi
	sub	esi,eax
    make_suffix_match_shifts_table:
	cmp	esi,[esp]
	jb	store_suffix_match_shift
	mov	eax,[esi]
	cmp	eax,[edi]
	je	store_suffix_match_shift
    find_suffix_match:
	sub	esi,4
	inc	edx
	cmp	esi,[esp]
	jb	match_part_of_suffix
	push	ecx esi edi
	repe	cmpsd
	pop	edi esi ecx
	jne	find_suffix_match
	jmp	store_suffix_match_shift
    match_part_of_suffix:
	mov	eax,[esp]
	push	ecx esi edi
	xchg	eax,esi
	sub	eax,esi
	sar	eax,2
	add	ecx,eax
	repe	cmpsd
	pop	edi esi ecx
	jne	suffix_match_shifts_table_done
    store_suffix_match_shift:
	mov	[ebx+256*4+(ecx-1)*4],edx
	sub	esi,4
	sub	edi,4
	inc	ecx
	cmp	ecx,[ebx]
	jbe	make_suffix_match_shifts_table
    suffix_match_shifts_table_done:
	pop	eax

find_next:
	mov	edi,[search_data]
	or	edi,edi
	jz	nothing_to_search_for
	push	[caret_line]
	push	[caret_line_number]
	push	[caret_position]
	push	[selection_position]
	test	[search_flags],FEFIND_BACKWARD
	jnz	search_backward
	test	[search_flags],FEFIND_INWHOLETEXT
	jz	search_for_text
	mov	eax,[first_line]
	mov	[caret_line],eax
	xor	eax,eax
	mov	[caret_position],eax
	inc	eax
	mov	[caret_line_number],eax
    search_for_text:
	mov	ecx,[edi]
	dec	ecx
	add	ecx,[caret_position]
	mov	esi,[caret_line]
	cmp	ecx,[peak_line_length]
	jae	text_not_in_this_line
	mov	[caret_position],ecx
	call	get_caret_segment
	mov	edi,[search_data]
	mov	eax,[edi]
	lea	ebx,[edi+(256+eax)*4]
	mov	ebx,[ebx+eax*4-4]
    search_in_line:
	cmp	edx,SEGMENT_DATA_CHARS
	jae	text_not_in_this_line
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	test	[search_flags],FEFIND_CASESENSITIVE
	jnz	compare_last_character
	call	upper_case_character
    compare_last_character:
	cmp	eax,ebx
	je	partial_match
    mismatch_shift:
	call	character_hash
	mov	ecx,[edi+eax*4]
    shift_search_position:
	add	edx,ecx
	add	ecx,[caret_position]
	cmp	ecx,[peak_line_length]
	jae	text_not_in_this_line
	mov	[caret_position],ecx
    check_search_position:
	cmp	edx,SEGMENT_DATA_CHARS
	jb	search_in_line
	mov	ecx,[esi]
	btr	ecx,0
	jnc	search_in_line
	sub	edx,SEGMENT_DATA_CHARS
	mov	esi,ecx
	jmp	check_search_position
    partial_match:
	mov	ecx,[edi]
	dec	ecx
	jz	text_found
	push	edi
	lea	edi,[edi+(256+ecx+1)*4]
	lea	edi,[edi+ecx*4]
    compare_text:
	sub	edx,1
	jc	compare_in_previous_segment
	sub	edi,4
	mov	eax,20h
	cmp	edx,SEGMENT_DATA_CHARS
	jae	compare_character
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	test	[search_flags],FEFIND_CASESENSITIVE
	jnz	compare_character
	call	upper_case_character
    compare_character:
	cmp	eax,[edi]
	loope	compare_text
	pop	edi
	je	text_found
	neg	ecx
	add	ecx,[edi]
	dec	ecx
	add	edx,ecx
	mov	ecx,[edi+(256+ecx-1)*4]
	jmp	shift_search_position
    compare_in_previous_segment:
	mov	esi,[esi+4]
	and	esi,not 1
	mov	edx,SEGMENT_DATA_CHARS
	jmp	compare_text
    text_not_in_this_line:
	mov	esi,[esi]
	or	esi,esi
	jz	text_not_found
	btr	esi,0
	jc	text_not_in_this_line
    search_in_next_line:
	mov	[caret_line],esi
	inc	[caret_line_number]
	mov	[caret_position],0
	mov	edi,[search_data]
	jmp	search_for_text
    text_found:
	mov	eax,[caret_position]
	inc	eax
	mov	[selection_position],eax
	sub	eax,[edi]
	mov	[caret_position],eax
	jz	left_text_edge_ok
	test	[search_flags],FEFIND_WHOLEWORDS
	jz	left_text_edge_ok
	mov	edi,[search_data]
	mov	ecx,[edi]
	mov	eax,[edi+(256+ecx)*4]
	call	recognize_character
	jc	left_text_edge_ok
	dec	[caret_position]
	call	get_caret_segment
	inc	[caret_position]
	cmp	edx,SEGMENT_DATA_CHARS
	jae	left_text_edge_ok
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	call	recognize_character
	jnc	found_not_acceptable
    left_text_edge_ok:
	mov	eax,[caret_position]
	xchg	eax,[selection_position]
	mov	[caret_position],eax
	mov	edi,[search_data]
	mov	ecx,[edi]
	lea	edi,[edi+(256+ecx)*4]
	mov	eax,[edi+ecx*4-4]
	cmp	eax,20h
	je	right_text_edge_blank
	test	[search_flags],FEFIND_WHOLEWORDS
	jz	right_text_edge_ok
	call	recognize_character
	jc	right_text_edge_ok
	call	get_caret_segment
	cmp	edx,SEGMENT_DATA_CHARS
	jae	right_text_edge_ok
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	call	recognize_character
	jc	right_text_edge_ok
	mov	eax,[selection_position]
	mov	[caret_position],eax
    found_not_acceptable:
	mov	edi,[search_data]
	mov	ecx,[edi]
	mov	eax,[edi+(256+ecx-1)*4]
	add	[caret_position],eax
	jmp	search_for_text
    right_text_edge_blank:
	call	get_caret_segment
    check_for_blank_end:
	mov	ecx,SEGMENT_DATA_CHARS
	sub	ecx,edx
	jz	blank_end_next_segment
	push	edi
	mov	eax,20h
	lea	edi,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	repe	scasd
	pop	edi
	jne	right_text_edge_ok
    blank_end_next_segment:
	mov	esi,[esi]
	or	esi,esi
	jz	text_not_found
	btr	esi,0
	jnc	search_in_next_line
	xor	edx,edx
	jmp	check_for_blank_end
    right_text_edge_ok:
	mov	eax,[caret_line]
	mov	ecx,[caret_line_number]
	mov	[selection_line],eax
	mov	[selection_line_number],ecx
	and	[search_flags],not FEFIND_INWHOLETEXT
	add	esp,16
	clc
	retn
    text_not_found:
	pop	[selection_position]
	pop	[caret_position]
	pop	[caret_line_number]
	pop	[caret_line]
    nothing_to_search_for:
	stc
	retn
    search_backward:
	test	[search_flags],FEFIND_INWHOLETEXT
	jz	backward_search_for_text
	or	eax,-1
	call	find_line
	mov	[caret_line],esi
	mov	[caret_line_number],ecx
	call	move_to_line_end
    backward_search_for_text:
	mov	ecx,[caret_position]
	sub	ecx,1
	jc	backward_text_not_in_this_line
	mov	[caret_position],ecx
	mov	edi,[search_data]
	mov	eax,[edi]
	mov	eax,[edi+(256+eax)*4]
	cmp	eax,20h
	jne	backward_search_starting_position_ok
	mov	esi,[caret_line]
	mov	ecx,[esi+8]
	mov	edi,[search_data]
	mov	eax,[edi]
	sub	ecx,eax
	jc	backward_text_not_in_this_line
	cmp	ecx,[caret_position]
	jae	backward_search_starting_position_ok
	mov	[caret_position],ecx
    backward_search_starting_position_ok:
	call	get_caret_segment
	mov	edi,[search_data]
	mov	eax,[edi]
	lea	ebx,[edi+(256+eax)*4]
	mov	ebx,[ebx+eax*4-4]
	cmp	edx,SEGMENT_DATA_CHARS
	jb	backward_search_in_line
	mov	ecx,edx
	sub	ecx,SEGMENT_DATA_CHARS-1
	sub	edx,ecx
	sub	[caret_position],ecx
    backward_search_in_line:
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	test	[search_flags],FEFIND_CASESENSITIVE
	jnz	compare_first_character
	call	upper_case_character
    compare_first_character:
	cmp	eax,ebx
	je	backward_partial_match
    backward_mismatch_shift:
	call	character_hash
	mov	ecx,[edi+eax*4]
    shift_backward_search_position:
	sub	edx,ecx
	sub	[caret_position],ecx
	jc	backward_text_not_in_this_line
    check_backward_search_position:
	cmp	edx,0
	jge	backward_search_in_line
	mov	esi,[esi+4]
	and	esi,not 1
	add	edx,SEGMENT_DATA_CHARS
	jmp	check_backward_search_position
    backward_partial_match:
	mov	ecx,[edi]
	dec	ecx
	jz	backward_text_found
	push	edi
	lea	edi,[edi+(256+ecx+1)*4]
	lea	edi,[edi+ecx*4]
    backward_compare_text:
	inc	edx
	cmp	edx,SEGMENT_DATA_CHARS
	jae	compare_in_next_segment
	sub	edi,4
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	test	[search_flags],FEFIND_CASESENSITIVE
	jnz	backward_compare_character
	call	upper_case_character
    backward_compare_character:
	cmp	eax,[edi]
	loope	backward_compare_text
	pop	edi
	je	backward_text_found
	neg	ecx
	add	ecx,[edi]
	dec	ecx
	sub	edx,ecx
	mov	ecx,[edi+(256+ecx)*4]
	jmp	shift_backward_search_position
    compare_in_next_segment:
	sub	edx,SEGMENT_DATA_CHARS
	mov	esi,[esi]
	btr	esi,0
	jnc	compare_blank_space
	dec	edx
	jmp	backward_compare_text
    compare_blank_space:
	pop	edi
	cmp	ecx,[edi]
	jbe	backward_text_found
    backward_text_not_in_this_line:
	mov	esi,[caret_line]
	mov	esi,[esi+4]
	or	esi,esi
	jz	text_not_found
	mov	[caret_line],esi
	dec	[caret_line_number]
	mov	ecx,[peak_line_length]
	mov	[caret_position],ecx
	jmp	backward_search_for_text
    backward_text_found:
	test	[search_flags],FEFIND_WHOLEWORDS
	jz	backward_left_text_edge_ok
	cmp	[caret_position],0
	je	backward_left_text_edge_ok
	mov	edi,[search_data]
	mov	ecx,[edi]
	lea	edi,[edi+(256+ecx)*4]
	mov	eax,[edi+ecx*4-4]
	call	recognize_character
	jc	backward_left_text_edge_ok
	dec	[caret_position]
	call	get_caret_segment
	inc	[caret_position]
	cmp	edx,SEGMENT_DATA_CHARS
	jae	backward_left_text_edge_ok
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	call	recognize_character
	jnc	backward_found_not_acceptable
    backward_left_text_edge_ok:
	mov	eax,[caret_position]
	mov	[selection_position],eax
	mov	edi,[search_data]
	mov	ecx,[edi]
	add	eax,ecx
	mov	[caret_position],eax
	test	[search_flags],FEFIND_WHOLEWORDS
	jz	backward_right_text_edge_ok
	mov	eax,[edi+(256+ecx)*4]
	call	recognize_character
	jc	backward_right_text_edge_ok
	call	get_caret_segment
	cmp	edx,SEGMENT_DATA_CHARS
	jae	backward_right_text_edge_ok
	mov	eax,[esi+SEGMENT_HEADER_LENGTH+edx*4]
	call	recognize_character
	jc	backward_right_text_edge_ok
	mov	eax,[selection_position]
	mov	[caret_position],eax
    backward_found_not_acceptable:
	mov	edi,[search_data]
	mov	ecx,[edi]
	mov	eax,[edi+(256+ecx-1)*4]
	sub	[caret_position],eax
	jbe	backward_text_not_in_this_line
	jmp	backward_search_for_text
    backward_right_text_edge_ok:
	mov	eax,[caret_position]
	xchg	eax,[selection_position]
	mov	[caret_position],eax
	mov	eax,[caret_line]
	mov	ecx,[caret_line_number]
	mov	[selection_line],eax
	mov	[selection_line_number],ecx
	and	[search_flags],not FEFIND_INWHOLETEXT
	add	esp,16
	clc
	retn

get_search_text_length:
	mov	ecx,[search_data]
	test	ecx,ecx
	jz	no_search_text
	mov	ecx,[ecx]
	clc
	retn
get_search_text:
	mov	esi,[search_data]
	test	esi,esi
	jz	no_search_text
	mov	ecx,[esi]
	lea	esi,[esi+(256+ecx)*4]
	test	[search_flags],FEFIND_BACKWARD
	jnz	copy_inverted_text
	rep	movsd
	xor	eax,eax
	stosd
	clc
	retn
    copy_inverted_text:
	mov	eax,[esi+ecx*4-4]
	stosd
	loop	copy_inverted_text
	xor	eax,eax
	stosd
	clc
	retn
    no_search_text:
	stc
	retn

release_search_data:
	mov	edi,[search_data]
	test	edi,edi
	jz	search_data_released
	mov	ebx,[search_handle]
	call	release_memory
	mov	[search_data],0
    search_data_released:
	retn

character_hash:
	and	eax,0FFh
	jnz	character_hash_ok
	mov	al,0FFh
    character_hash_ok:
	retn

upper_case_character:
	cmp	eax,100h
	jae	upper_case_ok
	movzx	eax,byte [upper_case_table+eax]
    upper_case_ok:
	retn
