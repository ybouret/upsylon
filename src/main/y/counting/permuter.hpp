
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
            const size_t dims;     //!< 1..dims objects
            const bool   weak;    //!< weak if there are some repeats
            const size_t classes; //!< number of classes
            
        protected:
            //! setup, just check that dims=n>0
            explicit permuter(const size_t n);

            //! copy parameters
            explicit permuter(const permuter &other) throw();

            size_t      *perm; //!< user's set perm [1...dims]
            
            void init_perm() throw();       //!< reset perm
            void next_perm() throw();       //!< compute valid next permutation
            void invalid_first_key() const; //!< shouldn't happen
            
        private:
            Y_DISABLE_ASSIGN(permuter);
        };
    }
    
}

#include "y/type/aliasing.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"
#include "y/memory/allocator.hpp"
#include "y/memory/embed.hpp"
#include "y/sequence/array.hpp"
#include "y/associative/suffix/store.hpp"

namespace upsylon {
    
    //! setup permuter for N items
#define Y_PERMUTER_CTOR(N) \
core::permuter(N),         \
store(),                   \
target(0),                 \
source(0),                 \
__next(0),                 \
wksp(0),                   \
wlen(0)
    
    
    //--------------------------------------------------------------------------
    //! permuter of integral types
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
        Y_DECL_ARGS(T,type);                       //!< aliases
        typedef suffix_store<type> store_type;     //!< alias
        typedef void (permuter::*next_proc)(void); //!< alias

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
            aliasing::_(index)=1;
        }
        
        //! full setup from a C-buffer of objects
        inline explicit permuter( const_type *buffer, const size_t buflen ) : Y_PERMUTER_CTOR(buflen)
        {
            setup_memory();
            copy_content(buffer,buflen);
            build_extent();
            aliasing::_(index)=1;
        }

        //! setup from another
        inline permuter(const permuter &other) :
        collection(),
        core::permuter(other),
        accessible<T>(),
        store(),
        target(0),
        source(0),
        __next(other.__next),
        wksp(0),
        wlen(0)
        {
            setup_memory();
            for(size_t i=dims;i>0;--i)
            {
                //target[i] = other.target[i];
                source[i] = other.source[i];
                //perm[i]   = other.perm[i];
            }
            boot();
            while(index<other.index)
            {
                next();
            }
            assert(index==other.index);
            assert(count==other.count);
#ifndef NDEBUG
            for(size_t i=dims;i>0;--i)
            {
                assert(perm[i]==other.perm[i]);
            }
#endif
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

        //! C-style copy current content
        inline void apply(mutable_type *dest) const throw()
        {
            assert(dest);
            for(size_t i=1;i<=dims;++i)
            {
                *(dest++) = target[i];
            }
        }

        //! C++-style copy current content
        inline void apply( addressable<T> &dest ) const throw()
        {
            for(size_t i=dims;i>0;--i) dest[i] = target[i];
        }


        //----------------------------------------------------------------------
        //
        // members
        //
        //----------------------------------------------------------------------
        store_type          store; //!< key store, shouldn't be touched during loop

    private:
        Y_DISABLE_ASSIGN(permuter);
        mutable_type       *target;
        mutable_type       *source;
        next_proc           __next;
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
                        ++aliasing::_(classes);
                    }
                }

                //______________________________________________________________
                //
                // computing effective number of permutations
                //______________________________________________________________
                aliasing::_(count) = count_with(reps,counting::with_sz);
                __next = weak ? & permuter<T>::next_config_weak : & permuter<T>::next_config;
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
            store.free();
            for(size_t k=dims;k>0;--k)
            {
                assert(k==perm[k]);
                target[k] = source[k];
            }
            if( weak && !store.insert(target,dims) )
            {
                invalid_first_key();
            }
        }

        //! re-initialize
        inline virtual void onBoot() { assert(1==index); initialize(); }

        //! generate next permutation and the config
        inline void next_config() throw()
        {
            next_perm();
            for(size_t k=dims;k>0;--k) target[k] = source[ perm[k] ];
        }

        //! generate next NEW permutation
        inline void next_config_weak()
        {
            do next_config(); while( !store.insert(target,dims) );
        }


        //! update to next config
        virtual void onNext()
        {
            assert(index<=count);
            assert(__next!=NULL);
            (this->*__next)();
        }
        
        
    };
    
}

#endif

