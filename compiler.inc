
; fasmx - flat assembler IDE for Linux/X11
; assembler interface (port of FASM.INC) and compilation in a child process

; results of compilation (in memory shared with the compiling process)

CR_EXIT 	  = 0
CR_PASSES	  = 4
CR_WRITTEN	  = 8
CR_TIME 	  = 12
CR_FORMAT	  = 16
CR_FLAGS	  = 20
CR_MESSAGE	  = 24
CR_DISPLAY	  = 28
CR_OUTPUT	  = 32
CR_INSTRUCTION	  = 36
CR_LINES_COUNT	  = 40
CR_DISPLAY_LENGTH = 44
CR_STRINGS_USED   = 48
CR_LINES	  = 64
CR_MAX_LINES	  = 1024
CR_DISPLAY_AREA   = 4000h
CR_DISPLAY_SIZE   = 100000h
CR_STRINGS_AREA   = CR_DISPLAY_AREA + CR_DISPLAY_SIZE + 10h
CR_STRINGS_SIZE   = 40000h
COMPILE_RESULTS_SIZE = CR_STRINGS_AREA + CR_STRINGS_SIZE

PRIO_PROCESS = 0

; interface functions called by the assembler

open:
	push	esi edi ebp
	call	adapt_core_path
	mov	eax,SYS_OPEN
	mov	ebx,core_path
	xor	ecx,ecx
	xor	edx,edx
	int	80h
	pop	ebp edi esi
	test	eax,eax
	js	core_file_error
	mov	ebx,eax
	clc
	retn
    core_file_error:
	stc
	retn

create:
	push	esi edi ebp edx
	call	adapt_core_path
	mov	ecx,O_CREAT+O_TRUNC+O_WRONLY
	mov	edx,644o
	pop	eax
	cmp	eax,[output_file]
	jne	core_create
	cmp	[output_format],5
	jne	core_create
	bt	[format_flags],0
	jnc	core_create
	mov	edx,755o
    core_create:
	mov	eax,SYS_OPEN
	mov	ebx,core_path
	int	80h
	pop	ebp edi esi
	test	eax,eax
	js	core_file_error
	mov	ebx,eax
	clc
	retn

close:
	mov	eax,SYS_CLOSE
	int	80h
	retn

read:
	push	ecx edx esi edi ebp
	mov	eax,SYS_READ
	xchg	ecx,edx
	int	80h
	pop	ebp edi esi edx ecx
	test	eax,eax
	js	core_file_error
	cmp	eax,ecx
	jne	core_file_error
	clc
	retn

write:
	push	ecx edx esi edi ebp
	mov	esi,edx
	mov	edi,ecx
    core_write_next:
	test	edi,edi
	jz	core_write_done
	mov	eax,SYS_WRITE
	mov	ecx,esi
	mov	edx,edi
	int	80h
	cmp	eax,-EINTR
	je	core_write_next
	test	eax,eax
	jle	core_write_failed
	add	esi,eax
	sub	edi,eax
	jmp	core_write_next
    core_write_done:
	pop	ebp edi esi edx ecx
	mov	eax,ecx
	clc
	retn
    core_write_failed:
	pop	ebp edi esi edx ecx
	stc
	retn

lseek:
	mov	ecx,edx
	xor	edx,edx
	mov	dl,al
	mov	eax,SYS_LSEEK
	int	80h
	cmp	eax,-4096
	ja	core_file_error
	clc
	retn

; adapt_core_path: edx = path, copies it to core_path with slashes

adapt_core_path:
	mov	esi,edx
	mov	edi,core_path
	mov	ecx,PATH_SIZE-1
    copy_core_path:
	lodsb
	cmp	al,'\'
	jne	core_path_character_ok
	mov	al,'/'
    core_path_character_ok:
	stosb
	test	al,al
	loopnz	copy_core_path
	mov	byte [edi],0
	retn

display_block:
	push	ebx edi
	mov	edi,[compile_results]
	mov	eax,[edi+CR_DISPLAY_LENGTH]
	mov	edx,CR_DISPLAY_SIZE-1
	sub	edx,eax
	cmp	ecx,edx
	jbe	display_space_ok
	mov	ecx,edx
    display_space_ok:
	add	[edi+CR_DISPLAY_LENGTH],ecx
	lea	edi,[edi+CR_DISPLAY_AREA+eax]
	rep	movsb
	mov	byte [edi],0
	pop	edi ebx
	retn

fatal_error:
	cmp	[compiling],0
	je	error_outside_compiler
	pop	eax
	mov	edi,[compile_results]
	mov	[edi+CR_MESSAGE],eax
    error_with_no_source:
	mov	al,0FFh
	jmp	compiler_exit

assembler_error:
	cmp	[compiling],0
	je	error_outside_compiler
	call	show_display_buffer
	pop	eax
	mov	edi,[compile_results]
	mov	[edi+CR_MESSAGE],eax
	mov	ebx,[current_line]
	test	ebx,ebx
	jz	error_with_no_source
	xor	ecx,ecx
    get_error_lines:
	mov	eax,[ebx]
	cmp	byte [eax],0
	je	get_next_error_line
	test	byte [ebx+7],80h
	jz	error_lines_ok
	inc	ecx
	mov	edx,ebx
    find_definition_origin:
	mov	edx,[edx+12]
	test	byte [edx+7],80h
	jnz	find_definition_origin
	mov	eax,[edx+4]
	and	eax,7FFFFFFFh
	push	eax
	mov	edx,[edx]
	push	edx
    get_next_error_line:
	mov	ebx,[ebx+8]
	jmp	get_error_lines
    error_lines_ok:
	inc	ecx
	mov	eax,[ebx+4]
	and	eax,7FFFFFFFh
	push	eax
	mov	edx,[ebx]
	push	edx
	mov	edi,[compile_results]
	xor	ebx,ebx
	mov	[edi+CR_LINES_COUNT],ebx
    store_error_lines:
	pop	esi
	pop	edx
	cmp	ebx,CR_MAX_LINES
	jae	error_line_skipped
	push	ecx edx
	mov	edi,[compile_results]
	mov	eax,[edi+CR_STRINGS_USED]
	cmp	eax,CR_STRINGS_SIZE-PATH_SIZE
	ja	error_line_stored
	lea	edx,[eax+CR_STRINGS_AREA]
	mov	[edi+CR_LINES+ebx*8],edx
	mov	edx,[esp]
	mov	[edi+CR_LINES+ebx*8+4],edx
	lea	edi,[edi+CR_STRINGS_AREA+eax]
	push	ebx
	xor	ebx,ebx
	call	full_path_name
	mov	esi,edi
	call	string_length
	pop	ebx
	inc	ecx
	mov	edi,[compile_results]
	add	[edi+CR_STRINGS_USED],ecx
	inc	dword [edi+CR_LINES_COUNT]
    error_line_stored:
	pop	edx ecx
    error_line_skipped:
	inc	ebx
	cmp	ebx,ecx
	jb	store_error_lines
	mov	ebx,[compile_results]
	mov	eax,[ebx+CR_STRINGS_USED]
	lea	edi,[ebx+CR_STRINGS_AREA+eax]
	add	eax,CR_STRINGS_AREA
	mov	[ebx+CR_INSTRUCTION],eax
	cmp	[preprocessing_done],0
	jne	error_in_preprocessed
	xor	al,al
	stosb
	jmp	instruction_converted
    error_in_preprocessed:
	mov	esi,[current_line]
	add	esi,16
	xor	dl,dl
    convert_instruction:
	lodsb
	cmp	al,1Ah
	je	copy_symbol
	cmp	al,22h
	je	copy_symbol
	cmp	al,3Bh
	je	ignore_preprocessor_symbols
	stosb
	or	al,al
	jz	instruction_converted
	xor	dl,dl
	jmp	convert_instruction
    copy_symbol:
	or	dl,dl
	jz	space_ok
	mov	byte [edi],20h
	inc	edi
      space_ok:
	cmp	al,22h
	je	quoted
	lodsb
	movzx	ecx,al
	rep	movsb
	or	dl,-1
	jmp	convert_instruction
      quoted:
	mov	al,27h
	stosb
	lodsd
	mov	ecx,eax
	jecxz	quoted_copied
      copy_quoted:
	lodsb
	stosb
	cmp	al,27h
	jne	quote_ok
	stosb
      quote_ok:
	loop	copy_quoted
      quoted_copied:
	mov	al,27h
	stosb
	or	dl,-1
	jmp	convert_instruction
    ignore_preprocessor_symbols:
	xor	al,al
	stosb
    instruction_converted:
	mov	al,2
	jmp	compiler_exit
    error_outside_compiler:
	mov	esp,[resume_esp]
	jmp	[resume_eip]

get_environment_variable:
	push	ebx esi
	mov	ecx,[memory_end]
	sub	ecx,edi
	cmp	ecx,1000h
	jbe	environment_limit_ok
	mov	ecx,1000h
    environment_limit_ok:
	mov	[environment_limit],ecx
	push	esi
	mov	edx,esi
	mov	esi,_section_environment
	call	ini_find
	pop	edx
	jnc	environment_value_found
	mov	esi,edx
	call	find_environment_variable
	test	eax,eax
	jz	environment_value_done
	mov	esi,eax
	call	string_length
    environment_value_found:
	cmp	ecx,[environment_limit]
	jbe	environment_value_size_ok
	mov	ecx,[environment_limit]
    environment_value_size_ok:
	rep	movsb
	cmp	edi,[memory_end]
	jae	out_of_memory
    environment_value_done:
	pop	esi ebx
	retn

make_timestamp:
	mov	eax,SYS_TIME
	xor	ebx,ebx
	int	80h
	xor	edx,edx
	retn

; compiler_exit: al = exit code (in compiling process)

compiler_exit:
	movzx	ebx,al
	mov	edi,[compile_results]
	mov	[edi+CR_EXIT],ebx
	movzx	eax,[current_pass]
	inc	eax
	mov	[edi+CR_PASSES],eax
	mov	eax,[written_size]
	mov	[edi+CR_WRITTEN],eax
	mov	eax,SYS_EXIT_GROUP
	int	80h

; compiling process

compile_child:
	mov	eax,SYS_CLOSE
	mov	ebx,[pipe_descriptors]
	int	80h
	mov	[compiling],1
	mov	esi,[compile_source]
	mov	edi,compile_directory
	call	directory_of_path
	mov	eax,SYS_CHDIR
	mov	ebx,compile_directory
	int	80h
	call	priority_index
	mov	edx,[nice_values+eax*4]
	mov	eax,SYS_SETPRIORITY
	mov	ebx,PRIO_PROCESS
	xor	ecx,ecx
	int	80h
	mov	[initial_definitions],0
	mov	eax,[compile_source]
	mov	[input_file],eax
	mov	[output_file],0
	mov	[symbols_file],0
	test	[command_flags],2
	jz	symbols_file_name_ok
	mov	esi,eax
	mov	edi,symbols_path
	call	copy_text
	mov	ebx,edi
	mov	esi,symbols_path
	call	file_title
    find_symbols_extension:
	cmp	edi,esi
	jbe	attach_fas_extension
	dec	edi
	cmp	byte [edi],'.'
	jne	find_symbols_extension
	jmp	fas_extension_place_ok
    attach_fas_extension:
	mov	edi,ebx
    fas_extension_place_ok:
	mov	esi,_fas_extension
	call	copy_text
	mov	[symbols_file],symbols_path
    symbols_file_name_ok:
	mov	eax,[compiler_memory]
	shl	eax,10
	jz	out_of_memory
    allocate_compiler_memory:
	mov	edx,eax
	shr	edx,2
	mov	ecx,eax
	sub	ecx,edx
	mov	[memory_end],ecx
	mov	[additional_memory_end],edx
	mov	ecx,eax
	push	eax
	call	get_memory
	pop	ecx
	test	eax,eax
	jnz	compiler_memory_allocated
	mov	eax,ecx
	shr	eax,1
	cmp	eax,8000h
	jb	out_of_memory
	jmp	allocate_compiler_memory
    compiler_memory_allocated:
	mov	[memory_start],eax
	mov	[code_start],eax
	add	eax,[memory_end]
	mov	[memory_end],eax
	mov	[additional_memory],eax
	add	[additional_memory_end],eax
	mov	[tagged_blocks],0
	mov	eax,esp
	and	eax,not 0FFFh
	add	eax,1000h-10000h
	mov	[stack_limit],eax
	call	get_time
	mov	[compile_start_time],eax
	mov	[preprocessing_done],0
	call	preprocessor
	mov	al,1
	call	report_progress
	or	[preprocessing_done],-1
	call	parser
	mov	al,2
	call	report_progress
	call	assembler
	mov	al,3
	call	report_progress
	call	formatter
	mov	al,4
	call	report_progress
	call	show_display_buffer
	call	get_time
	sub	eax,[compile_start_time]
	mov	edi,[compile_results]
	mov	[edi+CR_TIME],eax
	movzx	eax,[output_format]
	mov	[edi+CR_FORMAT],eax
	mov	eax,[format_flags]
	mov	[edi+CR_FLAGS],eax
	mov	esi,[output_file]
	test	esi,esi
	jz	output_path_stored
	mov	eax,[edi+CR_STRINGS_USED]
	lea	edx,[eax+CR_STRINGS_AREA]
	mov	[edi+CR_OUTPUT],edx
	lea	edi,[edi+CR_STRINGS_AREA+eax]
	mov	ebx,compile_directory
	call	full_path_name
	mov	esi,edi
	call	string_length
	inc	ecx
	mov	edi,[compile_results]
	add	[edi+CR_STRINGS_USED],ecx
    output_path_stored:
	xor	al,al
	jmp	compiler_exit

_fas_extension db '.fas',0

; report_progress: al = stage (in compiling process)

report_progress:
	mov	[progress_byte],al
	mov	eax,SYS_WRITE
	mov	ebx,[pipe_descriptors+4]
	mov	ecx,progress_byte
	mov	edx,1
	int	80h
	retn

; start_compile: starts compilation of [compile_source], returns CF=1 on failure

start_compile:
	cmp	[compile_results],0
	jne	compile_results_ready
	mov	ecx,COMPILE_RESULTS_SIZE
	call	get_shared_memory
	test	eax,eax
	jz	compile_start_failed
	mov	[compile_results],eax
    compile_results_ready:
	mov	edi,[compile_results]
	xor	eax,eax
	mov	ecx,CR_LINES/4
	rep	stosd
	mov	edi,[compile_results]
	mov	dword [edi+CR_DISPLAY],CR_DISPLAY_AREA
	mov	byte [edi+CR_DISPLAY_AREA],0
	mov	dword [edi+CR_OUTPUT],CR_DISPLAY_AREA
	mov	dword [edi+CR_INSTRUCTION],CR_DISPLAY_AREA+CR_DISPLAY_SIZE
	mov	byte [edi+CR_DISPLAY_AREA+CR_DISPLAY_SIZE],0
	mov	dword [edi+CR_EXIT],0FFh
	mov	eax,SYS_PIPE
	mov	ebx,pipe_descriptors
	int	80h
	test	eax,eax
	js	compile_start_failed
	mov	eax,SYS_FORK
	int	80h
	test	eax,eax
	jz	compile_child
	js	compile_fork_failed
	mov	[compile_pid],eax
	mov	eax,SYS_CLOSE
	mov	ebx,[pipe_descriptors+4]
	int	80h
	mov	eax,SYS_FCNTL
	mov	ebx,[pipe_descriptors]
	mov	ecx,F_SETFD
	mov	edx,FD_CLOEXEC
	int	80h
	mov	eax,[pipe_descriptors]
	mov	[compile_pipe],eax
	clc
	retn
    compile_fork_failed:
	mov	eax,SYS_CLOSE
	mov	ebx,[pipe_descriptors]
	int	80h
	mov	eax,SYS_CLOSE
	mov	ebx,[pipe_descriptors+4]
	int	80h
    compile_start_failed:
	stc
	retn

; cancel_compile: kills the compiling process

cancel_compile:
	cmp	[compile_pipe],-1
	je	compile_cancelled
	mov	eax,SYS_KILL
	mov	ebx,[compile_pid]
	mov	ecx,SIGKILL
	int	80h
	call	wait_for_compiler
    compile_cancelled:
	retn

; wait_for_compiler: closes pipe and waits for end of compiling process
; returns eax = exit code

wait_for_compiler:
	mov	eax,SYS_CLOSE
	mov	ebx,[compile_pipe]
	int	80h
	mov	[compile_pipe],-1
    wait_for_compiler_exit:
	mov	eax,SYS_WAITPID
	mov	ebx,[compile_pid]
	mov	ecx,compile_status
	xor	edx,edx
	int	80h
	cmp	eax,-EINTR
	je	wait_for_compiler_exit
	mov	eax,[compile_status]
	test	eax,7Fh
	jnz	compiler_crashed
	shr	eax,8
	and	eax,0FFh
	retn
    compiler_crashed:
	mov	edi,[compile_results]
	mov	dword [edi+CR_MESSAGE],_compiler_crashed
	mov	eax,0FFh
	retn

_compiler_crashed db 'compiler process terminated abnormally',0

; compile_pipe_input: data from compiling process is available

compile_pipe_input:
	mov	eax,SYS_READ
	mov	ebx,[compile_pipe]
	mov	ecx,pipe_input
	mov	edx,16
	int	80h
	cmp	eax,-EINTR
	je	pipe_input_done
	test	eax,eax
	jle	compiler_finished
	movzx	eax,byte [pipe_input+eax-1]
	push	eax
	mov	eax,ID_PROGRESS
	call	dialog_item
	pop	eax
	test	ebx,ebx
	jz	pipe_input_done
	mov	edx,4
	jmp	set_progress
    compiler_finished:
	call	wait_for_compiler
	jmp	end_dialog
    pipe_input_done:
	retn

; compile dialog

compile_procedure:
	cmp	eax,DM_INIT
	je	compile_init
	cmp	eax,DM_COMMAND
	je	compile_command_message
	cmp	eax,DM_CLOSE
	je	compile_cancel
	retn
    compile_init:
	mov	eax,ID_PROGRESS
	call	dialog_item
	test	ebx,ebx
	jz	compile_progress_ok
	xor	eax,eax
	mov	edx,4
	mov	[ebx+P_POS],eax
	mov	[ebx+P_MAX],edx
    compile_progress_ok:
	call	start_compile
	jnc	compile_started
	or	eax,-1
	jmp	end_dialog
    compile_started:
	retn
    compile_command_message:
	cmp	edx,IDCANCEL
	jne	compile_started
    compile_cancel:
	call	cancel_compile
	or	eax,-1
	jmp	end_dialog

; fm_compile: compiles assigned file (or current one)
; returns eax = 0 if compiled without errors

fm_compile:
	mov	eax,[assigned_file]
	push	eax
	cmp	eax,-1
	jne	compile_assigned_ok
	mov	eax,[current_tab]
	mov	[assigned_file],eax
    compile_assigned_ok:
	mov	edx,-1
	call	fm_save
	test	eax,eax
	jz	compile_save_all
	mov	eax,[assigned_file]
	call	fm_select
	mov	eax,[assigned_file]
	call	save_tab_as
	test	eax,eax
	jz	compile_save_all
	or	eax,-1
	jmp	compile_done
    compile_save_all:
	xor	ebx,ebx
    compile_save_next:
	cmp	ebx,[tabs_count]
	jae	do_compile
	push	ebx
	mov	eax,ebx
	call	tab_record
	cmp	dword [edi+TAB_PATH],0
	je	compile_skip_unnamed
	mov	eax,ebx
	mov	edx,-1
	call	fm_save
	test	eax,eax
	jnz	compile_save_failed
    compile_skip_unnamed:
	pop	ebx
	inc	ebx
	jmp	compile_save_next
    compile_save_failed:
	pop	eax
	push	eax
	call	fm_select
	pop	eax
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	call	show_saving_error
	or	eax,-1
	jmp	compile_done
    do_compile:
	mov	eax,[assigned_file]
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	test	esi,esi
	jz	compile_not_started
	mov	[compile_source],esi
	mov	esi,compile_dialog
	mov	edi,compile_procedure
	xor	eax,eax
	call	dialog_box
	cmp	eax,-1
	je	compile_done
	push	eax
	cmp	eax,2
	je	compile_error_details
	test	eax,eax
	jnz	compile_summary
	cmp	[compile_show_summary],0
	je	compile_summary_done
    compile_summary:
	mov	esi,summary_dialog
	mov	edi,summary_procedure
	call	dialog_box
	jmp	compile_summary_done
    compile_error_details:
	mov	esi,error_summary_dialog
	mov	edi,summary_procedure
	mov	eax,2
	call	dialog_box
    compile_summary_done:
	pop	eax
    compile_done:
	pop	edx
	cmp	edx,-1
	jne	compile_finished
	or	[assigned_file],-1
    compile_finished:
	retn
    compile_not_started:
	or	eax,-1
	jmp	compile_done

compile_command:
	mov	[command_flags],0
	mov	[compile_show_summary],1
	jmp	fm_compile

build_symbols:
	mov	[command_flags],2
	mov	[compile_show_summary],1
	jmp	fm_compile

; run_command: compiles and runs the program in a terminal window

run_command:
	mov	[command_flags],0
	mov	[compile_show_summary],0
	call	fm_compile
	test	eax,eax
	jnz	run_done
	mov	esi,[compile_results]
	cmp	dword [esi+CR_FORMAT],5
	jne	run_not_executable
	test	dword [esi+CR_FLAGS],1
	jz	run_not_executable
	cmp	dword [esi+CR_OUTPUT],CR_STRINGS_AREA
	jb	run_not_executable
	add	esi,[esi+CR_OUTPUT]
	mov	edi,run_path
	call	copy_text
	mov	esi,run_path
	mov	edi,run_directory
	call	directory_of_path
	mov	eax,SYS_FORK
	int	80h
	test	eax,eax
	jnz	run_parent
	call	detach_process
	mov	eax,SYS_CHDIR
	mov	ebx,run_directory
	int	80h
	mov	eax,SYS_EXECVE
	mov	ebx,_shell
	mov	ecx,run_arguments
	mov	edx,[environment]
	int	80h
	mov	eax,SYS_EXIT_GROUP
	mov	ebx,127
	int	80h
    run_parent:
	js	run_done
	mov	ebx,eax
	mov	eax,SYS_WAITPID
	xor	ecx,ecx
	xor	edx,edx
	int	80h
    run_done:
	retn
    run_not_executable:
	mov	esi,_run_object_error
	mov	edi,_caption
	mov	eax,MB_ICONERROR+MB_OK
	jmp	message_box

; show_line: eax = path, edx = line number (port of ShowLine)

show_line:
	mov	[show_line_path],eax
	mov	[show_line_number],edx
	mov	eax,[current_tab]
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	test	esi,esi
	jz	find_file_tab
	mov	edi,[show_line_path]
	call	compare_strings
	je	show_line_file_ok
    find_file_tab:
	xor	ebx,ebx
    find_file_window:
	cmp	ebx,[tabs_count]
	jae	load_for_show
	mov	eax,ebx
	call	tab_record
	mov	esi,[edi+TAB_PATH]
	test	esi,esi
	jz	next_file_window
	mov	edi,[show_line_path]
	call	compare_strings
	je	show_file
    next_file_window:
	inc	ebx
	jmp	find_file_window
    load_for_show:
	mov	esi,[show_line_path]
	mov	edi,show_path
	call	copy_text
	mov	esi,show_path
	xor	ecx,ecx
	call	fm_open
	cmp	eax,-1
	je	show_failed
	jmp	show_line_file_ok
    show_file:
	mov	eax,ebx
	call	fm_select
    show_line_file_ok:
	mov	eax,[show_line_number]
	mov	[fepos+FEPOS_SELECTION_LINE],eax
	mov	[fepos+FEPOS_CARET_LINE],eax
    get_lines_to_show:
	mov	eax,[show_line_number]
	mov	[fe_wparam],eax
	mov	edx,FM_GETLINELENGTH
	call	send_current_editor
	cmp	eax,-1
	je	show_lines
	mov	esi,show_line_cells
	cmp	eax,SHOW_LINE_CELLS-1
	jb	get_line_data
	lea	ecx,[eax*4+4]
	call	get_memory
	test	eax,eax
	jz	show_lines
	mov	esi,eax
    get_line_data:
	mov	[show_line_buffer],esi
	mov	eax,[show_line_number]
	mov	[fe_wparam],eax
	mov	[fe_lparam],esi
	mov	edx,FM_GETLINE
	call	send_current_editor
	mov	esi,[show_line_buffer]
	mov	ecx,eax
	or	ecx,ecx
	jz	no_more_lines
	mov	eax,[show_line_number]
	mov	[fepos+FEPOS_CARET_LINE],eax
	xor	edx,edx
    check_for_more_lines:
	lodsd
	cmp	eax,3Bh
	je	no_more_lines
	cmp	eax,100h
	jae	line_character_neutral
	cmp	byte [characters+eax],0
	je	line_symbol
    line_character_neutral:
	or	edx,edx
	jnz	line_neutral
	cmp	eax,27h
	je	line_quoted
	cmp	eax,22h
	je	line_quoted
    line_neutral:
	or	edx,-1
	loop	check_for_more_lines
	jmp	no_more_lines
    line_symbol:
	cmp	eax,'\'
	je	line_backslash
	xor	edx,edx
	loop	check_for_more_lines
	jmp	no_more_lines
    line_quoted:
	mov	ebx,eax
    line_quoted_character:
	dec	ecx
	jz	no_more_lines
	lodsd
	cmp	eax,ebx
	jne	line_quoted_character
	dec	ecx
	jz	no_more_lines
	lodsd
	cmp	eax,ebx
	je	line_quoted_character
	sub	esi,4
	xor	edx,edx
	jmp	check_for_more_lines
    line_backslash:
	dec	ecx
	jz	more_lines
	lodsd
	cmp	eax,20h
	je	line_backslash
	cmp	eax,3Bh
	jne	no_more_lines
    more_lines:
	inc	[show_line_number]
	call	release_show_line_buffer
	jmp	get_lines_to_show
    no_more_lines:
	call	release_show_line_buffer
    show_lines:
	mov	[fepos+FEPOS_SELECTION_POSITION],1
	inc	[fepos+FEPOS_CARET_LINE]
	mov	[fepos+FEPOS_CARET_POSITION],1
	mov	eax,[fepos+FEPOS_CARET_LINE]
	mov	[fe_wparam],eax
	mov	edx,FM_GETLINELENGTH
	call	send_current_editor
	cmp	eax,-1
	jne	show_ok
	dec	[fepos+FEPOS_CARET_LINE]
	mov	eax,[fepos+FEPOS_CARET_LINE]
	mov	[fe_wparam],eax
	mov	edx,FM_GETLINELENGTH
	call	send_current_editor
	inc	eax
	mov	[fepos+FEPOS_CARET_POSITION],eax
    show_ok:
	mov	[fe_wparam],fepos
	mov	edx,FM_SETPOS
	call	send_current_editor
	mov	edx,FM_GETMODE
	call	send_current_editor
	and	eax,not FEMODE_VERTICALSEL
	mov	[fe_wparam],eax
	mov	edx,FM_SETMODE
	call	send_current_editor
	mov	eax,[fepos+FEPOS_SELECTION_LINE]
	xchg	eax,[fepos+FEPOS_CARET_LINE]
	mov	[fepos+FEPOS_SELECTION_LINE],eax
	mov	eax,[fepos+FEPOS_SELECTION_POSITION]
	xchg	eax,[fepos+FEPOS_CARET_POSITION]
	mov	[fepos+FEPOS_SELECTION_POSITION],eax
	mov	[fe_wparam],fepos
	mov	edx,FM_SETPOS
	call	send_current_editor
	xor	eax,eax
	retn
    show_failed:
	or	eax,-1
	retn

SHOW_LINE_CELLS = 1000h

release_show_line_buffer:
	mov	ebx,[show_line_buffer]
	cmp	ebx,show_line_cells
	je	show_line_buffer_released
	call	release_memory
    show_line_buffer_released:
	retn

; assembler core

include 'source/errors.inc'
include 'source/symbdump.inc'
include 'source/preproce.inc'
include 'source/parser.inc'
include 'source/exprpars.inc'
include 'source/assemble.inc'
include 'source/exprcalc.inc'
include 'source/formats.inc'
include 'source/x86_64.inc'
include 'source/avx.inc'
include 'source/tables.inc'
include 'source/messages.inc'
