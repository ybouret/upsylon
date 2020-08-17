#include "y/parallel.hpp"
#include <cstring>
#include <cstdio>
#include <iostream>
#include <cmath>

namespace upsylon
{
    parallel:: ~parallel() throw() { }

    parallel:: parallel() throw() :
    L1(), size(1),rank(0),indx(1),label()
    { __format(); }

    parallel:: parallel(const size_t sz, const size_t rk) throw() :
    L1(), size(sz),rank(rk),indx(rk+1),label()
    {
        assert(size>0); assert(rank<size);
        __format();
    }

    

    double parallel:: efficiency(const double speed_up) const
    {
        if(size<=1)
        {
            return 100.0;
        }
        else
        {
            return floor(10000.0*(speed_up-1.0)/(size-1)+0.5)/100.0;
        }
    }

    void parallel:: __format() throw()
    {
        static const size_t max_size = 999;
        char *s = (char *)label;
        memset(s,0,sizeof(label));
        if(size>max_size)
        {
            strncpy(s,"too big",sizeof(label));
        }
        else
        {
            const char *fmt = "%03u.%03u";
            if(size<100)
            {
                fmt = "%02u.%02u";
            }
            if(size<10)
            {
                fmt = "%u.%u";
            }
            snprintf(s,sizeof(label),fmt,unsigned(size),unsigned(rank));
        }
    }

    

}

#include "y/os/real-time-clock.hpp"

namespace upsylon
{
    uint64_t parallel:: ticks(lockable &access) const throw()
    {
        Y_LOCK(access);
        return real_time_clock::ticks();
    }


}
