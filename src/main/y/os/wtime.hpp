//! \file
#ifndef Y_OS_WTIME_INCLUDED
#define Y_OS_WTIME_INCLUDED 1

#include "y/os/real-time-clock.hpp"

namespace upsylon
{
    //! wall time
    class wtime : public stopwatch
    {
    public:
        explicit wtime();         //!< set initial mark
        virtual ~wtime() throw(); //!< destructor

        virtual void     start();       //!< start again, time to zero
        virtual uint64_t count() const; //!< ellapsed ticks
        
    private:
        uint64_t mark;
        Y_DISABLE_COPY_AND_ASSIGN(wtime);
    };
}


#endif

