#pragma once

namespace Math {

#define PI 3.14

#define MIN(x, y) ((x) < (y) ? (x) : (y))
#define MAX(x, y) ((x) > (y) ? (x) : (y))

#define clamp(v, lower, upper) (MAX(MIN((v), (upper)), (lower)))

int sqrt(int x);
int abs(float x);
int abs(int x);
int floor(float x);
int floor(int x);
int round(double x);
int ceil(float num);

}
