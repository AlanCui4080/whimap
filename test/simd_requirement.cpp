#include <hwy/highway.h>
#include <iostream>
int main()
{
    return (HWY_SUPPORTED_TARGETS | HWY_AVX2);
};
