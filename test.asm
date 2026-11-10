format ELF64

public _main as 'main'

section '.text' executable

_main:
        xor rax, rax
        ret