
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
            
            //! count and set weak
            mpl::natural count_with(const repeats &reps, const upsylon::counting::with_mp_t &) const;
            size_t       count_with(const repeats &reps, const upsylon::counting::with_sz_t &) const;
            
            const size_t dims;
            const bool   weak;
            
        protected:
            explicit permuter(const size_t n);
            
            size_t      *perm;
            
            void init_perm() throw(); //!< reset perm
            void next_perm() throw(); //!< compute valid next permutation
            
            void throw_invalid_first_key() const;
            
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
#include "y/associative/suffix-store.hpp"

namespace upsylon {
    
#define Y_PERMUTER_CTOR(N) \
core::permuter(N),         \
target(0),                 \
source(0),                 \
kstore(),                  \
wksp(0),                   \
wlen(0)
    
    
    
    //! permuter of integral type
    template <typename T>
    class permuter :
    public core::permuter,
    public accessible<T>
    {
    public:
        Y_DECL_ARGS(T,type);
        typedef suffix_store<type> store_type;
        
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
            return target[index];
        }
        
        inline const suffix_store<type> & store() const throw()
        {
            return kstore;
        }
        
        inline size_t required_nodes() const throw()
        {
            return kstore.nodes + kstore.cache.size - 1;
        }
        
        inline void trim() throw()
        {
            kstore.cache.release();
        }
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(permuter);
        mutable_type       *target;
        mutable_type       *source;
        store_type          kstore;
        void               *wksp;
        size_t              wlen;
        
        
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
                repeats reps;
                {
                    size_t       i = dims;
                    while(i>0)
                    {
                        const_type & t = source[i];
                        size_t       j = i-1;
                        while(j>0&&source[j]== t)
                        {
                            --j;
                        }
                        const size_t num = i-j;
                        i=j;
                        if(num>1)
                        {
                            reps << num;
                        }
                    }
                }
                
                // computing effective number of permutations
                aliasing::_(count) = count_with(reps,counting::with_sz);
                
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
                source[i] = seq[i];
            }
            lightweight_array<mutable_type> arr( &source[1], dims);
            hsort(arr, comparison::decreasing<mutable_type> );
            
        }
        
        // allocated memory
        void setup_memory()
        {
            static memory::allocator &mmgr = counting::mem_instance();
            memory::embed emb[] =
            {
                memory::embed::as(target,dims),
                memory::embed::as(source,dims),
                memory::embed::as(perm,dims)
            };
            wksp = memory::embed::create(emb,sizeof(emb)/sizeof(emb[0]),mmgr,wlen);
            --target;
            --source;
            --perm;
        }
        
        // make default permutation and target=source
        void initialize()
        {
            init_perm();
            kstore.free();
            for(size_t k=dims;k>0;--k)
            {
                assert(k==perm[k]);
                target[k] = source[k];
            }
            if( weak && !kstore.insert(target,dims) )
            {
                throw_invalid_first_key();
            }
        }
        
        virtual void onBoot()
        {
            initialize();
        }
        
        inline void next_config() throw()
        {
            next_perm();
            for(size_t k=dims;k>0;--k)
            {
                target[k] = source[ perm[k] ];
            }
        }
        
        virtual void onNext()
        {
            assert(index<=count);
            if(weak)
            {
                do
                {
                    next_config();
                }
                while( !kstore.insert(target,dims) );
            }
            else
            {
                next_config();
            }
        }
        
        
    };
    
}

#endif

