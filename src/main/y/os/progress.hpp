//! \file
#ifndef Y_OS_PROGRESS_INCLUDED
#define Y_OS_PROGRESS_INCLUDE 1

#include "y/os/rt-clock.hpp"

namespace upsylon
{
    class progress : public rt_clock
    {
    public:
        static const size_t max_days    = 99;
        static const size_t max_seconds = max_days * 24 * 60 * 60;
        static const size_t format_size = 16;

        explicit progress();
        virtual ~progress() throw();

        void start() throw();
        void update(  double ratio );

        template <typename T>
        void update( const T num, const T max )
        {
            update( double(num)/double(max) );
        }

        //! fmt size >= format_size!
        static void format(char *fmt, const double tmx );

    private:
        Y_DISABLE_COPY_AND_ASSIGN(progress);
        uint64_t mark;
        uint64_t bips;

    public:
        const double percent;
        const double done;
        const double left;
    };
}

#endif
