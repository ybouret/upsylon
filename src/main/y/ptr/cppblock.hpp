
//! \file
#ifndef Y_PTR_CPPBLOCK_INCLUDED
#define Y_PTR_CPPBLOCK_INCLUDED 1

#include "y/ptr/memblk.hpp"
#include "y/sequence/addressable.hpp"
#include <cstring>

namespace upsylon
{

    //__________________________________________________________________________
    //
    //
    //! block of [1..size()] zombie objects
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = memory::global >
    class cppblock : public memblk<T,ALLOCATOR>, public addressable<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);                      //!< aliases
        typedef memblk<T,ALLOCATOR> memblk_type;  //!< base class

        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        //! cleanup
        inline virtual ~cppblock() throw() {item=0;}

        //! setup
        inline explicit cppblock(const size_t required) :
        memblk_type(required),
        item(this->pointee-1)
        {
        }

        //______________________________________________________________________
        //
        // addressable
        //______________________________________________________________________

        //! access
        inline virtual type & operator[](const size_t indx) throw()
        {
            assert(indx>=1);
            assert(indx<=this->count);
            return item[indx];
        }
        
        //______________________________________________________________________
        //
        // accessible
        //______________________________________________________________________
        //! access
        inline virtual const_type & operator[](const size_t indx) const throw()
        {
            assert(indx>=1);
            assert(indx<=this->count);
            return item[indx];
        }

        //! size
        inline virtual size_t size() const throw() { return this->count; }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(cppblock);
        mutable_type *item;
        
    };
}

#endif
