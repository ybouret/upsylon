
#include "y/os/wtime.hpp"

namespace upsylon
{

    wtime:: wtime() : stopwatch(), mark( ticks() )
    {
    }

    wtime:: ~wtime() throw()
    {
    }

    void wtime:: start()
    {
        calibrate();
        mark = ticks();
    }

    uint64_t wtime:: count() const
    {
        return ticks() - mark;
    }
    
}

