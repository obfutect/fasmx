
; fasmx - flat assembler IDE for Linux/X11
; syntax highlighting of assembly source (port of fasm_syntax from FASMW)

; init_character_tables: prepares the case conversion table used by search
; and the table of symbol characters used for syntax highlighting

init_character_tables:
	xor	ecx,ecx
    make_upper_case_table:
	mov	eax,ecx
	cmp	eax,'a'
	jb	upper_case_entry
	cmp	eax,'z'
	jbe	convert_to_upper_case
	cmp	eax,0E0h
	jb	upper_case_entry
	cmp	eax,0F7h
	je	upper_case_entry
	cmp	eax,0FFh
	je	upper_case_entry
    convert_to_upper_case:
	sub	eax,20h
    upper_case_entry:
	mov	[upper_case_table+ecx],al
	inc	ecx
	cmp	ecx,100h
	jb	make_upper_case_table
	mov	edi,syntax_symbols
	xor	eax,eax
	mov	ecx,100h/4
	rep	stosd
	mov	esi,symbol_characters+1
	movzx	ecx,byte [symbol_characters]
    mark_syntax_symbols:
	movzx	eax,byte [esi]
	inc	esi
	mov	byte [syntax_symbols+eax],1
	loop	mark_syntax_symbols
	mov	edi,characters
	mov	ecx,100h
	xor	al,al
    prepare_characters_table:
	stosb
	inc	al
	loop	prepare_characters_table
	mov	esi,characters+'a'
	mov	edi,characters+'A'
	mov	ecx,26
	rep	movsb
	mov	edi,characters
	mov	esi,symbol_characters+1
	movzx	ecx,byte [esi-1]
	xor	eax,eax
    convert_characters_table:
	lodsb
	mov	byte [edi+eax],0
	loop	convert_characters_table
	retn

; fasm_syntax: esi = cells, ecx = number of cells, edi = colors (zeroed)
; preserves ebx, esi, edi, ebp

fasm_syntax:
	push	ebx esi edi
	test	ecx,ecx
	jz	.done
	xor	edx,edx
  .scan_syntax:
	lodsd
  .check_character:
	cmp	eax,20h
	je	.syntax_space
	cmp	eax,3Bh
	je	.syntax_comment
	cmp	eax,100h
	jae	.not_symbol
	cmp	byte [syntax_symbols+eax],0
	jne	.syntax_symbol
  .not_symbol:
	or	edx,edx
	jnz	.syntax_neutral
	cmp	eax,27h
	je	.syntax_string
	cmp	eax,22h
	je	.syntax_string
	cmp	eax,24h
	je	.syntax_pascal_hex
	cmp	eax,39h
	ja	.syntax_neutral
	cmp	eax,30h
	jae	.syntax_number
  .syntax_neutral:
	or	edx,-1
	inc	edi
	loop	.scan_syntax
	jmp	.done
  .syntax_space:
	xor	edx,edx
	inc	edi
	loop	.scan_syntax
	jmp	.done
  .syntax_symbol:
	mov	byte [edi],1
	inc	edi
	xor	edx,edx
	loop	.scan_syntax
	jmp	.done
  .syntax_pascal_hex:
	cmp	ecx,1
	je	.syntax_neutral
	mov	eax,[esi]
	call	.is_symbol
	jc	.syntax_neutral
	cmp	eax,24h
	jne	.syntax_number
	cmp	ecx,2
	je	.syntax_neutral
	mov	eax,[esi+4]
	call	.is_symbol
	jc	.syntax_neutral
  .syntax_number:
	mov	byte [edi],2
	inc	edi
	loop	.number_character
	jmp	.done
  .number_character:
	lodsd
	call	.is_symbol
	jc	.check_character
	mov	byte [edi],2
	inc	edi
	loop	.number_character
	jmp	.done
  .syntax_string:
	mov	ebx,eax
  .string_character:
	mov	byte [edi],3
	inc	edi
	dec	ecx
	jz	.done
	lodsd
	cmp	eax,ebx
	jne	.string_character
	mov	byte [edi],3
	inc	edi
	dec	ecx
	jz	.done
	lodsd
	cmp	eax,ebx
	je	.string_character
	xor	edx,edx
	jmp	.check_character
  .process_comment:
	lodsd
	cmp	eax,20h
	jne	.syntax_comment
	inc	edi
	loop	.process_comment
	jmp	.done
  .syntax_comment:
	mov	byte [edi],4
	inc	edi
	loop	.process_comment
  .done:
	pop	edi esi ebx
	retn
  .is_symbol:
	cmp	eax,100h
	jae	.not_symbol_character
	cmp	byte [syntax_symbols+eax],1
	cmc
	retn
  .not_symbol_character:
	clc
	retn

; no_syntax: syntax procedure that leaves all cells in default color

no_syntax:
	retn
