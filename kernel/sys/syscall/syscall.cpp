#include <kernel/syscall.h>
#include <random.h>
#include <kernel/power.h>
#include <drivers/mouse/cursor.h>

void set_string(char * string, char * newvalue)
{
    for (int z = 0; z < std::len(string); z++)
    {
        string[z] = ' ';
    }

    for (int z = 0; z < std::len(newvalue); z++)
    {
        string[z] = newvalue[z];
        string[z+1] = 0;
    }
}

// ----------------------------- //

void print(char * text)
{
    printf(text);
}

void s_putchar(char text)
{
    putchar(text);
}

void get_num(int * num)
{
    num[0] = get_random_number();
}

void s_get_time(time_t time[1])
{
    time[0] = Time::get_time();
}

void s_putpixel(int x, int y, int color)
{
    Graphic::SetPixel(x, y, color);
}

void test(int num)
{
    num = 105;
}

void s_update_mouse()
{
    next_char();
    Kernel::update_hardware_cursor(text_x, text_y);
}

DEFN_SYSCALL1(print, 0, char *);

DEFN_SYSCALL1(s_putchar, 1, char);

DEFN_SYSCALL1(get_num, 2, int *);

DEFN_SYSCALL1(s_get_time, 3, time_t *);

DEFN_SYSCALL2(s_free, 5, int, int);

DEFN_SYSCALL3(s_putpixel, 6, int, int, int);

DEFN_SYSCALL1(test, 7, int);

DEFN_SYSCALL0(s_update_mouse, 8);

// ----------------------------- //

void sys_putchar(char text)
{
    syscall_s_putchar(text);
}

void sys_print(char *text, ...)
{
  char **arg = (char **) &text;
  int c;
  char buffer[20];

  arg++;

  while ((c = *text++) != 0)
    {
      if (c != '%')
      {
        if (c == '\n')
        {
            text_x = 0;
            text_y++;
        }
        else if (c == '\t')
        {
            for (int z = 0; z < 4; z++) sys_putchar(' ');
        }
        else
        {
            sys_putchar(c);
        }
      }
      else
        {
          char *p, *p2;
          int pad0 = 0, pad = 0;

          c = *text++;
          if (c == '0')
            {
              pad0 = 1;
              c = *text++;
            }

          if (c >= '0' && c <= '9')
            {
              pad = c - '0';
              c = *text++;
            }

          switch (c)
            {
            case 'c':
                sys_putchar(c);
                break;
            case 'd':
            case 'u':
            case 'x':
              std::itoa (buffer, c, *((int *) arg++));
              p = buffer;
              goto string;
              break;

            case 's':
              p = *arg++;
              if (! p)
                p = "(null)";

            string:
              for (p2 = p; *p2; p2++);
              for (; p2 < p + pad; p2++)
              {
                sys_putchar (pad0 ? '0' : ' ');
              }
              while (*p)
              {
                sys_putchar (*p++);
              }
              break;

            default:
              sys_putchar (*((int *) arg++));
              break;
            }
        }
    }
}

int sys_rand()
{
    int a[1];
    syscall_get_num(a);
    return a[0];
}

time_t sys_get_time()
{
    time_t time[1];
    syscall_s_get_time(time);
    return time[0];
}

void sys_putpixel(int x, int y, int color)
{
    syscall_s_putpixel(x, y, color);
}

int sys_test()
{
    int num[1];
    syscall_test(num[0]);
    return num[0];
}

// ----------------------------- //


void syscall_append(void *func)
{
    syscalls.push_back((void *)func);
}

void syscall_handler(registers_t regs)
{
   if (regs.eax >= syscalls.size())
       return;

   void *location = syscalls.get(regs.eax);

   int ret;

   asm volatile (" \
     push %1; \
     push %2; \
     push %3; \
     push %4; \
     push %5; \
     call *%6; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
     pop %%ebx; \
   " : "=a" (ret) : "r" (regs.edi), "r" (regs.esi), "r" (regs.edx), "r" (regs.ecx), "r" (regs.ebx), "r" (location));
   //regs.eax = ret;
}

namespace Kernel {

void init_syscalls()
{
    syscall_append((void *)print);
    syscall_append((void *)s_putchar);
    syscall_append((void *)get_num);
    syscall_append((void *)s_get_time);
    syscall_append((void *)s_putpixel);
    syscall_append((void *)s_update_mouse);
    Kernel::register_interrupt_handler(IRQ16, syscall_handler);
    Kernel::system_log("Syscalls initialized at interrupt 48!");
}

}
