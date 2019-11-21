

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
        lower(x0,y0),
        upper(x1,y1)
        {
            assert(x0<=x1);
            assert(y0<=y1);
        }

        Rectangle:: Rectangle(Point p0, Point p1) throw() :
        Area(1+chkdif(p0.x,p1.x),1+chkdif(p0.y,p1.y)),
        lower(p0),
        upper(p1)
        {
            assert(p0.x<=p1.x);
            assert(p0.y<=p1.y);
        }




        Rectangle:: Rectangle(const Rectangle &other) throw() :
        Area(other),
        lower(other.lower),
        upper(other.upper)
        {

        }
        

        void Rectangle:: displayRectangle() const
        {
            std::cerr << lower << "->" << upper << std::endl;
        }

        bool Area:: contains(const Rectangle &r) const throw()
        {
            return contains(r.lower) && contains(r.upper);
        }

    }

}

