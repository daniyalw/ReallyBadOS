#pragma once

#include <math.h>

namespace Math {

int sqrt(int x) {
    int a = 1;
    int delta = 3;
    while (a <= x) {
        a += delta; // (a + 1)^2
        delta += 2;
    }
    return delta / 2 - 1;
}

int abs(float x) {
  if (x > 0)
    return x;
  else
    return -x;
}

int abs(int x) {
  if (x > 0)
    return x;
  else
    return -x;
}

int floor(float x) {
    if (x >= 0) {
        return (int)x;
    } else {
        int y = (int)x;
        return ((float)y == x) ? y : y - 1;
    }
}

int floor(int x) {
    if (x >= 0) {
        return x;
    } else {
        int y = x;
        return (y == x) ? y : y - 1;
    }
}

int round(double x)
{
    if (x < 0.0)
        return (int)(x - 0.5);
    else
        return (int)(x + 0.5);
}

int ceil(float num)
{   int a = num;
    if ((float)a != num)
        return a+1;
    return a;
}

int average(int * numbers, int number_size)
{
    int total = 0;

    for (int z = 0; z < number_size; z++)
    {
        total += numbers[z];
    }

    return total/number_size;
}

}
