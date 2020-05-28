
#include "y/tensor/tensor1d.hpp"
#include "y/memory/global.hpp"
#include "y/exception.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon {

    namespace core
    {
        
        tensor1d:: ~tensor1d() throw()
        {
            static memory::allocator &mgr = memory::global::location();
            
            assert(0==built);
            _bzset(cols);
            if(bytes>0)
            {
                assert(NULL!=where);
                mgr.release(where,bytes);
            }
            else
            {
                assert(NULL==where);
            }
        }
        
        tensor1d:: tensor1d(const size_t c) :
        cols(c),
        built(0),
        bytes(0),
        where(0)
        {
            if(cols<=0) throw exception("tensor1d #cols=0");
        }
        
        void tensor1d:: create(memory::embed emb[],const size_t num)
        {
            static memory::allocator &mgr = memory::global::instance();
            assert(num>0);
            assert(emb!=NULL);
            where  = memory::embed::create(emb,num,mgr,bytes,NULL);
        }
        
        size_t tensor1d:: allocated() const throw()
        {
            return bytes;
        }


        
        
    }
}

