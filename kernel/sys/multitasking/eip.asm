.global read_eip
.type read_eip, @function

read_eip:
    mov (%esp), %eax
    ret
