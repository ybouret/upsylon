
//! \file

#ifndef Y_COUNTING_PERMUTATIONS_INCLUDED
#define Y_COUNTING_PERMUTATIONS_INCLUDED 1

#include "y/counting/perm.hpp"
#include "y/ptr/auto.hpp"
#include "y/sort/heap.hpp"
#include "y/memory/embed.hpp"
#include "y/comparison.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {


    //__________________________________________________________________________
    //
    //
    //! base class for permutations with repetitions
    //
    //__________________________________________________________________________
    class permutations : public counting
    {
    public:
        //______________________________________________________________________
        //
        //! types and definitions
        //______________________________________________________________________
        typedef size_t  shift_t; //!< encoding shifts format
        typedef uint8_t probe_t; //!< internal base type for probing combinations

        //______________________________________________________________________
        //
        //! methods
        //______________________________________________________________________
        virtual ~permutations() throw();               //!< cleanup
        const permutation & operator*() const throw(); //!< current internal permutation

    protected:
        explicit permutations() throw();               //!< initialize
        explicit permutations(const permutations &);   //!< copy
        void     next_perm()    throw();               //!< next valid permutation

        //! setup from data decomposition in groups
        /**
         sum(groups)=dims, and factorial(dims)
         is reduced by the repetitions
         */
        void     setup(const accessible<size_t> &groups);

        const size_t                dims;  //!< perm->size()
        auto_ptr<permutation>       perm;  //!< internal permutations
        const shift_t              *shift; //!< shift to next valid permutation
        const size_t                bytes; //!< for shift

    private:
        Y_DISABLE_ASSIGN(permutations);
        void cleanup() throw();
        void acquire_shift();
    };

    //! inline initializers
#define Y_PERMUTATIONS_CTOR() \
target(0), source(0), groups(0), entry(0), space(0)

    //__________________________________________________________________________
    //
    //
    //! permutations of integral types
    //
    //__________________________________________________________________________
    template <typename T>
    class permutations_of : public permutations, public accessible<T>
    {
    public:
        //______________________________________________________________________
        //
        //! types and definitions
        //______________________________________________________________________
        Y_DECL_ARGS(T,type);

        //______________________________________________________________________
        //
        //! C++
        //______________________________________________________________________

        //! destructor
        inline virtual ~permutations_of() throw()
        {
            static memory::allocator &mgr = counting::mem_location();
            target = 0;
            source = 0;
            groups = 0;
            mgr.release(entry,space);
        }

        //! construct with some data[1..size]
        inline explicit permutations_of(const accessible<T> &data) :
        permutations(), Y_PERMUTATIONS_CTOR()
        {
            initialize_with(data);
            aliasing::_(index) = 1;
        }

        //! construct with some buffer[0..buflen-1], buflen>0
        inline explicit permutations_of(const_type *buffer, const size_t buflen) :
        permutations(), Y_PERMUTATIONS_CTOR()
        {
            assert(buffer);
            assert(buflen>0);
            const lightweight_array<mutable_type> data( (mutable_type*)buffer,buflen);
            initialize_with(data);
            aliasing::_(index) = 1;
        }

        //______________________________________________________________________
        //
        //! counting interface
        //______________________________________________________________________

        //! display current permutation
        inline virtual std::ostream &show(std::ostream &os) const
        {
            return perm->show(os);
        }

        //______________________________________________________________________
        //
        //! accesible interface
        //______________________________________________________________________

        //! size=dims
        inline virtual size_t size() const throw() { return dims; }

        //! access in 1..dims
        inline const_type & operator[](const size_t indx) const throw()
        {
            assert(indx>=1);
            assert(indx<=dims);
            return target[indx];
        }



    private:
        Y_DISABLE_COPY_AND_ASSIGN(permutations_of);
        mutable_type *target; //!< current data
        mutable_type *source; //!< original data, sorted
        size_t       *groups; //!< decomposition of original data
        void         *entry;  //!< memory entry
        size_t        space;  //!< memory space

        inline void setup_memory_for(const size_t n)
        {
            assert(n>0);
            memory::embed emb[] = {
                memory::embed::as(target,n),
                memory::embed::as(source,n),
                memory::embed::as(groups,n)
            };
            entry = memory::embed::create(emb, sizeof(emb)/sizeof(emb[0]), counting::mem_instance(), space);
        }

        inline void initialize_with(const accessible<T> &data)
        {
            assert(data.size()>0);

            //------------------------------------------------------------------
            // setup memory
            //------------------------------------------------------------------
            const size_t n = data.size();
            setup_memory_for(n);

            //------------------------------------------------------------------
            // initialize source
            //------------------------------------------------------------------
            {
                lightweight_array<mutable_type> src(source--,n);
                // copy in source
                for(size_t i=n;i>0;--i)
                {
                    source[i] = data[i];
                }
                // decreasing sort
                hsort(src,comparison::decreasing<mutable_type> );
            }

            //------------------------------------------------------------------
            // initialize target
            //------------------------------------------------------------------
            --target;
            for(size_t i=n;i>0;--i)
            {
                target[i] = source[i];
            }

            //------------------------------------------------------------------
            // initialize groups
            //------------------------------------------------------------------
            {
                size_t g = 0;
                size_t i = n;
                while(i>0)
                {
                    const_type & t = source[i];
                    size_t       j = i-1;
                    while(j>0&&source[j]<=t)
                    {
                        --j;
                    }
                    const size_t num = i-j;
                    i=j;
                    groups[g++] = num;
                }
                const lightweight_array<size_t> grp(groups,g);
                setup(grp);
            }

        }

        void update() throw()
        {
            assert(perm->good());
            const accessible<size_t> &self = *perm;
            for(size_t i=dims;i>0;--i) target[i] = source[ self[i] ];
        }

        inline virtual void onBoot()
        {
            perm->boot();
            update();
        }

        inline virtual void onNext()
        {
            next_perm();
            update();
        }

    };



}

#endif

