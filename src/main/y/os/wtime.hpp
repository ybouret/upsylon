#ifndef Y_OS_WTIME_INCLUDED
#define Y_OS_WTIME_INCLUDED 1

#include "y/os/rt-clock.hpp"

namespace upsylon
{
    //! wall time
    class wtime : public rt_clock
    {
    public:
        explicit wtime();         //!< set initial mark
        virtual ~wtime() throw(); //!< destructor

        void     start();       //!< start again, time to zero
        uint64_t count() const; //!< ellapsed ticks
        double   query() const; //!< ellapsed time in seconds



    private:
        uint64_t mark;
        Y_DISABLE_COPY_AND_ASSIGN(wtime);
    };
}


#endif

