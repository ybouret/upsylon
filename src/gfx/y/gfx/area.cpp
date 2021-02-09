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
            if( (w>0&&h<=0) || (h>0&&w<=0) )
            {
                throw exception("%s(invalid %s=%ld and %s=%ld)",afn, Check::Width, long(w), Check::Height, long(h) );
            }
        }

        Area:: Area(const Area &a) throw() :
        x(a.x),
        y(a.y),
        w(a.w),
        h(a.h),
        xm(a.xm),
        ym(a.ym),
        n(w*h)
        {
        }
        
        std::ostream & operator<<(std::ostream &os, const Area &a)
        {
            const Point ini(a.x,a.y);
            const Point end(a.xm,a.ym);
            const Point len(a.w,a.h);
            os << '(' << ini << '-' << '>' << end << ':' << len << '=' << '#' <<  a.n << ')';
            return os;
        }


        Area Area:: getCore() const
        {
            static const char fn[] = "Area::getCore";
            if(w<=2) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=2) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(x+1,y+1,w-2,h-2);
        }

        Area Area:: getLeft() const
        {
            static const char fn[] = "Area::getLeft";
            if(w<=0) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=2) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(x,y+1,1,h-2);
        }

        Area Area:: getRight() const
        {
            static const char fn[] = "Area::getLeft";
            if(w<=0) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=2) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(xm,y+1,1,h-2);
        }

        Area Area:: getBottom() const
        {
            static const char fn[] = "Area::getBottom";
            if(w<=2) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=0) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(x+1,y,w-2,1);
        }
        
        Area Area:: getTop() const
        {
            static const char fn[] = "Area::getTop";
            if(w<=2) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=0) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(x+1,ym,w-2,1);
        }

        Area Area:: getBottomLeft() const
        {
            static const char fn[] = "Area::getBottomLeft";
            if(w<=0) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=0) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(x,y,1,1);
        }

        Area Area:: getBottomRight() const
        {
            static const char fn[] = "Area::getBottomRight";
            if(w<=0) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=0) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(xm,y,1,1);
        }

        Area Area:: getTopLeft() const
        {
            static const char fn[] = "Area::getTopLeft";
            if(w<=0) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=0) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(x,ym,1,1);
        }

        Area Area:: getTopRight() const
        {
            static const char fn[] = "Area::getTopRight";
            if(w<=0) throw exception("%s(%s=%ld)",fn, Check::Width,  long(w));
            if(h<=0) throw exception("%s(%s=%ld)",fn, Check::Height, long(h));
            return Area(xm,ym,1,1);
        }

        Area Area:: get(const unsigned flags) const
        {
            static const char fn[] = "Area:get";
            switch(flags)
            {
                case Position::Core   : return getCore();
                case Position::Left   : return getLeft();
                case Position::Right  : return getRight();
                case Position::Top    : return getTop();
                case Position::Bottom : return getBottom();

                case Position::Top | Position::Left:  return getTopLeft();
                case Position::Top | Position::Right: return getTopRight();

                case Position::Bottom | Position::Left:  return getBottomLeft();
                case Position::Bottom | Position::Right: return getBottomRight();

                default:
                    break;
            }
            throw exception("%s(invalid flags=%u)",fn,flags);
        }

        bool Area:: owns(const unit_t X, const unit_t Y) const throw()
        {
            return (X>=x) && (X<=xm) && (Y>=y) && (Y<=ym);
        }

        bool Area:: owns(const Point p) const throw()
        {
            return owns(p.x,p.y);
        }

        bool Area:: owns(const Area &sub) const throw()
        {
            return (sub.x>=x) && (sub.xm<=xm) && (sub.y>=y) && (sub.ym<=ym);
        }

        bool Area:: hasCore() const throw()
        {
            return w>2 && h>2;
        }


    }
}
