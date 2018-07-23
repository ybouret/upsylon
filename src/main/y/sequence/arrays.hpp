//! \file
#ifndef Y_ARRAYS_INCLUDED
#define Y_ARRAYS_INCLUDED 1

#include "y/sequence/array.hpp"

namespace upsylon
{
    template <typename T,typename ALLOCATOR = memory::global>
    class arrays
    {
    public:
        const size_t       count;


        inline explicit arrays(const size_t n) :
        count(n),
        hmem_( ALLOCATOR::instance() )
        {
        }

        inline virtual ~arrays() throw()
        {
        }

    private:
        Y_DISABLE_COPY_AND_ASSIGN(arrays);
        memory::allocator &hmem_;
    };
}

#endif

