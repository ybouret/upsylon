//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/object.hpp"
#include "y/parops.hpp"
#include "y/memory/shack.hpp"

namespace upsylon
{

    typedef memory::shack parallel_cache; //!< local cache is necessary

    //__________________________________________________________________________
    //
    //
    //! information and operation for parallelism
    //
    //__________________________________________________________________________
    class parallel : public parallel_cache
    {
    public:

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~parallel() throw();                                 //!< destructor
        explicit parallel() throw();                                 //!< sequential setup
        explicit parallel(const size_t sz, const size_t rk) throw(); //!< parallel

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        double efficiency(const double speed_up) const; //!< compute efficiency, two significative figures

        //! get the work portion according to rank/size
        template <typename T> inline
        void split(T &user_length, T &user_offset) const throw()
        {
            parops::split_any(user_length,user_offset,size,rank);
        }

        //! get system ticks, thread-safe
        uint64_t     ticks(lockable &) const throw();

        

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________

    public:
        const size_t   size;     //!< the family size
        const size_t   rank;     //!< 0..size-1
        const size_t   indx;     //!< 1..size
        const char     label[8]; //!< size.rank or "too big"
        

    private:
        Y_DISABLE_COPY_AND_ASSIGN(parallel);
        void __format() throw();
    };
}

#endif

