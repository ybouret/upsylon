//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/os/platform.hpp"
#include <cmath>

namespace upsylon
{
    //! information and operation for parallelism
    class parallel
    {
    public:
        
        //! destructor
        inline virtual ~parallel() throw() {}

        //! sequential
        inline explicit parallel() throw() : size(1),rank(0),indx(1),priv(0),label() { __format(); }

        //! parallel
        inline explicit parallel(const size_t sz, const size_t rk) throw() :
        size(sz),rank(rk),indx(rk+1),priv(0),label()
        {
            assert(size>0); assert(rank<size);
            __format();
        }



        const size_t size;     //!< the family size
        const size_t rank;     //!<  0..size-1
        const size_t indx;     //!<  1..size
        void        *priv;     //!< private data
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
        inline double efficiency(const double speed_up)
        {
            if(size<=1)
            {
                return 100.0;
            }
            else
            {
                return floor(10000.0*(speed_up-1.0)/(size-1))/100.0;
            }
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(parallel);
        void __format() throw();
    };
}

#endif

