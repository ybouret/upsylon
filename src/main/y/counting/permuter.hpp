
//! \file

#ifndef Y_COUNTING_PERMUTER_INCLUDED
#define Y_COUNTING_PERMUTER_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/counting/ops.hpp"
#include "y/sort/heap.hpp"
#include "y/memory/allocator.hpp"
#include "y/memory/embed.hpp"

namespace upsylon {


    //! permuter of integral type
    template <typename T>
    class permuter : public accessible<T>
    {
    public:
        Y_DECL_ARGS(T,type);

        explicit permuter( const accessible<T> &seq ) :
        dims( seq.size() ),
        curr(0),
        prev(0),
        perm(0),
        wksp(0),
        wlen(0)
        {
            setup_memory();
            make_content(seq);
        }
        
        virtual ~permuter() throw()
        {
            static memory::allocator &mmgr = counting::mem_location();
            mmgr.release(wksp,wlen);
        }

        virtual size_t       size() const throw() { return dims; }
        virtual const_type & operator[](const size_t index) const
        {
            assert(index>0);
            assert(index<=dims);
            return curr[dims];
        }


        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(permuter);
        const size_t dims;
        T           *curr;
        T           *prev;
        size_t      *perm;
        void        *wksp;
        size_t       wlen;

        void make_content( const accessible<T> &seq )
        {
            assert( seq.size() == dims );

        }

        void setup_memory()
        {
            static memory::allocator &mmgr = counting::mem_instance();
            memory::embed emb[] =
            {
                memory::embed::as(curr,dims),
                memory::embed::as(prev,dims),
                memory::embed::as(perm,dims)
            };
            wksp = memory::embed::create(emb,sizeof(emb)/sizeof(emb[0]),mmgr,wlen);
            --curr;
            --prev;
            --perm;
            core::counting::init(perm,dims);
        }

        
    };
    
}

#endif

