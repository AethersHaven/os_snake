#ifndef MATH_H
#define MATH_H

#define PI 3.14159265f

int pow(int base, int exponent);
float powf(float base, int exponent);
int clamp(int val, int min, int max);
float fclamp(float val, float min, float max);
float fmod(float dividend, float divisor);
float moddf(float x, float y);
int abs(int val);
float fabs(float val);
float sqrt(float number);
float fmin(float a, float b);
float fmax(float a, float b);
int min(int a, int b);
int max(int a, int b);
int factorial(int val);
float cos(float rad);
float sin(float rad);
float tan(float rad);
int sign(int val);
int floor(float x);
int ceil(float x);

#endif