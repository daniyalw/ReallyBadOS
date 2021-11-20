const int pi = 3.1415926535;

#define PI pi

// thanks Remco123
// https://github.com/Remco123/CactusOS/blob/13ad72885c53ce25bd0315e4efc0a86883b6c11c/lib/src/math.cpp

int abs(int c)
{
    if(c == 0)
        return 0;
    if(c < 0)
        return -c;
    if(c > 0)
        return c;
    return c;
}

int sign(int c)
{
    if(c < 0)
        return -1;
    if(c == 0)
        return 0;
    if(c > 1)
        return 1;
    return 0;
}

double fabs(double x)
{
    if(x == 0.0)
        return 0;
    if(x < 0.0)
        return -x;
    if(x > 0.0)
        return x;
    return x;
}

double sin(double x)
{
    int i = 1;
    double cur = x;
    double acc = 1;
    double fact= 1;
    double pow = x;
    while (fabs(acc) > .00000001 && i < 100){
        fact *= ((2*i)*(2*i+1));
        pow *= -1 * x*x;
        acc =  pow / fact;
        cur += acc;
        i++;
    }
    return cur;
}

double cos(double x) {
    return sin(x + PI / 2.0);
}

// thanks sleepy-monax
// https://github.com/skiftOS/skift/blob/82ac3e77cb52c46b3982e60bda7a3679e0d0c211/userspace/libs/libmath/MinMax.h

#define clamp(__v, __lower, __upper) (MAX(MIN((__v), (__upper)), (__lower)))

#define MIN(__x, __y) ((__x) < (__y) ? (__x) : (__y))

#define MAX(__x, __y) ((__x) > (__y) ? (__x) : (__y))
