

#include "y/graphic/rectangle.hpp"
#include "y/exception.hpp"

namespace upsylon {

    namespace Graphic {

        Rectangle:: ~Rectangle() throw()
        {
        }

        static inline unit_t chkdif( unit_t &lo, unit_t &hi) throw()
        {
            if(hi<lo) cswap(lo,hi);
            assert(lo<=hi);
            return hi-lo;
        }

        Rectangle:: Rectangle(  unit_t x0,   unit_t y0,
                                unit_t x1,   unit_t y1) throw() :
        Area(1+chkdif(x0,x1),1+chkdif(y0,y1)),
        xmin(x0),
        ymin(y0),
        xmax(x1),
        ymax(y1)
        {
            assert(x0<=x1); assert(xmin<=xmax);
            assert(y0<=y1); assert(ymin<=ymax);

        }

        Rectangle:: Rectangle(const Rectangle &other) throw() :
        Area(other),
        xmin(other.xmin),
        ymin(other.ymin),
        xmax(other.xmax),
        ymax(other.ymax)
        {

        }

        bool Area:: contains(const Rectangle &rect) const throw()
        {
            return contains(rect.xmin,rect.ymin) && contains(rect.xmax,rect.ymax);
        }

        void Rectangle:: displayRectangle() const
        {
            std::cerr << "[" << xmin << "," << ymin << "]->[" << xmax << "," << ymax << "]" << std::endl;
        }


    }

}

