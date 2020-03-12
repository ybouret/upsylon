
//! \file

#ifndef Y_COUNTING_PERMUTER_INCLUDED
#define Y_COUNTING_PERMUTER_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/sort/heap.hpp"
#include "y/memory/allocator.hpp"
#include "y/memory/embed.hpp"

namespace upsylon {
    
    //! permuter of integral type
    template <typename T>
    class permuter : public accessible<T>
    {
    public:
        
        explicit permuter( const accessible<T> &seq ) :
        dims( seq.size() ),
        curr(0),
        prev(0),
        perm(0)
        {
        }
        
        virtual ~permuter() throw()
        {
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(permuter);
        const size_t dims;
        T           *curr;
        T           *prev;
        size_t      *perm;
        
        
    };
    
}

#endif

