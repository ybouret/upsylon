#include "y/tensor/tensor2d.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace core
    {
        
        tensor2d:: ~tensor2d() throw()
        {
            _bzset(rows);
            _bzset(items_per_slice);
        }
        
        tensor2d:: tensor2d(const size_t r, const size_t c) :
        tensor1d(c),
        rows(r),
        items_per_slice(rows*cols)
        {
            if(rows<=0) throw exception("tensor2d #rows=0");
        }
        
        
    }
}
