#include "y/rtld/dll.h"

#include <cmath>

Y_DLL double Y_DLL_API  Sin( double x )
{
    return sin(x);
}

Y_DLL int32_t Data = 1;
