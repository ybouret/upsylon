#ifndef Y_OS_WTIME_INCLUDED
#define Y_OS_WTIME_INCLUDED 1

#include "y/os/rt-clock.hpp"

namespace upsylon
{
    class wtime : public rt_clock
    {
    public:
        explicit wtime(); //!< set initial mark
        virtual ~wtime() throw(); //!< destructor

        void     reset();
        uint64_t count() const;
        double   query() const;



    private:
        uint64_t mark;
        Y_DISABLE_COPY_AND_ASSIGN(wtime);
    };
}


#endif

