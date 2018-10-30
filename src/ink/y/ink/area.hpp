//! \file
#ifndef Y_INK_AREA_INCLUDED
#define Y_INK_AREA_INCLUDED 1

#include "y/ink/types.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/cslot.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! area
        class Area : public Object
        {
        public:
            static const size_t DIMENSION = 2; //!< physical dimension

            const coord  lower;  //!< lower coordinate
            const coord  upper;  //!< upper coordinate
            const size_t w;      //!< width:  u.x-l.x+1 or 0 if empty
            const size_t h;      //!< height: u.y-l.y+1, or 0 if empty
            const size_t pixels; //!< w*h

            //! default area, origin
            inline Area() throw() : Object(), lower(0,0), upper(0,0), w(1), h(1), pixels(1) {}

            //! copy
            inline Area(const Area &other) throw() : Object(),
            lower(other.lower), upper(other.upper), w(other.w), h(other.h), pixels(other.pixels) {}

            //! destructor
            inline ~Area() throw() {}

            //! build by upper or by coordinates, never empty
            inline Area(const coord           l,
                        const coord           p) throw() :
            Object(),
            lower(l),
            upper(p),
            w(0),
            h(0),
            pixels(0)
            {
                if(lower.x>upper.x) cswap(lower.x,upper.x);
                if(lower.y>upper.y) cswap(lower.y,upper.y);
                (size_t&)w = 1+upper.x-lower.x;
                (size_t&)h = 1+upper.y-lower.y;
                (size_t&)pixels = w*h;
            }

            //! build by lower coordinate and size, may be empty
            inline Area(const coord           l,
                        const size_t          W,
                        const size_t          H) throw() :
            Object(),
            lower(l),
            upper(l),
            w(W),
            h(H),
            pixels(w*h)
            {
                coord &u = (coord&)upper;
                u.x += W;
                u.y += H;
                u.dec();
            }
            
            //! test if coordinate is insinde
            inline bool has( const coord c ) const throw()
            {
                return (c.x>=lower.x) && (c.y>=lower.y) && (c.x<=upper.x) && (c.y<=upper.y);
            }

            //! test if coordinate is inside
            inline bool has( const unit_t x, const unit_t y ) const throw()
            {
                return (x>=lower.x) && (y>=lower.y) && (x<=upper.x) && (y<=upper.y);
            }

            //! test if contains a sub-area
            inline bool contains(const Area &sub) const throw()
            {
                return has(sub.lower) && has(sub.upper);
            }


            //! output info
            inline friend std::ostream & operator<<( std::ostream &os, const Area &a )
            {
                os << '{' << a.lower << "->" << a.upper << " : " << a.w << "x" << a.h << " #" << a.pixels << '}';
                return os;
            }
            
        private:
            Y_DISABLE_ASSIGN(Area);

        };

        //! wrapper to extract value
#define Y_INK_AREA_LIMITS(A)     \
const unit_t xmin = (A).lower.x; \
const unit_t xmax = (A).upper.x; \
const unit_t ymin = (A).lower.y; \
const unit_t ymax = (A).upper.y
        

    }
}

#endif

