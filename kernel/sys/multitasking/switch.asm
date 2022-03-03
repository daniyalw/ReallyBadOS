[GLOBAL perform_task_switch]
; perform_task_switch(eip, ebp, esp)
perform_task_switch:
     cli;
     mov ecx, [esp+4]   ; EIP
     mov ebp, [esp+8]  ; EBP
     mov esp, [esp+12]  ; ESP
     mov eax, 0x12344   ; magic number to detect a task switch
     sti;
     jmp ecx
