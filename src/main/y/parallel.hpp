//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/object.hpp"
#include "y/parops.hpp"
#include "y/memory/groove.hpp"
#include "y/memory/marker.hpp"

namespace upsylon
{


    //__________________________________________________________________________
    //
    //
    //! information and operation for parallelism
    //
    //__________________________________________________________________________
    class parallel
    {
    public:
        //______________________________________________________________________
        //
        // types and definition
        //______________________________________________________________________
        typedef memory::groove              cache_type; //!< cache for extra data
        static const memory::storage::model cache_kind = memory::storage::shared; //!< inner cache

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
        void split(T &length, T &offset) const throw()
        {
            parops::split_any(length,offset,size,rank);
        }

        //! get system ticks, thread-safe
        uint64_t     ticks(lockable &) const throw();

        //! get cache
        cache_type       & operator*() throw();

        //! get cache
        const cache_type & operator*() const throw();

        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
    private:
        cache_type     L1;       //!< primary cache for outer computations
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

