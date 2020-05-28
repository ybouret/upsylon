
#include "y/tensor/tensor3d.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace core
    {
        
        tensor3d:: ~tensor3d() throw()
        {
            _bzset(slices);
            _bzset(it3d);
        }
        
        tensor3d:: tensor3d(const size_t r, const size_t c, const size_t s) :
        tensor2d(r,c),
        slices(s),
        it3d(it2d*slices)
        {
            if(slices<=0) throw exception("tensor3d #slices=0");
        }
        
        
    }
}
