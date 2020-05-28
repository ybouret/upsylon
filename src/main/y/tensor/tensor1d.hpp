//! file
#ifndef Y_TENSOR1D_INCLUDED
#define Y_TENSOR1D_INCLUDED 1

#include "y/type/self-destruct.hpp"
#include "y/type/block/zset.hpp"
#include "y/sequence/addressable.hpp"
#include "y/memory/embed.hpp"

namespace upsylon {

    namespace core
    {
        struct tensor
        {
            static memory::allocator & instance();
            static memory::allocator & location() throw();
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
        bytes(0),
        items(0),
        built(0),
        where(0)
        {
            memory::embed emb[] =
            {
                memory::embed::as(items,count)
            };
            where = memory::embed::create(emb,sizeof(emb)/sizeof(emb[0]),core::tensor::instance(),bytes,NULL);
            build(where);
        }

        inline virtual size_t       size()                  const throw() { return count; }
        inline virtual type       & operator[](size_t indx)       throw() { assert(indx>0); assert(indx<=count); return items[indx]; }
        inline virtual const_type & operator[](size_t indx) const throw() { assert(indx>0); assert(indx<=count); return items[indx]; }

        const size_t  count;


    private:
        size_t        bytes; //!< private bytes
        mutable_type *items;
        size_t        built;
        void         *where;
        
        inline void cleanup() throw()
        {
            while(built>0)
            {
                self_destruct( items[built--] );
            }
            items = 0;
            if(bytes>0)
            {
                assert(where);
                core::tensor::location().release(where,bytes);
                assert(0==where);
                assert(0==bytes);
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
