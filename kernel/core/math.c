#include "math.h"

int pow(int base, int exponent)
{
    int number = 1;

    if (exponent == 0)
    {
        return 1;
    }
    else if (exponent > 0)
    {
        while (exponent > 0)
        {
            number *= base;
            exponent--;
        }
    }
    else
    {
        while (exponent < 0)
        {
            number /= base;
            exponent++;
        }
    }
    return number;
}

float powf(float base, int exponent)
{
    float number = 1;

    if (exponent == 0)
    {
        return 1;
    }
    else if (exponent > 0)
    {
        while (exponent > 0)
        {
            number *= base;
            exponent--;
        }
    }
    else
    {
        while (exponent < 0)
        {
            number /= base;
            exponent++;
        }
    }
    return number;
}

int clamp(int val, int min, int max)
{
    return val > max ? max : val < min ? min
                                       : val;
}

float fclamp(float val, float min, float max)
{
    return val > max ? max : val < min ? min
                                       : val;
}

float fmod(float dividend, float divisor)
{
    if (divisor == 0.0)
    {
        return 0.0;
    }
    int quotient = (int)(dividend / divisor);
    return dividend - quotient * divisor;
}

float moddf(float x, float y) { return fmod(x, y); }

int abs(int val) { return val < 0 ? -val : val; }

float fabs(float val) { return val < 0 ? -val : val; }

float sqrt(float number)
{
    long i;
    float x2, y;
    const float threehalfs = 1.5F;

    x2 = number * 0.5F;
    y = number;
    i = *(long *)&y;           // evil floating point bit level hacking
    i = 0x5f3759df - (i >> 1); // what the fuck?
    y = *(float *)&i;
    y = y * (threehalfs - (x2 * y * y)); // 1st iteration
    y = y * (threehalfs - (x2 * y * y)); // 2nd iteration, this can be removed

    return 1 / y;
}

float fmin(float a, float b) { return a <= b ? a : b; }
float fmax(float a, float b) { return a >= b ? a : b; }
int min(int a, int b) { return a <= b ? a : b; }
int max(int a, int b) { return a <= b ? a : b; }

int factorial(int n)
{
    int result = 1;
    for (int i = 1; i <= n; ++i)
    {
        result *= i;
    }
    return result;
}

float cos(float rad)
{
    rad = moddf(rad, 2 * PI);
    if (rad < 0)
    {
        rad += 2 * PI;
    }

    int sign = 1;
    if (rad > PI)
    {
        rad -= PI;
        sign = -1;
    }

    float sum = 0;
    for (int i = 0; i <= 10; i += 2)
    {
        sum += pow(-1, i / 2) * powf(rad, i) / factorial(i);
    }

    return sign * sum;
}

float sin(float rad)
{
    return cos(PI / 2 - rad);
}

float tan(float rad)
{
    float c = cos(rad);
    float s = sin(rad);
    return s / (fabs(c) < 0.0001f ? 0.0001f : c);
}

int sign(int val)
{
    return val >= 0 ? 1 : -1;
}

int floor(float x)
{
    int xi = (int)x;
    return x < 0 && x != xi ? xi - 1 : xi;
}

int ceil(float x)
{
    int xi = (int)x;
    return x > 0 && x != xi ? xi + 1 : xi;
}