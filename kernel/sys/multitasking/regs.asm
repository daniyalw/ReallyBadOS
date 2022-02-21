global kernel_regs_switch
kernel_regs_switch:
    cli;
    mov ecx, [esp+4]   ; EIP
    mov ebp, [esp+8]  ; EBP
    mov esp, [esp+12]  ; ESP
    mov eax, 0x12345   ; magic number to detect a task switch
    sti;
