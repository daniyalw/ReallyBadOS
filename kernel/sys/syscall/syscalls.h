extern "C" void call_sys0(int num)
{
    void * a;
 asm volatile("int $48" : "=a" (a) : "0" (num));
}

extern "C" void call_sys1(int num, auto p1)
{
    void * a;
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1));
}
extern "C" void call_sys2(int num, auto p1, auto p2)
{
    void * a;
 asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2));
}

extern "C" void call_sys3(int num, auto p1, auto p2, auto p3)
{
    void * a;
    asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2), "d" (p3));
}

extern "C" void call_sys4(int num, auto p1, auto p2, auto p3, auto p4)
{
    void * a;
    asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2), "d" (p3), "S" (p4));
}

extern "C" void call_sys5(int num, auto p1, auto p2, auto p3, auto p4, auto p5)
{
    void * a;
    asm volatile("int $48" : "=a" (a) : "0" (num), "b" (p1), "c" (p2), "d" (p3), "S" (p4), "D" (p5));
}
