#include "y/os/real-time-clock.hpp"
#include "y/exceptions.hpp"
#include "y/concurrent/mutex.hpp"

#if defined(Y_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#endif

#if defined(Y_LINUX) || defined(Y_FREEBSD) || defined(Y_SUNOS) || defined(Y_OPENBSD)
#define Y_USE_CLOCK_GETTIME 1
#endif

#if defined(Y_USE_CLOCK_GETTIME)
#include <sys/time.h>
#include <cerrno>
#include <iostream>
#endif

#include <cmath>
#include "y/type/utils.hpp"
#include "y/core/ipower.hpp"

namespace upsylon
{

#if defined(Y_APPLE)

    void real_time_clock::calibrate()
    {
        Y_GIANT_LOCK();
        mach_timebase_info_data_t timebase;
        const kern_return_t err = mach_timebase_info(&timebase);
        if(err != KERN_SUCCESS)
        {
            throw imported::exception("mach_timebase_info","%s",mach_error_string(err));
        }
        double conversion_factor = double(timebase.numer) / timebase.denom;
        freq = 1e-9 * conversion_factor;
    }

    uint64_t real_time_clock:: ticks()
    {
        Y_GIANT_LOCK();
        return mach_absolute_time();
    }

    double   real_time_clock:: operator()(uint64_t num_ticks) const throw()
    {
        return freq * double(num_ticks);
    }

#endif

#if defined(Y_USE_CLOCK_GETTIME)

    static const uint64_t __giga64 = Y_U64(0x3B9ACA00);

    void real_time_clock:: calibrate()
    {
        Y_GIANT_LOCK();
        struct timespec tp  = { 0, 0 };
        const int       err = clock_getres( CLOCK_REALTIME, &tp );
        if(err!=0)
            throw libc::exception( errno, "clock_getres" );
        const uint64_t res = __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
        freq = res;
    }

    uint64_t real_time_clock:: ticks()
    {
        Y_GIANT_LOCK();
        struct timespec tp  = { 0, 0 };
        const int       err = clock_gettime( CLOCK_REALTIME, &tp );
        if(err!=0)
            throw libc::exception( errno, "clock_gettime" );

        return __giga64*uint64_t(tp.tv_sec) + uint64_t(tp.tv_nsec);
    }

    double real_time_clock:: operator()( uint64_t num_ticks ) const throw()
    {
        return 1e-9 * double(num_ticks);
    }
#endif


#if defined(Y_WIN)
    void real_time_clock:: calibrate()
    {
        static const long double l_one = 1;
        Y_GIANT_LOCK();
        LARGE_INTEGER F;
        if( ! :: QueryPerformanceFrequency( &F ) )
        {
            throw win32::exception( ::GetLastError(), "::QueryPerformanceFrequency" );
        }
        freq = l_one / static_cast<long double>( F.QuadPart );
    }

    uint64_t real_time_clock:: ticks()
    {
        Y_GIANT_LOCK();
        int64_t Q = 0;
        if( ! ::QueryPerformanceCounter( (LARGE_INTEGER *)&Q)  )
        {
            throw win32::exception( ::GetLastError(), " ::QueryPerformanceCounter" );
        }
        return uint64_t(Q);
    }

    double real_time_clock:: operator()( uint64_t num_clicks ) const throw()
    {
        return freq * double(num_clicks);
    }
#endif


    real_time_clock:: real_time_clock() : freq(0)
    {
        calibrate();
    }

    real_time_clock:: ~real_time_clock() throw()
    {
        freq=0;
    }

    void real_time_clock:: sleep(const double nsec) const
    {
        const uint64_t start = ticks();
        while( (*this)(ticks()-start) < nsec )
            ;
    }

    double real_time_clock:: speedup( const uint64_t full, const uint64_t fast, const size_t precision) const
    {
        if(fast<=0) throw exception("rt_clock.speedup(division by zero)");
        const double p = ipower<double>(10,precision);
        const real_time_clock &self = *this;
        return floor( p * self(full)/self(fast) + 0.5)/p;
    }
}

namespace upsylon
{
    stopwatch::  stopwatch() : real_time_clock() {}
    stopwatch:: ~stopwatch() throw() {}

    double stopwatch:: query() const
    {
        const real_time_clock &self = *this;
        return self( count() );
    }
}
