//! \file
#ifndef Y_OS_RT_CLOCK_INCLUDED
#define Y_OS_RT_CLOCK_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{

    //! real time clock interface
    class rt_clock
    {
    public:
        //! frequency to convert ticks to seconds
#if defined(Y_APPLE) || defined(Y_WIN)
        typedef double   freq_type;
#else
        typedef uint64_t freq_type;
#endif
        explicit rt_clock();         //!< calibrate during constructor
        virtual ~rt_clock() throw(); //!< destructor

        static uint64_t  ticks();     //!< system ticks
        void             calibrate(); //!< called during constructor, needed to transforms ticks to seconds.
        double           operator()(uint64_t num_ticks) const throw(); //!< ticks to seconds conversion
        void             sleep( const double nsec ) const;             //!< loop for nsec
        //! compute a speed up with the given precision
        double           speedup( const uint64_t full, const uint64_t fast, const size_t precision=3) const;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(rt_clock);
        freq_type freq;
    };

    //! stopwatch interface
    class stopwatch : public rt_clock
    {
    public:
        virtual ~stopwatch() throw(); //!< destructor

        virtual void     start()       = 0; //!< initialize from rt_clock
        virtual uint64_t count() const = 0; //!< ellapsed tikcs
        double           query() const;     //!< convert current count into secondss

    protected:
        explicit stopwatch(); //!< setup

    private:
        Y_DISABLE_COPY_AND_ASSIGN(stopwatch);
    };

}

#endif

