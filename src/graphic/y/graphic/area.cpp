
#include "y/graphic/area.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        Area:: ~Area() throw()
        {
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
        w(a.w),
        h(a.h),
        items(a.items)
        {
        }

        bool Area:: has(const unit_t i, const unit_t j) const throw()
        {
            return (i>=0) && (j>=0) && (i<w) && (j<h);
        }

    }
}

