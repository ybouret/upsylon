#include "y/gfx/area.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"

namespace upsylon
{
    namespace GFX
    {
        Area:: ~Area() throw()
        {
            _bzset(x);
            _bzset(y);
            _bzset(w);
            _bzset(h);
            _bzset(n);
        }

        static const char fn[] = "Area";

        Area:: Area(const unit_t X, const unit_t Y, const unit_t W, const unit_t H) :
        x(X),
        y(Y),
        w( Check::GEQZ(W, Check::Width, fn) ),
        h( Check::GEQZ(H, Check::Height,fn) ),
        n(w*h)
        {
            
        }
    }
}
