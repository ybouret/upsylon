#include "y/gfx/area.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"

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

        static const char afn[] = "Area";

        Area:: Area(const unit_t X, const unit_t Y, unit_t W, unit_t H) :
        x(X),
        y(Y),
        w( Check::GEQZ(W, Check::Width, afn) ),
        h( Check::GEQZ(H, Check::Height,afn) ),
        xm(x+ --W),
        ym(y+ --H),
        n(w*h)
        {
            
        }

        std::ostream & operator<<(std::ostream &os, const Area &a)
        {
            const Point ini(a.x,a.y);
            const Point end(a.xm,a.ym);
            const Point len(a.w,a.h);
            os << '(' << ini << '-' << '>' << end << ':' << len << ')';
            return os;
        }


        Area Area:: getCore() const
        {
            static const char fn[] = "Area::getCore";
            if(w<=2) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=2) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(x+1,y+1,w-2,h-2);
        }


    }
}
