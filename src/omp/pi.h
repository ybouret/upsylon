#ifndef PI_INCLUDED
#define PI_INCLUDED 1

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>

#if defined(PI_OPTIMIZED)

static double pi_term(const int i)
{
    static const double value[2] = { 4.0, -4.0 };
    const        double real_i   = i;
    return value[i&1]/(real_i+real_i+1.0);
}

#endif

#endif

