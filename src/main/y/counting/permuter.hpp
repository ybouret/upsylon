
//! \file

#ifndef Y_COUNTING_PERMUTER_INCLUDED
#define Y_COUNTING_PERMUTER_INCLUDED 1

#include "y/counting/counting.hpp"
#include "y/counting/ops.hpp"

#include "y/core/list.hpp"
#include "y/core/node.hpp"

namespace upsylon {
    
    namespace core {

        //----------------------------------------------------------------------
        //
        //! base class for object permuter
        //
        //----------------------------------------------------------------------
        class permuter : public upsylon::counting
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef core::cpp_node_of<size_t> repeat;  //!< multiple number count
            typedef core::list_of_cpp<repeat> repeats; //!< repeats decomposition

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! cleanup
            virtual ~permuter() throw();
            

            

            //! count and set weak if a repetition is detected
            mpl::natural count_with(const repeats &, const upsylon::counting::with_mp_t &) const;
          
            //! converting from mp version
            size_t       count_with(const repeats &, const upsylon::counting::with_sz_t &) const;

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const size_t dims; //!< 1..dims objects
            const bool   weak; //!< weak if there are some repeats
            
        protected:
            //! setup, just check that dims=n>0
            explicit permuter(const size_t n);
            
            size_t      *perm; //!< user's set perm [1...dims]
            
            void init_perm() throw();       //!< reset perm
            void next_perm() throw();       //!< compute valid next permutation
            void invalid_first_key() const; //!< shouldn't happen
            
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
    
    //! setup permuter for N items
#define Y_PERMUTER_CTOR(N) \
core::permuter(N),         \
target(0),                 \
source(0),                 \
kstore(),                  \
wksp(0),                   \
wlen(0)
    
    
    //--------------------------------------------------------------------------
    //! permuter of integral type
    /**
     produce all the distinct permutations of given objects
     */
    //--------------------------------------------------------------------------
    template <typename T>
    class permuter : public core::permuter, public accessible<T>
    {
    public:
        //----------------------------------------------------------------------
        //
        // types and definitions
        //
        //----------------------------------------------------------------------
        Y_DECL_ARGS(T,type);                    //!< aliases
        typedef suffix_store<type> store_type;  //!< alias

        //----------------------------------------------------------------------
        //
        // C++
        //
        //----------------------------------------------------------------------
        //! full setup
        inline explicit permuter( const accessible<T> &seq ) :  Y_PERMUTER_CTOR( seq.size() )
        {
            setup_memory();
            copy_content(seq);
            build_extent();
        }
        
        //! full setup from a C-buffer of objects
        inline explicit permuter( const_type *buffer, const size_t buflen ) : Y_PERMUTER_CTOR(buflen)
        {
            setup_memory();
            copy_content(buffer,buflen);
            build_extent();
        }
        
        
        //! cleanup
        inline virtual ~permuter() throw()
        {
            release();
        }

        //! display
        inline virtual std::ostream &show(std::ostream &os) const
        {
            const accessible<T> &self = *this;
            return os << self;
        }

        //----------------------------------------------------------------------
        //
        // accessible interface
        //
        //----------------------------------------------------------------------

        //! number of initial used objects
        inline virtual size_t       size() const throw() { return dims; }
        
        //! object[1..size]
        inline virtual const_type & operator[](const size_t index) const
        {
            assert(index>0);
            assert(index<=dims);
            return target[index];
        }

        //----------------------------------------------------------------------
        //
        // specific methods
        //
        //----------------------------------------------------------------------

        //! return internal store
        inline const suffix_store<type> & store() const throw()
        {
            return kstore;
        }
        
        //! return required nodes so far
        inline size_t required_nodes() const throw()
        {
            return kstore.nodes + kstore.cache.size - 1;
        }
        
        //! cleanup cache
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
        
        
        // release all memory
        inline void release() throw()
        {
            static memory::allocator &mmgr = counting::mem_location();
            mmgr.release(wksp,wlen);
        }

        // build the underlying structure
        inline void build_extent()
        {

            try {

                //______________________________________________________________
                //
                // once init : sort objects by "decreasing" value
                //______________________________________________________________
                lightweight_array<mutable_type> arr( &source[1], dims);
                hsort(arr, comparison::decreasing<mutable_type> );

                //______________________________________________________________
                //
                // build first configuration with the identity permutation
                //______________________________________________________________
                initialize();

                //______________________________________________________________
                //
                // check repetitions
                //______________________________________________________________
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
                            reps.push_back( new repeat(num) );
                        }
                    }
                }

                //______________________________________________________________
                //
                // computing effective number of permutations
                //______________________________________________________________
                aliasing::_(count) = count_with(reps,counting::with_sz);
                
            }
            catch(...)
            {
                release();
                throw;
            }
        }
        
        inline void copy_content( const accessible<T> &seq )
        {
            assert( seq.size() >= dims );
            for(size_t i=dims;i>0;--i)
            {
                source[i] = seq[i];
            }
          
        }
        
        inline void copy_content( const_type *buffer, const size_t buflen )
        {
            assert( buflen >= dims );
            const_type *last = buffer + buflen;
            for(size_t i=dims;i>0;--i)
            {
                source[i] = *(--last);
            }
        }
        
        
        // allocated memory
        inline void setup_memory()
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
        inline void initialize()
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
                invalid_first_key();
            }
        }
        
        inline virtual void onBoot()
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

