//! file
#ifndef Y_TENSOR1D_INCLUDED
#define Y_TENSOR1D_INCLUDED 1

#include "y/type/self-destruct.hpp"
#include "y/type/block/zset.hpp"
#include "y/sequence/addressable.hpp"

namespace upsylon {

    namespace core
    {
        struct tensor
        {
            static void *acquireMemory(size_t &n);
            static void  releaseMemory(void * &p, size_t &n) throw();
            template <typename T>
            static inline T *acquireMemoryFor(size_t &n)
            {
                n *= sizeof(T);
                return static_cast<T*>( acquireMemory(n) );
            }
        };
    }

    template <typename T>
    class tensor1d : public addressable<T>
    {
    public:
        Y_DECL_ARGS(T,type);


        //! cleanup
        inline virtual ~tensor1d() throw() { cleanup(); }

        //! setup
        inline tensor1d(const size_t n) :
        count(n),
        bytes(count),
        items(core::tensor::acquireMemoryFor<mutable_type>(bytes)),
        built(0)
        {
            build(items);
        }

        inline virtual size_t       size()                  const throw() { return count; }
        inline virtual type       & operator[](size_t indx)       throw() { assert(indx>0); assert(indx<=count); return items[indx]; }
        inline virtual const_type & operator[](size_t indx) const throw() { assert(indx>0); assert(indx<=count); return items[indx]; }

        const size_t  count;


    private:
        size_t        bytes; //!< private bytes
        mutable_type *items;
        size_t        built;

        inline void cleanup() throw()
        {
            while(built>0)
            {
                self_destruct( items[built--] );
            }
            if(bytes>0)
            {
                ++items;
                core::tensor::releaseMemory( *(void **)&items, bytes );
                assert(0==items);
                assert(0==bytes);
            }
            else
            {
                items = 0;
            }
            _bzset(count);
        }

        void build(void *addr)
        {
            assert(!(NULL==addr&&count>0));
            mutable_type *base = static_cast<mutable_type*>(addr);
            items              = base-1;
            try {
                while(built<count)
                {
                    new (base+built) mutable_type();
                    ++built;
                }
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }


        Y_DISABLE_COPY_AND_ASSIGN(tensor1d);
    };
    
}

#endif
