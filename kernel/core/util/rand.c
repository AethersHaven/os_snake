unsigned int seed = 1;

void srand()
{
    unsigned int lo, hi;
    asm volatile("rdtsc" : "=a"(lo), "=d"(hi));
    seed = lo;
}

float rand()
{
    seed = seed * 1103515245 + 12345;
    return ((seed >> 16) & 0x7FFF) / (float)0x7FFF;
}

float rand_range(float min, float max)
{
    return min + rand() * (max - min);
}