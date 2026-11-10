
; fasmx - flat assembler IDE for Linux/X11
; conversion between UTF-8 and text cells

; Bytes that do not form a valid UTF-8 sequence (and zero bytes) are kept
; in cells with values 0DC00h-0DCFFh, so that they are written back
; unchanged. Valid UTF-8 never decodes to these values.

ESCAPED_BYTE = 0DC00h

; utf8_to_cells: esi = UTF-8 data, ecx = length, edi = buffer for cells
; (needs (length+1)*4 bytes); writes zero-terminated string of cells
; returns edi = end of cells

utf8_to_cells:
	push	ebx edx ebp
	lea	ebp,[esi+ecx]
    decode_utf8:
	cmp	esi,ebp
	jae	utf8_decoded
	movzx	eax,byte [esi]
	inc	esi
	cmp	eax,80h
	jb	decode_ascii
	cmp	eax,0C2h
	jb	invalid_utf8
	cmp	eax,0E0h
	jb	decode_two_bytes
	cmp	eax,0F0h
	jb	decode_three_bytes
	cmp	eax,0F5h
	jb	decode_four_bytes
    invalid_utf8:
	or	eax,ESCAPED_BYTE
	jmp	store_cell
    decode_ascii:
	test	eax,eax
	jz	invalid_utf8
    store_cell:
	stosd
	jmp	decode_utf8
    decode_two_bytes:
	lea	ebx,[esi+1]
	cmp	ebx,ebp
	ja	invalid_utf8
	movzx	edx,byte [esi]
	mov	bl,dl
	and	bl,0C0h
	cmp	bl,80h
	jne	invalid_utf8
	and	eax,1Fh
	shl	eax,6
	and	edx,3Fh
	or	eax,edx
	inc	esi
	jmp	store_cell
    decode_three_bytes:
	lea	ebx,[esi+2]
	cmp	ebx,ebp
	ja	invalid_utf8
	movzx	edx,byte [esi]
	mov	bl,dl
	and	bl,0C0h
	cmp	bl,80h
	jne	invalid_utf8
	mov	bl,[esi+1]
	and	bl,0C0h
	cmp	bl,80h
	jne	invalid_utf8
	cmp	eax,0E0h
	jne	three_bytes_not_overlong
	cmp	edx,0A0h
	jb	invalid_utf8
    three_bytes_not_overlong:
	cmp	eax,0EDh
	jne	three_bytes_not_surrogate
	cmp	edx,0A0h
	jae	invalid_utf8
    three_bytes_not_surrogate:
	and	eax,0Fh
	shl	eax,12
	and	edx,3Fh
	shl	edx,6
	or	eax,edx
	movzx	edx,byte [esi+1]
	and	edx,3Fh
	or	eax,edx
	add	esi,2
	jmp	store_cell
    decode_four_bytes:
	lea	ebx,[esi+3]
	cmp	ebx,ebp
	ja	invalid_utf8
	movzx	edx,byte [esi]
	mov	bl,dl
	and	bl,0C0h
	cmp	bl,80h
	jne	invalid_utf8
	mov	bl,[esi+1]
	and	bl,0C0h
	cmp	bl,80h
	jne	invalid_utf8
	mov	bl,[esi+2]
	and	bl,0C0h
	cmp	bl,80h
	jne	invalid_utf8
	cmp	eax,0F0h
	jne	four_bytes_not_overlong
	cmp	edx,90h
	jb	invalid_utf8
    four_bytes_not_overlong:
	cmp	eax,0F4h
	jne	four_bytes_in_range
	cmp	edx,90h
	jae	invalid_utf8
    four_bytes_in_range:
	and	eax,7
	shl	eax,18
	and	edx,3Fh
	shl	edx,12
	or	eax,edx
	movzx	edx,byte [esi+1]
	and	edx,3Fh
	shl	edx,6
	or	eax,edx
	movzx	edx,byte [esi+2]
	and	edx,3Fh
	or	eax,edx
	add	esi,3
	jmp	store_cell
    utf8_decoded:
	xor	eax,eax
	stosd
	pop	ebp edx ebx
	retn

; utf8_encode: eax = cell, edi = destination
; returns edi = end of encoded character; preserves all other registers

utf8_encode:
	cmp	eax,80h
	jb	encode_one_byte
	cmp	eax,ESCAPED_BYTE
	jb	encode_character
	cmp	eax,ESCAPED_BYTE+100h
	jb	encode_one_byte
    encode_character:
	push	eax ecx
	cmp	eax,800h
	jb	encode_two_bytes
	cmp	eax,10000h
	jb	encode_three_bytes
	mov	ecx,eax
	shr	ecx,18
	or	cl,0F0h
	mov	[edi],cl
	inc	edi
	mov	ecx,eax
	shr	ecx,12
	and	cl,3Fh
	or	cl,80h
	mov	[edi],cl
	inc	edi
	jmp	encode_last_two_bytes
    encode_three_bytes:
	mov	ecx,eax
	shr	ecx,12
	or	cl,0E0h
	mov	[edi],cl
	inc	edi
    encode_last_two_bytes:
	mov	ecx,eax
	shr	ecx,6
	and	cl,3Fh
	or	cl,80h
	mov	[edi],cl
	inc	edi
	jmp	encode_last_byte
    encode_two_bytes:
	mov	ecx,eax
	shr	ecx,6
	or	cl,0C0h
	mov	[edi],cl
	inc	edi
    encode_last_byte:
	and	al,3Fh
	or	al,80h
	mov	[edi],al
	inc	edi
	pop	ecx eax
	retn
    encode_one_byte:
	mov	[edi],al
	inc	edi
	retn
