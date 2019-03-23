//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/object.hpp"
#include "y/memory/xslot.hpp"
#include <cstring>

namespace upsylon
{
    typedef memory::xslot<memory::global> parallel_cache; //!< alias for base type

    //! information and operation for parallelism
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
        const char   label[8]; //!< size.rank
        
        //! get the work portion according to rank/size
        template <typename T>
        inline void split( T &length, T &offset ) const throw()
        {
            T todo(length/size);
            for(size_t r=1;r<=rank;++r)
            {
                length -= todo;
                offset += todo;
                todo    = length/(size-r);
            }
            length = todo;
        }


        //! compute efficiency, two significative figures
        double efficiency(const double speed_up) const;

    private:
        Y_DISABLE_COPY_AND_ASSIGN(parallel);
        void __format() throw();
    };
}

#endif

