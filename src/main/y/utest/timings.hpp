//! \file
#ifndef Y_UTEST_TIMINGS_INCLUDED
#define Y_UTEST_TIMINGS_INCLUDED 1

#include "y/os/rt-clock.hpp"

namespace upsylon
{
    //! append to COUNT the number of ticks to execute CODE
#define Y_TIMINGS_TICKS(COUNT,CODE)                \
/**/do {                                           \
/**/    const uint64_t __mark = rt_clock::ticks(); \
/**/    do { CODE; } while(false);                 \
/**/    (COUNT) += (rt_clock::ticks() - __mark);   \
/**/} while(false)


    //! compute the SPEED of CODE during DURATION
#define Y_TIMINGS_(SPEED,DURATION,CODE,CYCLES)                        \
/**/do {                                                              \
/**/    upsylon::rt_clock __timing;                                   \
/**/    const uint64_t    __ini = __timing.ticks();                   \
/**/    double            __ell = 0;                                  \
/**/    const double      __all = (DURATION);                         \
/**/    size_t           &__cyc = CYCLES;                             \
/**/    __cyc=0;                                                      \
/**/    do { ++__cyc; CODE; }                                         \
/**/    while( (__ell=__timing( __timing.ticks() - __ini )) < __all );\
/**/    SPEED = double(__cyc)/__ell;                                 \
/**/} while(false)

    //! compute the SPEED of CODE during DURATION
#define Y_TIMINGS(SPEED,DURATION,CODE)            \
/**/do {                                          \
/**/    size_t __cycles=0;                        \
/**/    Y_TIMINGS_(SPEED,DURATION,CODE,__cycles); \
/**/} while(false)
    
}
#endif

