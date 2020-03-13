
//! \file

#ifndef Y_COUNTING_PERMUTER_INCLUDED
#define Y_COUNTING_PERMUTER_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/counting/ops.hpp"
#include "y/memory/allocator.hpp"
#include "y/memory/embed.hpp"
#include "y/sequence/array.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/core/list.hpp"
#include "y/core/node.hpp"

namespace upsylon {

    namespace core {

        class permuter {
        public:
            typedef core::cpp_node_of<size_t> repeat;
            typedef core::list_of_cpp<repeat> repeats;

            virtual ~permuter() throw();

        protected:
            explicit permuter(const size_t n);

            const size_t dims;
            size_t      *perm;

        private:
            Y_DISABLE_COPY_AND_ASSIGN(permuter);
        };
    }

#define Y_PERMUTER_CTOR(N) \
core::permuter(N),\
curr(0),\
prev(0),\
wksp(0),\
wlen(0)



    //! permuter of integral type
    template <typename T>
    class permuter : public core::permuter, public accessible<T>
    {
    public:
        Y_DECL_ARGS(T,type);

        explicit permuter( const accessible<T> &seq ) :
        Y_PERMUTER_CTOR( seq.size() )
        {
            setup_memory();
            make_content(seq);
            build_extent();
        }
        
        virtual ~permuter() throw()
        {
            release();
        }

        virtual size_t       size() const throw() { return dims; }
        virtual const_type & operator[](const size_t index) const
        {
            assert(index>0);
            assert(index<=dims);
            return curr[index];
        }


        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(permuter);
        T           *curr;
        T           *prev;
        void        *wksp;
        size_t       wlen;


        inline void release() throw()
        {
            static memory::allocator &mmgr = counting::mem_location();
            mmgr.release(wksp,wlen);
        }

        inline void build_extent()
        {
            try {
                lightweight_array<T> arr( &curr[1], dims);
                hsort(arr, comparison::decreasing<T> );
                for(size_t i=dims;i>0;--i)
                {
                    prev[i] = curr[i];
                }
            }
            catch(...)
            {
                release();
                throw;
            }
        }

        inline void make_content( const accessible<T> &seq )
        {
            assert( seq.size() >= dims );
            for(size_t i=dims;i>0;--i)
            {
                curr[i] = seq[i];
            }

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

