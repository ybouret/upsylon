
#include "y/graphic/area.hpp"
#include "y/exception.hpp"

#include "y/type/bzset.hpp"

namespace upsylon {

    namespace Graphic {

        Area:: ~Area() throw()
        {
            _bzset(w);
            _bzset(h);
            _bzset(items);
        }

        Area:: Area( const size_t W, const size_t H) :
        w(W),
        h(H),
        items(w*h)
        {
            static const char fn[] = "Graphic::Area: ";

            if(w<0) throw exception("%swidth overflow",fn);
            if(h<0) throw exception("%sheight overflow",fn);
            if( (w>0&&h==0) || (h>0&&w==0) )
            {
                throw exception("%s%lux%lu is invalid", fn, long(w), long(h) );
            }
        }

        Area:: Area(const Area &a) throw() :
        Object(),
        w(a.w),
        h(a.h),
        items(a.items)
        {
        }

        std::ostream & operator<<( std::ostream &os, const Area &area )
        {
            os << "[" << area.w << "x" << area.h << "]";
            return os;
        }


        bool Area:: contains(const unit_t i, const unit_t j) const throw()
        {
            return (i>=0) && (j>=0) && (i<w) && (j<h);
        }


        bool Area:: contains(const Point &p) const throw()
        {
            return contains(p.x,p.y);
        }

        bool Area:: sameAreaThan( const Area &area ) const throw()
        {
            if( (w == area.w) && (h == area.h) )
            {
                assert(items==area.items);
                return true;
            }
            else
            {
                assert(items!=area.items);
                return false;
            }
        }


    }
}

