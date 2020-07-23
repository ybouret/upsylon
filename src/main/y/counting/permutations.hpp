
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
    class permutations_ : public counting
    {
    public:
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        typedef size_t  shift_t; //!< encoding shifts format
        typedef uint8_t probe_t; //!< internal base type for probing combinations

        //______________________________________________________________________
        //
        // methods
        //______________________________________________________________________
        virtual ~permutations_() throw();               //!< cleanup
        const permutation & operator*() const throw(); //!< current internal permutation

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! compute number of distinct permutations based on groups description
        static mpl::natural compute(size_t &sum, const accessible<size_t> &groups, const with_mp_t &);

        //! compute number of distinct permutations based on groups description
        static size_t       compute(size_t &sum,const accessible<size_t> &groups, const with_sz_t &);

    protected:
        explicit permutations_() throw();               //!< initialize
        explicit permutations_(const permutations_ &);   //!< copy
        void     next_perm()    throw();               //!< next valid permutation

        //! setup from data decomposition in groups
        /**
         sum(groups)=dims, and factorial(dims)
         is reduced by the repetitions
         */
        void     setup(const accessible<size_t> &groups);

        auto_ptr<permutation>       perm;  //!< internal permutations
        const shift_t              *shift; //!< shift to next valid permutation
        const size_t                bytes; //!< for shift

    private:
        Y_DISABLE_ASSIGN(permutations_);
        void cleanup() throw();
        void acquire_shift();
    };

    //! inline initializers
#define Y_PERMUTATIONS_CTOR() \
accessible<T>(), target(0), source(0), groups(0), wksp(0), wlen(0)

    //__________________________________________________________________________
    //
    //
    //! permutations of integral types
    //
    //__________________________________________________________________________
    template <typename T>
    class permutations : public permutations_, public accessible<T>
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
        inline virtual ~permutations() throw()
        {
            static memory::allocator &mgr = counting::mem_location();
            target = 0;
            source = 0;
            groups = 0;
            mgr.release(wksp,wlen);
        }

        //! construct with some data[1..size]
        template <typename U>
        inline explicit permutations(const accessible<U> &data) :
        permutations_(), Y_PERMUTATIONS_CTOR()
        {
            initialize_with(data);
            aliasing::_(index) = 1;
        }

        //! construct with some buffer[0..buflen-1], buflen>0
        template <typename U>
        inline explicit permutations(const U *buffer, const size_t buflen) :
        permutations_(), Y_PERMUTATIONS_CTOR()
        {
            assert(buffer);
            assert(buflen>0);
            const lightweight_array<U> data( (U*)buffer,buflen);
            initialize_with(data);
            aliasing::_(index) = 1;
        }

        //! templated copy
        template <typename U>
        inline permutations(const permutations<U> &other) :
        permutations_(other), Y_PERMUTATIONS_CTOR()
        {
            setup_memory_for(space);
            --target;
            --source;
            --groups;
            other.copy_content(target,source,groups,space);
            assert( has_same_state_than(other) );
        }

        //! default copy
        inline permutations(const permutations &other) :
        collection(), permutations_(other), Y_PERMUTATIONS_CTOR()
        {
            setup_memory_for(space);
            --target;
            --source;
            --groups;
            other.copy_content(target,source,groups,space);
            assert( has_same_state_than(other) );
        }


        //! full check
        template <typename U>
        inline bool has_same_state_than(const permutations<U> &rhs) const throw()
        {
            const permutations<T> &lhs = *this;
            if(lhs.size() != rhs.size() ) return false;
            if(lhs.count  != rhs.count  ) return false;
            if(lhs.index  != rhs.index  ) return false;
            return permutation::are_equal(*lhs,*rhs);
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

        //! size=space
        inline virtual size_t size() const throw() { return space; }

        //! access in 1..dims
        inline const_type & operator[](const size_t indx) const throw()
        {
            assert(indx>=1);
            assert(indx<=space);
            return target[indx];
        }

        //______________________________________________________________________
        //
        //! helpers
        //______________________________________________________________________

        //! used to cross-initialize
        template <typename U>
        inline void copy_content(U * __target, U *__source, size_t * __groups, const size_t items) const throw()
        {
            assert(items==space);
            assert(__target);
            assert(__source);
            assert(__groups);
            for(size_t i=items;i>0;--i)
            {
                __target[i] = static_cast<const U>(target[i]);
                __source[i] = static_cast<const U>(source[i]);
                __groups[i] = groups[i];
            }
        }

        //! C-style copy
        template <typename U>
        inline void apply(U *buffer) const throw()
        {
            assert(buffer);
            for(size_t i=0,j=1;i<space;++i,++j)
            {
                buffer[i] = static_cast<U>( target[j] );
            }
        }

        //! reloading
        template <typename U>
        inline void reload( const permutations<U> &other ) throw()
        {
            assert(count==other.count);
            assert(size()==other.size());
            perm->reload(*other);
            aliasing::_(index) = other.index;
            if(perm->good())
            {
                update();
            }
        }

    private:
        Y_DISABLE_ASSIGN(permutations);
        mutable_type *target; //!< current data
        mutable_type *source; //!< original data, sorted
        size_t       *groups; //!< decomposition of original data
        void         *wksp;  //!< memory entry
        size_t        wlen;  //!< memory space

        inline void setup_memory_for(const size_t n)
        {
            assert(n>0);
            memory::embed emb[] =
            {
                memory::embed::as(target,n),
                memory::embed::as(source,n),
                memory::embed::as(groups,n)
            };
            wksp = memory::embed::create(emb, sizeof(emb)/sizeof(emb[0]), counting::mem_instance(), wlen);
        }

        template <typename U>
        inline void initialize_with(const accessible<U> &data)
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
                // local sort
                hsort(src,comparison::increasing<mutable_type> );
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
                    while(j>0&&t<=source[j])
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

        //! update target from current configuration
        void update() throw()
        {
            assert(perm->good());
            const accessible<size_t> &self = *perm;
            for(size_t i=space;i>0;--i) target[i] = source[ self[i] ];
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

