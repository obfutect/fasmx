
; fasmx - flat assembler IDE for Linux/X11
; settings in INI file

INI_BUFFER_SIZE = 40000h

; init_settings: locates and reads the settings file

init_settings:
	mov	esi,_config_home_variable
	call	find_environment_variable
	mov	edi,ini_path
	test	eax,eax
	jz	config_in_home
	cmp	byte [eax],0
	je	config_in_home
	mov	esi,eax
	call	copy_text
	jmp	config_directory_ready
    config_in_home:
	mov	esi,_home_variable
	call	find_environment_variable
	test	eax,eax
	jz	config_directory_ready
	mov	esi,eax
	call	copy_text
	mov	esi,_config_directory
	call	copy_text
	mov	eax,SYS_MKDIR
	mov	ebx,ini_path
	mov	ecx,755o
	int	80h
    config_directory_ready:
	mov	esi,_ini_file_name
	call	copy_text
	mov	eax,SYS_READLINK
	mov	ebx,_self_executable
	mov	ecx,path_buffer
	mov	edx,PATH_SIZE-1
	int	80h
	test	eax,eax
	jns	executable_path_ok
	xor	eax,eax
    executable_path_ok:
	mov	byte [path_buffer+eax],0
	mov	esi,path_buffer
	mov	edi,include_path
	call	directory_of_path
	mov	esi,include_path
	call	string_length
	lea	edi,[esi+ecx]
	mov	esi,_include_directory_name
	call	copy_text
	mov	ecx,INI_BUFFER_SIZE
	call	get_memory
	mov	[ini_data],eax
	mov	[ini_length],0
	test	eax,eax
	jz	settings_defaults
	mov	ebx,ini_path
	call	load_file
	jc	no_settings_file
	cmp	ecx,INI_BUFFER_SIZE-1
	jbe	settings_size_ok
	mov	ecx,INI_BUFFER_SIZE-1
    settings_size_ok:
	mov	[ini_length],ecx
	mov	esi,eax
	mov	edi,[ini_data]
	rep	movsb
	call	release_memory
	jmp	read_settings
    no_settings_file:
	mov	esi,_section_environment
	mov	edx,_key_environment_include
	mov	ebx,include_path
	call	ini_set
	call	ini_flush
    read_settings:
	mov	esi,_section_compiler
	mov	edx,_key_compiler_memory
	call	ini_get_int
	jc	memory_setting_ok
	test	eax,eax
	jz	memory_setting_ok
	mov	[compiler_memory],eax
    memory_setting_ok:
	mov	esi,_section_compiler
	mov	edx,_key_compiler_priority
	call	ini_get_int
	jc	priority_setting_ok
	mov	[compiler_priority],eax
    priority_setting_ok:
    settings_defaults:
	mov	[passes_limit],100
	cmp	[ini_data],0
	je	settings_read
	mov	esi,_section_compiler
	mov	edx,_key_compiler_passes
	call	ini_get_int
	jc	passes_setting_ok
	test	eax,eax
	jz	passes_setting_ok
	cmp	eax,10000h
	ja	passes_setting_ok
	mov	[passes_limit],ax
    passes_setting_ok:
	mov	ebx,option_settings
    read_option_setting:
	mov	edx,[ebx]
	test	edx,edx
	jz	options_read
	mov	esi,_section_options
	push	ebx
	call	ini_get_int
	pop	ebx
	jc	next_option_setting
	mov	ecx,[ebx+4]
	not	ecx
	and	[fedit_style],ecx
	test	eax,eax
	jz	next_option_setting
	mov	ecx,[ebx+4]
	or	[fedit_style],ecx
    next_option_setting:
	add	ebx,8
	jmp	read_option_setting
    options_read:
	mov	esi,_section_options
	mov	edx,_key_options_oneinstanceonly
	call	ini_get_int
	jc	instance_setting_ok
	and	eax,1
	mov	[instance_flags],eax
    instance_setting_ok:
	mov	ebx,color_settings
    read_color_setting:
	mov	edx,[ebx]
	test	edx,edx
	jz	colors_read
	push	ebx
	mov	esi,_section_colors
	mov	edi,string_buffer
	mov	ecx,100h
	call	ini_get
	jc	next_color_setting
	mov	esi,string_buffer
	call	parse_color
	jc	next_color_setting
	mov	ebx,[esp]
	mov	ebx,[ebx+4]
	mov	[ebx],eax
    next_color_setting:
	pop	ebx
	add	ebx,8
	jmp	read_color_setting
    colors_read:
	mov	esi,_section_font
	mov	edx,_key_font_name
	mov	edi,font_name
	mov	ecx,256
	call	ini_get
	mov	esi,_section_window
	mov	edx,_key_window_left
	call	ini_get_int
	jc	window_settings_read
	mov	[window_left],eax
	mov	esi,_section_window
	mov	edx,_key_window_top
	call	ini_get_int
	jc	window_settings_read
	mov	[window_top],eax
	mov	esi,_section_window
	mov	edx,_key_window_right
	call	ini_get_int
	jc	window_settings_read
	mov	[window_right],eax
	mov	esi,_section_window
	mov	edx,_key_window_bottom
	call	ini_get_int
	jc	window_settings_read
	mov	[window_bottom],eax
	mov	[window_position_known],1
	mov	esi,_section_window
	mov	edx,_key_window_maximized
	call	ini_get_int
	jc	window_settings_read
	and	eax,1
	mov	[window_maximized],eax
    window_settings_read:
    settings_read:
	retn

; parse_color: esi = "r,g,b", returns CF=0 and eax = 0RRGGBBh

parse_color:
	call	parse_color_component
	jc	color_invalid
	mov	edx,eax
	call	skip_comma
	jc	color_invalid
	call	parse_color_component
	jc	color_invalid
	shl	edx,8
	or	edx,eax
	call	skip_comma
	jc	color_invalid
	call	parse_color_component
	jc	color_invalid
	shl	edx,8
	or	eax,edx
	clc
	retn
    color_invalid:
	stc
	retn
    parse_color_component:
	push	edx
	call	parse_number
	pop	edx
	jc	color_invalid
	cmp	eax,0FFh
	ja	color_invalid
	clc
	retn
    skip_comma:
	lodsb
	cmp	al,20h
	je	skip_comma
	cmp	al,9
	je	skip_comma
	cmp	al,','
	jne	color_invalid
	clc
	retn

; save_settings: writes settings at the end of program

save_settings:
	cmp	[ini_data],0
	je	settings_saved
	mov	eax,[compiler_memory]
	mov	edx,_key_compiler_memory
	call	set_compiler_value
	movzx	eax,[passes_limit]
	mov	edx,_key_compiler_passes
	call	set_compiler_value
	mov	eax,[compiler_priority]
	mov	edx,_key_compiler_priority
	call	set_compiler_value
	mov	ebx,option_settings
    write_option_setting:
	mov	edx,[ebx]
	test	edx,edx
	jz	options_written
	xor	eax,eax
	mov	ecx,[ebx+4]
	test	[fedit_style],ecx
	jz	option_value_ready
	inc	eax
    option_value_ready:
	push	ebx
	mov	esi,_section_options
	call	set_number_value
	pop	ebx
	add	ebx,8
	jmp	write_option_setting
    options_written:
	mov	ebx,color_settings
    write_color_setting:
	mov	edx,[ebx]
	test	edx,edx
	jz	colors_written
	push	ebx edx
	mov	ebx,[ebx+4]
	mov	ebx,[ebx]
	mov	edi,number_text
	mov	[color_to_write],ebx
	movzx	eax,byte [color_to_write+2]
	call	format_number
	mov	byte [edi],','
	inc	edi
	movzx	eax,byte [color_to_write+1]
	call	format_number
	mov	byte [edi],','
	inc	edi
	movzx	eax,byte [color_to_write]
	call	format_number
	pop	edx
	mov	esi,_section_colors
	mov	ebx,number_text
	call	ini_set
	pop	ebx
	add	ebx,8
	jmp	write_color_setting
    colors_written:
	cmp	byte [font_name],0
	je	font_written
	mov	esi,_section_font
	mov	edx,_key_font_name
	mov	ebx,font_name
	call	ini_set
    font_written:
	call	window_is_maximized
	push	eax
	mov	esi,_section_window
	mov	edx,_key_window_maximized
	call	set_number_value
	pop	eax
	test	eax,eax
	jnz	window_written
	mov	eax,[main_window]
	xor	ecx,ecx
	xor	edx,edx
	call	x11_translate_coordinates
	mov	[window_left],ecx
	mov	[window_top],edx
	add	ecx,[main_width]
	mov	[window_right],ecx
	add	edx,[main_height]
	mov	[window_bottom],edx
	mov	esi,_section_window
	mov	edx,_key_window_left
	mov	eax,[window_left]
	call	set_number_value
	mov	esi,_section_window
	mov	edx,_key_window_top
	mov	eax,[window_top]
	call	set_number_value
	mov	esi,_section_window
	mov	edx,_key_window_right
	mov	eax,[window_right]
	call	set_number_value
	mov	esi,_section_window
	mov	edx,_key_window_bottom
	mov	eax,[window_bottom]
	call	set_number_value
    window_written:
	call	ini_flush
    settings_saved:
	retn
    set_compiler_value:
	mov	esi,_section_compiler

; set_number_value: esi = section, edx = key, eax = value

set_number_value:
	push	edi
	mov	edi,number_text
	mov	[format_signed],1
	call	format_number
	mov	[format_signed],0
	mov	ebx,number_text
	call	ini_set
	pop	edi
	retn

; save_instance_setting: writes option of single instance immediately

save_instance_setting:
	cmp	[ini_data],0
	je	settings_saved
	mov	esi,_section_options
	mov	edx,_key_options_oneinstanceonly
	mov	eax,[instance_flags]
	call	set_number_value
	jmp	ini_flush

; window_is_maximized: returns eax = 1 if main window is maximized

window_is_maximized:
	mov	eax,[main_window]
	mov	ecx,[atom_net_wm_state]
	xor	edx,edx
	call	x11_get_property
	jc	window_not_maximized
	mov	ecx,[esi+16]
	lea	esi,[esi+32]
	xor	edx,edx
	jecxz	window_state_checked
    check_window_state:
	lodsd
	cmp	eax,[atom_net_wm_state_maximized_vert]
	jne	not_vertically_maximized
	or	edx,1
    not_vertically_maximized:
	cmp	eax,[atom_net_wm_state_maximized_horz]
	jne	not_horizontally_maximized
	or	edx,2
    not_horizontally_maximized:
	loop	check_window_state
    window_state_checked:
	xor	eax,eax
	cmp	edx,3
	jne	window_maximized_known
	inc	eax
    window_maximized_known:
	retn
    window_not_maximized:
	xor	eax,eax
	retn

; ini_find: esi = section, edx = key
; returns CF=0 and esi = value, ecx = length of value

ini_find:
	push	eax ebx edx edi ebp
	mov	[ini_section],esi
	mov	[ini_key],edx
	mov	esi,[ini_data]
	test	esi,esi
	jz	ini_value_missing
	mov	ebp,esi
	add	ebp,[ini_length]
	xor	ebx,ebx
    ini_find_line:
	cmp	esi,ebp
	jae	ini_value_missing
	call	ini_skip_spaces
	cmp	esi,ebp
	jae	ini_value_missing
	cmp	byte [esi],'['
	je	ini_section_line
	test	ebx,ebx
	jz	ini_next_line
	mov	edi,[ini_key]
	call	ini_compare_name
	jc	ini_next_line
	call	ini_skip_spaces
	cmp	esi,ebp
	jae	ini_next_line
	cmp	byte [esi],'='
	jne	ini_next_line
	inc	esi
	call	ini_skip_spaces
	mov	edi,esi
    ini_find_value_end:
	cmp	edi,ebp
	jae	ini_value_end_found
	mov	al,[edi]
	cmp	al,0Ah
	je	ini_value_end_found
	cmp	al,0Dh
	je	ini_value_end_found
	inc	edi
	jmp	ini_find_value_end
    ini_value_end_found:
	cmp	edi,esi
	jbe	ini_value_trimmed
	mov	al,[edi-1]
	cmp	al,20h
	je	ini_trim_value
	cmp	al,9
	jne	ini_value_trimmed
    ini_trim_value:
	dec	edi
	jmp	ini_value_end_found
    ini_value_trimmed:
	mov	ecx,edi
	sub	ecx,esi
	pop	ebp edi edx ebx eax
	clc
	retn
    ini_section_line:
	inc	esi
	call	ini_skip_spaces
	mov	edi,[ini_section]
	call	ini_compare_name
	mov	ebx,0
	jc	ini_next_line
	call	ini_skip_spaces
	cmp	esi,ebp
	jae	ini_next_line
	cmp	byte [esi],']'
	jne	ini_next_line
	inc	ebx
    ini_next_line:
	cmp	esi,ebp
	jae	ini_value_missing
	lodsb
	cmp	al,0Ah
	jne	ini_next_line
	jmp	ini_find_line
    ini_value_missing:
	pop	ebp edi edx ebx eax
	stc
	retn

; ini_skip_spaces: esi = text, ebp = end

ini_skip_spaces:
	cmp	esi,ebp
	jae	ini_spaces_skipped
	cmp	byte [esi],20h
	je	ini_skip_space
	cmp	byte [esi],9
	jne	ini_spaces_skipped
    ini_skip_space:
	inc	esi
	jmp	ini_skip_spaces
    ini_spaces_skipped:
	retn

; ini_compare_name: esi = text, edi = name, ebp = end
; returns CF=0 and esi after name if it matches (without case)

ini_compare_name:
	push	esi
    ini_compare_character:
	mov	al,[edi]
	test	al,al
	jz	ini_name_end
	cmp	esi,ebp
	jae	ini_name_mismatch
	mov	ah,[esi]
	or	al,20h
	or	ah,20h
	cmp	al,ah
	jne	ini_name_mismatch
	inc	esi
	inc	edi
	jmp	ini_compare_character
    ini_name_end:
	cmp	esi,ebp
	jae	ini_name_matches
	mov	al,[esi]
	cmp	al,20h
	je	ini_name_matches
	cmp	al,9
	je	ini_name_matches
	cmp	al,'='
	je	ini_name_matches
	cmp	al,']'
	je	ini_name_matches
    ini_name_mismatch:
	pop	esi
	stc
	retn
    ini_name_matches:
	add	esp,4
	clc
	retn

; ini_get: esi = section, edx = key, edi = buffer, ecx = size
; returns CF=0 if value was copied

ini_get:
	push	ecx edi
	call	ini_find
	pop	edi edx
	jc	ini_get_done
	dec	edx
	cmp	ecx,edx
	jbe	ini_get_size_ok
	mov	ecx,edx
    ini_get_size_ok:
	push	edi
	rep	movsb
	mov	byte [edi],0
	pop	edi
	clc
    ini_get_done:
	retn

; ini_get_int: esi = section, edx = key, returns CF=0 and eax = value

ini_get_int:
	push	edi
	mov	edi,number_text
	mov	ecx,32
	call	ini_get
	jc	ini_int_missing
	mov	esi,number_text
	call	parse_number
    ini_int_missing:
	pop	edi
	retn

; ini_set: esi = section, edx = key, ebx = value

ini_set:
	pushad
	mov	[ini_section],esi
	mov	[ini_key],edx
	mov	[ini_value],ebx
	mov	esi,[ini_data]
	test	esi,esi
	jz	ini_set_done
	cmp	[ini_length],INI_BUFFER_SIZE-10000h
	ja	ini_set_done
	mov	ebp,esi
	add	ebp,[ini_length]
	mov	[ini_key_line],0
	mov	[ini_insert_position],0
	xor	ebx,ebx
    ini_scan_line:
	cmp	esi,ebp
	jae	ini_scan_done
	mov	[ini_line_start],esi
	call	ini_skip_spaces
	cmp	esi,ebp
	jae	ini_scan_done
	mov	al,[esi]
	cmp	al,0Ah
	je	ini_scan_next_line
	cmp	al,0Dh
	je	ini_scan_next_line
	cmp	al,'['
	je	ini_scan_section
	test	ebx,ebx
	jz	ini_scan_next_line
	mov	edi,[ini_key]
	call	ini_compare_name
	jc	ini_scan_content_line
	call	ini_skip_spaces
	cmp	esi,ebp
	jae	ini_scan_content_line
	cmp	byte [esi],'='
	jne	ini_scan_content_line
	mov	eax,[ini_line_start]
	mov	[ini_key_line],eax
	call	ini_line_end
	mov	[ini_key_line_end],esi
	jmp	ini_scan_done
    ini_scan_content_line:
	call	ini_line_end
	mov	[ini_insert_position],esi
	jmp	ini_scan_line
    ini_scan_section:
	test	ebx,ebx
	jnz	ini_scan_done
	inc	esi
	call	ini_skip_spaces
	mov	edi,[ini_section]
	call	ini_compare_name
	jc	ini_scan_next_line
	inc	ebx
	call	ini_line_end
	mov	[ini_insert_position],esi
	jmp	ini_scan_line
    ini_scan_next_line:
	call	ini_line_end
	jmp	ini_scan_line
    ini_scan_done:
	mov	edi,ini_temporary
	mov	esi,[ini_data]
	cmp	[ini_key_line],0
	jne	ini_replace_line
	test	ebx,ebx
	jnz	ini_insert_line
	mov	ecx,[ini_length]
	rep	movsb
	call	ini_ensure_line_break
	mov	byte [edi],'['
	inc	edi
	mov	esi,[ini_section]
	call	copy_text
	mov	byte [edi],']'
	mov	byte [edi+1],0Ah
	add	edi,2
	call	ini_store_key_line
	jmp	ini_set_store
    ini_replace_line:
	mov	ecx,[ini_key_line]
	sub	ecx,esi
	rep	movsb
	call	ini_store_key_line
	mov	esi,[ini_key_line_end]
	jmp	ini_copy_rest
    ini_insert_line:
	mov	ecx,[ini_insert_position]
	sub	ecx,esi
	rep	movsb
	call	ini_ensure_line_break
	call	ini_store_key_line
	mov	esi,[ini_insert_position]
    ini_copy_rest:
	mov	ecx,[ini_data]
	add	ecx,[ini_length]
	sub	ecx,esi
	jbe	ini_set_store
	rep	movsb
    ini_set_store:
	mov	ecx,edi
	sub	ecx,ini_temporary
	cmp	ecx,INI_BUFFER_SIZE-1
	ja	ini_set_done
	mov	[ini_length],ecx
	mov	esi,ini_temporary
	mov	edi,[ini_data]
	rep	movsb
    ini_set_done:
	popad
	retn

; ini_line_end: esi = position in line, returns esi = start of next line

ini_line_end:
	cmp	esi,ebp
	jae	ini_line_end_found
	lodsb
	cmp	al,0Ah
	jne	ini_line_end
    ini_line_end_found:
	retn

; ini_ensure_line_break: edi = end of written text

ini_ensure_line_break:
	cmp	edi,ini_temporary
	je	ini_line_break_ok
	cmp	byte [edi-1],0Ah
	je	ini_line_break_ok
	mov	byte [edi],0Ah
	inc	edi
    ini_line_break_ok:
	retn

; ini_store_key_line: writes "key=value" line at edi

ini_store_key_line:
	mov	esi,[ini_key]
	call	copy_text
	mov	byte [edi],'='
	inc	edi
	mov	esi,[ini_value]
	call	copy_text
	mov	byte [edi],0Ah
	inc	edi
	retn

; ini_flush: writes settings file

ini_flush:
	cmp	[ini_data],0
	je	ini_flushed
	mov	ebx,ini_path
	mov	esi,[ini_data]
	mov	ecx,[ini_length]
	call	save_file
    ini_flushed:
	retn
