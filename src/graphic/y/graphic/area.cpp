
#include "y/graphic/area.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        Area:: ~Area() throw()
        {
        }

        Area:: Area( const size_t W, const size_t H) :
        w(W),
        wm(w-1),
        h(H),
        hm(h-1),
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
        w(a.w), wm(a.wm),
        h(a.h), hm(a.hm),
        items(a.items)
        {
        }

        void Area:: displayArea() const
        {
            std::cerr << "[" << w << "x" << h << "]" << std::endl;
        }


        bool Area:: contains(const unit_t i, const unit_t j) const throw()
        {
            return (i>=0) && (j>=0) && (i<w) && (j<h);
        }

    }
}

