#include <hwy/highway.h>
#include <iostream>
int main()
{
    return (HWY_STATIC_TARGET & HWY_AVX2);
};
