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
    //! alias for base type
    //
    //__________________________________________________________________________
    typedef memory::xslot<> parallel_cache;
    

    //__________________________________________________________________________
    //
    //
    //! information and operation for parallelism
    //
    //__________________________________________________________________________
    class parallel : public parallel_cache
    {
    public:
        
        //! destructor
        virtual ~parallel() throw();

        //! sequential
        explicit parallel() throw();

        //! parallel
        explicit parallel(const size_t sz, const size_t rk) throw();

        const size_t size;     //!< the family size
        const size_t rank;     //!<  0..size-1
        const size_t indx;     //!<  1..size
        const char   label[8]; //!< size.rank or "too big"
        
        //! get the work portion according to rank/size
        template <typename T>
        inline void split(T &length, T &offset) const throw()
        {
            parops::split_any(length,offset,size,rank);
        }


        //! compute efficiency, two significative figures
        double efficiency(const double speed_up) const;

      

    private:
        Y_DISABLE_COPY_AND_ASSIGN(parallel);
        void __format() throw();
    };
}

#endif

