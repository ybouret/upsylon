
//! \file
#ifndef Y_PTR_MEMBLK_INCLUDED
#define Y_PTR_MEMBLK_INCLUDED 1

#include "y/ptr/ptr.hpp"
#include "y/memory/allocator/global.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    //__________________________________________________________________________
    //
    //
    //! base class for auto pointer of blocks
    //
    //__________________________________________________________________________
    template <typename T, typename ALLOCATOR = memory::global >
    class memblk : public ptr<T>
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type); //!< aliases

        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        //! cleanup
        virtual ~memblk() throw()
        {
            static ALLOCATOR &mgr = ALLOCATOR::location();
            mgr.release( *(void **)& (this->pointee), aliasing::_(bytes) );
            aliasing::_(count) = 0;
        }

        //______________________________________________________________________
        //
        // other methods
        //______________________________________________________________________
        //! clear memory
        inline void clear() throw()
        {
            memset(this->pointee,0,this->bytes);
        }

        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        const size_t  count; //!< available blocks max_of(required,1)
        const size_t  bytes; //!< system's bytes

    protected:
        //! acquire memory
        explicit memblk(const size_t required) :
        ptr<T>(0),
        count(required?required:1),
        bytes( count*sizeof(T) )
        {
            this->pointee = static_cast<type *>(ALLOCATOR::instance().acquire( aliasing::_(bytes) ) );
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(memblk);
    
    };
}

#endif

