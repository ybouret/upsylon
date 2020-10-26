#include "y/rtld/dll.h"
#include <cmath>

#include <cstdio>

static inline void enter()
{
    if(stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Entering DLL\n");
        fflush(stderr);
    }
}

static inline void leave()
{
    if(stderr)
    {
        fflush(stderr);
        fprintf(stderr, "Leaving DLL\n");
        fflush(stderr);
    }
}

Y_DLL_SETUP(enter,leave);

Y_DLL_EXTERN()

Y_EXPORT double Y_DLL_API Sin( double x )
{
    return sin(x);
}

Y_EXPORT int32_t Data = 0x1234;


Y_DLL_FINISH()
