//! \file
#ifndef Y_PARALLEL_INCLUDED
#define Y_PARALLEL_INCLUDED 1

#include "y/object.hpp"
#include <cmath>
#include <cstring>

namespace upsylon
{
    //! information and operation for parallelism
    class parallel
    {
    public:
        
        //! destructor
        inline virtual ~parallel() throw() { free(); }

        //! sequential
        inline explicit parallel() throw() : size(1),rank(0),indx(1),label(), space(0), bytes(0) { __format(); }

        //! parallel
        inline explicit parallel(const size_t sz, const size_t rk) throw() :
        size(sz),rank(rk),indx(rk+1),label(), space(0), bytes(0)
        {
            assert(size>0); assert(rank<size);
            __format();
        }



        const size_t size;     //!< the family size
        const size_t rank;     //!<  0..size-1
        const size_t indx;     //!<  1..size
        const char   label[8]; //!< size.rank
        void        *space;    //!< private memory space
        size_t       bytes;    //!< private memory bytes

        //! free private memory
        void free() throw();

        //! make private space of at least n clean bytes
        void make( const size_t n );

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

        //! fast cast
        template <typename T>
        T &get() throw()
        {
            assert(bytes>=sizeof(T));
            assert(space!=0);
            return *static_cast<T*>(space);
        }

        //! compute efficiency, two significative figures
        inline double efficiency(const double speed_up) const
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

