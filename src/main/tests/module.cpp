#include "y/rtld/export.h"

#include <cmath>

Y_EXPORT
double Y_DLL_API  Sin( double x )
{
    return sin(x);
}

Y_EXPORT int32_t Data = 1;
