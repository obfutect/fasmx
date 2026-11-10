
; fasmx - flat assembler IDE for Linux/X11
; Linux system interface (i386 system calls)

SYS_EXIT	  = 1
SYS_FORK	  = 2
SYS_READ	  = 3
SYS_WRITE	  = 4
SYS_OPEN	  = 5
SYS_CLOSE	  = 6
SYS_WAITPID	  = 7
SYS_EXECVE	  = 11
SYS_CHDIR	  = 12
SYS_TIME	  = 13
SYS_LSEEK	  = 19
SYS_ACCESS	  = 33
SYS_KILL	  = 37
SYS_MKDIR	  = 39
SYS_PIPE	  = 42
SYS_FCNTL	  = 55
SYS_SETSID	  = 66
SYS_READLINK	  = 85
SYS_MUNMAP	  = 91
SYS_SETPRIORITY   = 97
SYS_SOCKETCALL	  = 102
SYS_UNAME	  = 122
SYS_POLL	  = 168
SYS_GETCWD	  = 183
SYS_MMAP2	  = 192
SYS_STAT64	  = 195
SYS_GETDENTS64	  = 220
SYS_EXIT_GROUP	  = 252
SYS_CLOCK_GETTIME = 265

F_SETFD    = 2
FD_CLOEXEC = 1
SIGKILL    = 9
MAP_SHARED = 1
S_IFMT	   = 170000o
S_IFDIR    = 40000o
O_DIRECTORY = 200000o

O_RDONLY = 0
O_WRONLY = 1
O_CREAT  = 100o
O_EXCL   = 200o
O_TRUNC  = 1000o

SEEK_SET = 0
SEEK_END = 2

EINTR  = 4
ENOENT = 2

POLLIN = 1

PROT_READ     = 1
PROT_WRITE    = 2
MAP_PRIVATE   = 2
MAP_ANONYMOUS = 20h

AF_UNIX     = 1
SOCK_STREAM = 1
SYS_SOCKET  = 1
SYS_CONNECT = 3

CLOCK_MONOTONIC = 1

STDOUT = 1
STDERR = 2

; get_memory: ecx = size
; returns eax = address of zeroed memory (0 on failure), ebx = handle
; preserves esi, edi, ebp

get_memory:
	push	esi edi ebp
	lea	ecx,[ecx+1000h+0FFFh]
	and	ecx,not 0FFFh
	push	ecx
	mov	eax,SYS_MMAP2
	xor	ebx,ebx
	mov	edx,PROT_READ+PROT_WRITE
	mov	esi,MAP_PRIVATE+MAP_ANONYMOUS
	or	edi,-1
	xor	ebp,ebp
	int	80h
	pop	ecx
	cmp	eax,-4096
	ja	get_memory_failed
	mov	[eax],ecx
	add	eax,1000h
	mov	ebx,eax
	pop	ebp edi esi
	retn
    get_memory_failed:
	xor	eax,eax
	xor	ebx,ebx
	pop	ebp edi esi
	retn

; release_memory: ebx = handle (may be 0)
; preserves all registers except eax

release_memory:
	test	ebx,ebx
	jz	memory_released
	push	ebx ecx
	mov	ecx,[ebx-1000h]
	sub	ebx,1000h
	mov	eax,SYS_MUNMAP
	int	80h
	pop	ecx ebx
    memory_released:
	retn

; find_environment_variable: esi = name
; returns eax = pointer to value (0 if not defined)

find_environment_variable:
	push	ebx ecx edx esi edi
	mov	ebx,[environment]
	test	ebx,ebx
	jz	no_environment_variable
    next_environment_variable:
	mov	edi,[ebx]
	test	edi,edi
	jz	no_environment_variable
	add	ebx,4
	mov	edx,esi
    compare_variable_name:
	mov	al,[edx]
	mov	cl,[edi]
	test	al,al
	jz	variable_name_end
	cmp	al,cl
	jne	next_environment_variable
	inc	edx
	inc	edi
	jmp	compare_variable_name
    variable_name_end:
	cmp	cl,'='
	jne	next_environment_variable
	lea	eax,[edi+1]
	pop	edi esi edx ecx ebx
	retn
    no_environment_variable:
	xor	eax,eax
	pop	edi esi edx ecx ebx
	retn

; load_file: ebx = path
; returns CF=0 and eax = data, ecx = size, ebx = memory handle,
; or CF=1 and eax = negative error code

load_file:
	push	esi edi
	mov	eax,SYS_OPEN
	mov	ecx,O_RDONLY
	xor	edx,edx
	int	80h
	test	eax,eax
	js	load_file_failed
	mov	esi,eax
	mov	eax,SYS_LSEEK
	mov	ebx,esi
	xor	ecx,ecx
	mov	edx,SEEK_END
	int	80h
	test	eax,eax
	js	load_file_close
	mov	edi,eax
	mov	eax,SYS_LSEEK
	mov	ebx,esi
	xor	ecx,ecx
	mov	edx,SEEK_SET
	int	80h
	lea	ecx,[edi+1]
	call	get_memory
	test	eax,eax
	jz	load_file_no_memory
	mov	[load_file_buffer],eax
	mov	[load_file_size],edi
	mov	edx,edi
	mov	edi,eax
    load_file_read:
	test	edx,edx
	jz	load_file_read_done
	mov	eax,SYS_READ
	mov	ebx,esi
	mov	ecx,edi
	int	80h
	cmp	eax,-EINTR
	je	load_file_read
	test	eax,eax
	js	load_file_read_failed
	jz	load_file_read_done
	add	edi,eax
	sub	edx,eax
	jmp	load_file_read
    load_file_read_done:
	sub	edi,[load_file_buffer]
	mov	[load_file_size],edi
	mov	eax,SYS_CLOSE
	mov	ebx,esi
	int	80h
	mov	eax,[load_file_buffer]
	mov	ecx,[load_file_size]
	mov	ebx,eax
	pop	edi esi
	clc
	retn
    load_file_read_failed:
	push	eax
	mov	ebx,[load_file_buffer]
	call	release_memory
	pop	eax
	jmp	load_file_close
    load_file_no_memory:
	mov	eax,-12
    load_file_close:
	push	eax
	mov	eax,SYS_CLOSE
	mov	ebx,esi
	int	80h
	pop	eax
    load_file_failed:
	pop	edi esi
	stc
	retn

; save_file: ebx = path, esi = data, ecx = size
; returns CF=1 on failure

save_file:
	mov	eax,O_WRONLY+O_CREAT+O_TRUNC
	jmp	save_file_with_flags

; save_new_file: same interface, but never replaces an existing file.
save_new_file:
	mov	eax,O_WRONLY+O_CREAT+O_EXCL
save_file_with_flags:
	push	esi edi
	mov	edi,ecx
	mov	ecx,eax
	mov	eax,SYS_OPEN
	mov	edx,644o
	int	80h
	test	eax,eax
	js	save_file_failed
	mov	ebx,eax
    save_file_write:
	test	edi,edi
	jz	save_file_written
	mov	eax,SYS_WRITE
	mov	ecx,esi
	mov	edx,edi
	int	80h
	cmp	eax,-EINTR
	je	save_file_write
	test	eax,eax
	js	save_file_write_failed
	add	esi,eax
	sub	edi,eax
	jmp	save_file_write
    save_file_written:
	mov	eax,SYS_CLOSE
	int	80h
	test	eax,eax
	js	save_file_failed
	pop	edi esi
	clc
	retn
    save_file_write_failed:
	mov	eax,SYS_CLOSE
	int	80h
    save_file_failed:
	pop	edi esi
	stc
	retn

; get_time: returns eax = monotonic time in milliseconds

get_time:
	push	ebx ecx edx
	mov	eax,SYS_CLOCK_GETTIME
	mov	ebx,CLOCK_MONOTONIC
	mov	ecx,timespec
	int	80h
	mov	eax,[timespec]
	mov	ecx,1000
	mul	ecx
	mov	ecx,eax
	mov	eax,[timespec+4]
	xor	edx,edx
	mov	ebx,1000000
	div	ebx
	add	eax,ecx
	pop	edx ecx ebx
	retn

; write_string: ebx = file descriptor, esi = zero-terminated string

write_string:
	push	eax ecx edx edi
	mov	edi,esi
	xor	al,al
	or	ecx,-1
	repne	scasb
	not	ecx
	dec	ecx
	mov	edx,ecx
	mov	ecx,esi
	mov	eax,SYS_WRITE
	int	80h
	pop	edi edx ecx eax
	retn

; write_number: ebx = file descriptor, eax = unsigned number

write_number:
	push	eax ecx edx esi edi
	mov	edi,number_buffer+11
	mov	byte [edi],0
	mov	ecx,10
    convert_number_digit:
	xor	edx,edx
	div	ecx
	add	dl,'0'
	dec	edi
	mov	[edi],dl
	test	eax,eax
	jnz	convert_number_digit
	mov	esi,edi
	call	write_string
	pop	edi esi edx ecx eax
	retn

; exit_program: al = exit code

exit_program:
	movzx	ebx,al
	mov	eax,SYS_EXIT
	int	80h

; get_shared_memory: ecx = size
; returns eax = address of zeroed memory shared with child processes

get_shared_memory:
	push	esi edi ebp
	lea	ecx,[ecx+1000h+0FFFh]
	and	ecx,not 0FFFh
	push	ecx
	mov	eax,SYS_MMAP2
	xor	ebx,ebx
	mov	edx,PROT_READ+PROT_WRITE
	mov	esi,MAP_SHARED+MAP_ANONYMOUS
	or	edi,-1
	xor	ebp,ebp
	int	80h
	pop	ecx
	cmp	eax,-4096
	ja	get_memory_failed
	mov	[eax],ecx
	add	eax,1000h
	mov	ebx,eax
	pop	ebp edi esi
	retn

; resize_memory: ebx = handle, ecx = new size
; returns eax = new address (data copied), ebx = new handle (0 on failure,
; old memory is then kept)

resize_memory:
	push	esi edi
	push	ebx
	mov	esi,ebx
	mov	edi,ecx
	call	get_memory
	test	eax,eax
	jz	resize_failed
	push	eax ebx
	mov	ecx,[esi-1000h]
	sub	ecx,1000h
	cmp	ecx,edi
	jbe	resize_copy_size_ok
	mov	ecx,edi
    resize_copy_size_ok:
	mov	edi,eax
	shr	ecx,2
	rep	movsd
	pop	ebx eax
	xchg	ebx,[esp]
	call	release_memory
	pop	ebx
	mov	eax,ebx
	pop	edi esi
	retn
    resize_failed:
	pop	ebx
	xor	eax,eax
	pop	edi esi
	retn

; get_current_directory: edi = buffer (PATH_SIZE bytes)

get_current_directory:
	push	eax ebx ecx
	mov	eax,SYS_GETCWD
	mov	ebx,edi
	mov	ecx,PATH_SIZE
	int	80h
	test	eax,eax
	jns	current_directory_ok
	mov	word [edi],'/'
    current_directory_ok:
	pop	ecx ebx eax
	retn

; full_path_name: esi = path, ebx = base directory (0 for current one),
; edi = buffer (PATH_SIZE bytes) for absolute path without . and ..

full_path_name:
	push	eax ebx ecx edx esi edi
	mov	[full_path_output],edi
	mov	edi,full_path_buffer
	cmp	byte [esi],'/'
	je	copy_path_to_normalize
	cmp	byte [esi],'\'
	je	copy_path_to_normalize
	test	ebx,ebx
	jnz	copy_base_directory
	call	get_current_directory
	jmp	find_end_of_base
    copy_base_directory:
	push	esi
	mov	esi,ebx
	mov	ecx,PATH_SIZE-2
    copy_base_character:
	lodsb
	stosb
	test	al,al
	loopnz	copy_base_character
	mov	byte [edi-1],0
	pop	esi
	mov	edi,full_path_buffer
    find_end_of_base:
	cmp	byte [edi],0
	je	base_end_found
	inc	edi
	jmp	find_end_of_base
    base_end_found:
	mov	al,'/'
	stosb
    copy_path_to_normalize:
	lea	ecx,[full_path_buffer+PATH_SIZE*2-1]
	sub	ecx,edi
    copy_path_character:
	lodsb
	cmp	al,'\'
	jne	path_character_ok
	mov	al,'/'
    path_character_ok:
	stosb
	test	al,al
	loopnz	copy_path_character
	mov	byte [edi-1],0
	mov	esi,full_path_buffer
	mov	edi,[full_path_output]
	lea	edx,[edi+PATH_SIZE-1]
	mov	ebx,edi
    next_path_component:
	cmp	byte [esi],'/'
	jne	path_component_start
	inc	esi
	jmp	next_path_component
    path_component_start:
	cmp	byte [esi],0
	je	path_normalized
	cmp	byte [esi],'.'
	jne	copy_path_component
	mov	al,[esi+1]
	test	al,al
	jz	skip_dot_component
	cmp	al,'/'
	je	skip_dot_component
	cmp	al,'.'
	jne	copy_path_component
	mov	al,[esi+2]
	test	al,al
	jz	parent_component
	cmp	al,'/'
	jne	copy_path_component
    parent_component:
	add	esi,2
    remove_last_component:
	cmp	edi,ebx
	je	next_path_component
	dec	edi
	cmp	byte [edi],'/'
	jne	remove_last_component
	jmp	next_path_component
    skip_dot_component:
	inc	esi
	jmp	next_path_component
    copy_path_component:
	cmp	edi,edx
	jae	path_normalized
	mov	byte [edi],'/'
	inc	edi
    copy_component_character:
	lodsb
	test	al,al
	jz	last_component_copied
	cmp	al,'/'
	je	next_path_component
	cmp	edi,edx
	jae	path_normalized
	stosb
	jmp	copy_component_character
    last_component_copied:
	dec	esi
	jmp	next_path_component
    path_normalized:
	cmp	edi,ebx
	jne	terminate_full_path
	mov	byte [edi],'/'
	inc	edi
    terminate_full_path:
	mov	byte [edi],0
	pop	edi esi edx ecx ebx eax
	retn

; file_title: esi = path, returns esi = name of file in path

file_title:
	push	eax edx
	mov	edx,esi
    find_file_title:
	lodsb
	test	al,al
	jz	file_title_found
	cmp	al,'/'
	jne	find_file_title
	mov	edx,esi
	jmp	find_file_title
    file_title_found:
	mov	esi,edx
	pop	edx eax
	retn

; directory_of_path: esi = path, edi = buffer, copies directory part

directory_of_path:
	push	eax ecx esi edi
	push	edi
	call	file_title
	mov	ecx,esi
	pop	edi
	mov	esi,[esp+4]
	sub	ecx,esi
	cmp	ecx,1
	jbe	root_directory
	dec	ecx
	rep	movsb
	mov	byte [edi],0
	pop	edi esi ecx eax
	retn
    root_directory:
	mov	word [edi],'/'
	pop	edi esi ecx eax
	retn

; get_file_type: ebx = path
; returns eax = 0 if no such file, 1 for regular file, 2 for directory

get_file_type:
	push	ebx ecx
	mov	eax,SYS_STAT64
	mov	ecx,stat_buffer
	int	80h
	test	eax,eax
	js	no_such_file
	mov	eax,dword [stat_buffer+16]
	and	eax,S_IFMT
	cmp	eax,S_IFDIR
	je	directory_type
	mov	eax,1
	pop	ecx ebx
	retn
    directory_type:
	mov	eax,2
	pop	ecx ebx
	retn
    no_such_file:
	xor	eax,eax
	pop	ecx ebx
	retn

; compare_strings_nocase: esi, edi = zero-terminated strings
; returns ZF=1 when equal (ASCII letters compared without case)

compare_strings_nocase:
	push	eax esi edi
    compare_nocase_character:
	mov	al,[esi]
	mov	ah,[edi]
	inc	esi
	inc	edi
	cmp	al,'A'
	jb	first_case_ok
	cmp	al,'Z'
	ja	first_case_ok
	add	al,20h
    first_case_ok:
	cmp	ah,'A'
	jb	second_case_ok
	cmp	ah,'Z'
	ja	second_case_ok
	add	ah,20h
    second_case_ok:
	cmp	al,ah
	jne	strings_compared
	test	al,al
	jnz	compare_nocase_character
    strings_compared:
	pop	edi esi eax
	retn

; compare_strings: esi, edi = zero-terminated strings, returns ZF=1 if equal

compare_strings:
	push	eax esi edi
    compare_string_character:
	mov	al,[esi]
	cmp	al,[edi]
	jne	strings_compared
	inc	esi
	inc	edi
	test	al,al
	jnz	compare_string_character
	pop	edi esi eax
	retn

; string_length: esi = zero-terminated string, returns ecx = length

string_length:
	push	eax edi
	mov	edi,esi
	xor	al,al
	or	ecx,-1
	repne	scasb
	not	ecx
	dec	ecx
	pop	edi eax
	retn

; copy_text: esi = source, edi = destination, returns edi at terminator

copy_text:
	push	eax esi
    copy_string_character:
	lodsb
	stosb
	test	al,al
	jnz	copy_string_character
	dec	edi
	pop	esi eax
	retn

; format_number: eax = value, edi = buffer, writes decimal digits
; returns edi at terminating zero; signed if format_signed is set

format_number:
	push	eax ecx edx esi
	mov	esi,edi
	test	eax,eax
	jns	format_unsigned
	cmp	[format_signed],0
	je	format_unsigned
	mov	byte [edi],'-'
	inc	edi
	neg	eax
	mov	esi,edi
    format_unsigned:
	mov	ecx,10
    format_digit:
	xor	edx,edx
	div	ecx
	add	dl,'0'
	mov	[edi],dl
	inc	edi
	test	eax,eax
	jnz	format_digit
	mov	byte [edi],0
	push	edi
	dec	edi
    reverse_digits:
	cmp	esi,edi
	jae	digits_reversed
	mov	al,[esi]
	xchg	al,[edi]
	mov	[esi],al
	inc	esi
	dec	edi
	jmp	reverse_digits
    digits_reversed:
	pop	edi
	pop	esi edx ecx eax
	retn

; parse_number: esi = string, returns CF=0 and eax = value (optional sign),
; esi after number

parse_number:
	push	ebx ecx edx
    skip_number_spaces:
	cmp	byte [esi],20h
	je	number_space
	cmp	byte [esi],9
	jne	number_spaces_skipped
    number_space:
	inc	esi
	jmp	skip_number_spaces
    number_spaces_skipped:
	xor	ebx,ebx
	cmp	byte [esi],'-'
	jne	number_sign_plus
	inc	ebx
	inc	esi
	jmp	number_sign_ok
    number_sign_plus:
	cmp	byte [esi],'+'
	jne	number_sign_ok
	inc	esi
    number_sign_ok:
	xor	eax,eax
	xor	ecx,ecx
    parse_digit:
	movzx	edx,byte [esi]
	sub	edx,'0'
	cmp	edx,9
	ja	number_parsed
	imul	eax,10
	jo	number_overflow
	add	eax,edx
	jc	number_overflow
	inc	esi
	inc	ecx
	jmp	parse_digit
    number_parsed:
	jecxz	number_overflow
	test	ebx,ebx
	jz	parse_number_ok
	neg	eax
    parse_number_ok:
	pop	edx ecx ebx
	clc
	retn
    number_overflow:
	pop	edx ecx ebx
	stc
	retn
