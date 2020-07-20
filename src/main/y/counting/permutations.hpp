
//! \file

#ifndef Y_COUNTING_PERMUTATIONS_INCLUDED
#define Y_COUNTING_PERMUTATIONS_INCLUDED 1

#include "y/counting/perm.hpp"
#include "y/ptr/auto.hpp"
#include "y/sort/heap.hpp"
#include "y/memory/embed.hpp"
#include "y/comparison.hpp"

namespace upsylon {


    class permutations : public counting
    {
    public:
        typedef size_t  shift_t;
        typedef uint8_t probe_t;
        
        virtual ~permutations() throw();

        

        void     setup(const accessible<size_t> &groups);

        const permutation & operator*() const throw();

    protected:
        explicit permutations() throw();
        void init_perm() throw();
        void next_perm() throw();
        const size_t                dims;  //!< perm->size()
        auto_ptr<permutation>       perm;  //!< sub-permutations
        const shift_t              *shift; //!<
        const size_t                bytes; //!< for shift

    private:
        Y_DISABLE_COPY_AND_ASSIGN(permutations);
        void cleanup() throw();


    };

#define Y_PERMUTATIONS_CTOR() \
target(0), source(0), groups(0), entry(0), length(0)

    //! permutations of integral types
    template <typename T>
    class permutations_of : public permutations, public accessible<T>
    {
    public:
        Y_DECL_ARGS(T,type);

        inline virtual ~permutations_of() throw()
        {
            static memory::allocator &mgr = counting::mem_location();
            target = 0;
            source = 0;
            groups = 0;
            mgr.release(entry,length);
        }

        inline explicit permutations_of(const accessible<T> &data) :
        permutations(), Y_PERMUTATIONS_CTOR()
        {
            initialize_with(data);
        }

        inline explicit permutations_of(const_type *buffer, const size_t buflen) :
        permutations(), Y_PERMUTATIONS_CTOR()
        {
            assert(buffer);
            assert(buflen>0);
            const lightweight_array<mutable_type> data( (mutable_type*)buffer,buflen);
            initialize_with(data);
        }

        inline virtual std::ostream &show(std::ostream &os) const
        {
            return os;
        }

        inline virtual size_t size() const throw() { return dims; }

        inline const_type & operator[](const size_t indx) const throw()
        {
            assert(indx>=1);
            assert(indx<=dims);
            return target[indx];
        }

        


    private:
        Y_DISABLE_COPY_AND_ASSIGN(permutations_of);
        mutable_type *target;
        mutable_type *source;
        size_t       *groups;
        void         *entry;
        size_t        length;

        inline void setup_memory_for(const accessible<T> &data)
        {
            const size_t n = data.size();
            memory::embed emb[] = {
                memory::embed::as(target,n),
                memory::embed::as(source,n),
                memory::embed::as(groups,n)
            };
            entry = memory::embed::create(emb, sizeof(emb)/sizeof(emb[0]), counting::mem_instance(), length);

        }

        inline void initialize_with(const accessible<T> &data)
        {
            assert(data.size()>0);

            //------------------------------------------------------------------
            // setup memory
            //------------------------------------------------------------------
            setup_memory_for(data);
            const size_t n = data.size();

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

