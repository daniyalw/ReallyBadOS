global jmp_somewhere
jmp_somewhere:
    mov eax, [esp+4]
    jmp eax
    ret
