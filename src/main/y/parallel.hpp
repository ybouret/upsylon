//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/object.hpp"
#include "y/parops.hpp"
#include "y/memory/xslot.hpp"
#include "y/memory/groove.hpp"

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
        typedef memory::groove cache_type; //!< cache for extra data


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
        cache_type       & operator*()       throw();   //!< cache access
        const cache_type & operator*() const throw();   //!< cache access, const

        //! get the work portion according to rank/size
        template <typename T> inline
        void split(T &length, T &offset) const throw()
        {
            parops::split_any(length,offset,size,rank);
        }

        //! forwarding call
        template <typename T> T &       get()       throw() { return cache.get<T>(); }

        //! forwarding call, const
        template <typename T> const T & get() const throw() { return cache.get<T>(); }


        //______________________________________________________________________
        //
        // members
        //______________________________________________________________________
        cache_type   cache;    //!< local cache
        const size_t size;     //!< the family size
        const size_t rank;     //!<  0..size-1
        const size_t indx;     //!<  1..size
        const char   label[8]; //!< size.rank or "too big"
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(parallel);
        void __format() throw();
    };
}

#endif

