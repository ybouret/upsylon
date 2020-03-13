
//! \file

#ifndef Y_COUNTING_PERMUTER_INCLUDED
#define Y_COUNTING_PERMUTER_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/counting/ops.hpp"

#include "y/core/list.hpp"
#include "y/core/node.hpp"

namespace upsylon {

    namespace core {

        class permuter : public upsylon::counting
        {
        public:
            typedef core::cpp_node_of<size_t> repeat;
            typedef core::list_of_cpp<repeat> repeats;

            virtual ~permuter() throw();

            mpl::natural count_with(const repeats &reps, const upsylon::counting::with_mp_t &) const;
            size_t       count_with(const repeats &reps, const upsylon::counting::with_sz_t &) const;

        protected:
            explicit permuter(const size_t n);

            const size_t dims;
            size_t      *perm;

            void init_perm() throw(); //!< reset perm


        private:
            Y_DISABLE_COPY_AND_ASSIGN(permuter);
        };
    }

}

#include "y/type/aliasing.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/memory/allocator.hpp"
#include "y/memory/embed.hpp"
#include "y/sequence/array.hpp"

namespace upsylon {

#define Y_PERMUTER_CTOR(N) \
core::permuter(N),\
curr(0),  \
prev(0),  \
wksp(0),  \
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


        // release all memoru
        inline void release() throw()
        {
            static memory::allocator &mmgr = counting::mem_location();
            mmgr.release(wksp,wlen);
        }

        inline void build_extent()
        {
            try {

                // sort current, copy to previous
                initialize();

                // check repetitions
                {
                    repeats reps;
                    {
                        size_t       i = dims;
                        while(i>0)
                        {
                            const_type & t = curr[i];
                            size_t       j = i-1;
                            while(j>0&&curr[j]== t)
                            {
                                --j;
                            }
                            const size_t num = i-j;
                            i=j;
                            std::cerr << '(' << t << ')' << 'x' << num << std::endl;
                            if(num>1) reps << num;
                        }
                    }
                    aliasing::_(count) = count_with(reps,counting::with_sz);
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
        }

        void initialize() throw()
        {
            init_perm();
            lightweight_array<T> arr( &curr[1], dims);
            hsort(arr, comparison::decreasing<T> );
            for(size_t k=dims;k>0;--k)
            {
                prev[k] = curr[k];
            }
        }

        virtual void start_() throw()
        {
            initialize();
        }

        virtual void next_() throw()
        {

        }

        
    };
    
}

#endif

