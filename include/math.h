#pragma once

namespace Math {

#define PI 3.14

#define MIN(__x, __y) ((__x) < (__y) ? (__x) : (__y))
#define MAX(__x, __y) ((__x) > (__y) ? (__x) : (__y))

int sqrt(int x);
int abs(float x);
int abs(int x);
int floor(float x);
int floor(int x);
int round(double x);
int ceil(float num);

}
