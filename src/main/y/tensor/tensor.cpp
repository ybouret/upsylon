
#include "y/tensor/tensor.hpp"
#include "y/memory/global.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"
#include "y/type/aliasing.hpp"
#include <cstring>

namespace upsylon {

    namespace core {

        const size_t tensor::lo[4] = { 1,1,1,1 };

        tensor :: tensor() throw() :
        dimensions(0),
        built(0),
        bytes(0),
        where(0),
        up(),
        cols(   up[0] ),
        rows(   up[1] ),
        slices( up[2] ),
        frames( up[3] )
        {
            assert(sizeof(up)==sizeof(lo));
            memcpy(up,lo,sizeof(lo));
        }

        tensor :: ~tensor() throw()
        {
            static memory::allocator &mgr = memory::global::location();

            assert(0==built);
            if(bytes>0)
            {
                assert(NULL!=where);
                mgr.release(where,bytes);
            }
            else
            {
                assert(NULL==where);
            }
            memcpy(up,lo,sizeof(lo));
        }

        size_t tensor:: allocated() const throw()
        {
            return bytes;
        }

        void tensor:: create(memory::embed emb[],const size_t num)
        {
            static memory::allocator &mgr = memory::global::instance();
            assert(num>0);
            assert(emb!=NULL);
            where  = memory::embed::create(emb,num,mgr,bytes,NULL);
        }

        const size_t *tensor:: lower() const throw()
        {
            return lo;
        }

        const size_t *tensor:: upper() const throw()
        {
            return up;
        }

        void tensor:: set_upper(const size_t dim, const size_t num) throw()
        {
            assert(dim<sizeof(up)/sizeof(up[0]));
            assert(num>0);
            up[dim] = num;
        }

        void tensor:: record(const size_t num) throw()
        {
            set_upper( aliasing::_(dimensions)++, num);
        }

    }

}


