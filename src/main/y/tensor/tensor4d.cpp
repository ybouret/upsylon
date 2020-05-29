#include "y/tensor/tensor4d.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace core
    {
        
        tensor4d:: ~tensor4d() throw()
        {
            _bzset(items_per_view);
        }
        
        tensor4d:: tensor4d(const size_t r,
                            const size_t c,
                            const size_t s,
                            const size_t f) :
        tensor3d(r,c,s),
        items_per_view(items_per_frame*f)
        {
            if(f<=0) throw exception("tensor4d #frames=0");
            record(f);
        }
        
        
    }
}
