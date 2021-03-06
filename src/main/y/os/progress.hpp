//! \file
#ifndef Y_OS_PROGRESS_INCLUDED
#define Y_OS_PROGRESS_INCLUDED 1

#include "y/os/real-time-clock.hpp"
#include <iosfwd>

namespace upsylon
{
    //! compute progress timings
    class progress : public stopwatch
    {
    public:
        static const size_t max_days    = 99;                   //!< days limit
        static const size_t s_per_day   = 24 * 60 * 60;         //!< seconds per day
        static const size_t max_seconds = max_days * s_per_day; //!< seconds limit
        static const size_t format_size = 32;                   //!< char[format_size] for output

        explicit progress();         //!< setup
        virtual ~progress() throw(); //!< destructor

        virtual void     start() throw();      //!< initialize stopwatch and data
        virtual uint64_t count() const;        //!< ticks from last start
        void             update(double ratio); //!< update predictions 0<=ratio<=1

        //! helper
        template <typename T>
        static inline double ratio_of(const T num, const T max)
        {
            return (num>=max) ? 1.0 : double(num)/double(max);
        }

        //! wrapper to update
        template <typename T>
        inline void update( const T num, const T max )
        {
            update( ratio_of(num,max) );
        }

        //! fmt size >= format_size!
        static void format(char *fmt, const double tmx);

        //! output
        std::ostream &display(std::ostream &) const;

        //! print with minimal delay
        void print(std::ostream &,const double ratio,const double every);

        //! wrapper to print
        template <typename T>
        void print(std::ostream &os,const T num, const T max,const double every)
        {
            print(os,ratio_of(num,max),every);
        }


    private:
        Y_DISABLE_COPY_AND_ASSIGN(progress);
        uint64_t mark;
        uint64_t bips;
        double   last;
        
    public:
        const double     percent; //!< 0..100
        const double     done;    //!< ellapsed time so far
        const double     left;    //!< time needed to finish
        mutable unsigned counter; //!< counter for display
    };
}

#endif
