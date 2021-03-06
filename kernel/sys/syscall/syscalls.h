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
