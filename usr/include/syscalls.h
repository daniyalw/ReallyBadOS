#pragma once

#define PRINT 0
#define PUTCHAR 1
#define RANDOM 2
#define TIME 3
#define PUTPIXEL 4
#define UPDATE_MOUSE 5
#define INFO 6
#define FOPEN 7
#define FCLOSE 8
#define FEXEC 9
#define FLS 10
#define MKDIR 11
#define MKFILE 12
#define WRITE_FILE 13
#define MALLOC 14
#define FREE 15
#define APPEND_FILE 16
#define READ_FILE 17
#define CREATE_PROC 18
#define YIELD_PROC 19
#define EXIT_PROC 20
#define GETENV 21
#define SETENV 22
#define REALLOC 23
#define GET_TAG 24
#define IPC_SEND 25
#define IPC_RECV 26
#define TASKING_NAME 27

#define CALL_SYS0(ret, num) \
        asm volatile("int $48" : "=a" (ret) : "0" (num));

#define CALL_SYS1(ret, num, p1) \
 asm volatile("int $48" : "=a" (ret) : "0" (num), "b" (p1));

#define CALL_SYS2(ret, num, p1, p2) \
    asm volatile("int $48" : "=a" (ret) : "0" (num), "b" (p1), "c" (p2));

#define CALL_SYS3(ret, num, p1, p2, p3) \
    asm volatile("int $48" : "=a" (ret) : "0" (num), "b" (p1), "c" (p2), "d" (p3));

#define CALL_SYS4(ret, num, p1, p2, p3, p4) \
    asm volatile("int $48" : "=a" (ret) : "0" (num), "b" (p1), "c" (p2), "d" (p3), "S" (p4));

#define CALL_SYS5(ret, num, p1, p2, p3, p4, p5) \
    asm volatile("int $48" : "=a" (ret) : "0" (num), "b" (p1), "c" (p2), "d" (p3), "S" (p4), "D" (p5));
