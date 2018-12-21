#include "y/geometry/contour2d.hpp"

namespace upsylon
{
    namespace geometry
    {
        // fonctions
        unsigned contour2d:: sign_flag( const double value ) throw()
        {
            if(value<0)      return sign_negative;
            else if(value>0) return sign_positive;
            else             return sign_zero;
        }

        // identifier
        contour2d:: identifier:: identifier( const coordinate &c, const offset &d ) throw() :
        coord(c), delta(d)
        {

        }

        contour2d:: identifier:: ~identifier() throw()
        {
        }

        contour2d:: identifier:: identifier( const identifier &other) throw() :
        coord(other.coord), delta(other.delta)
        {
        }

        bool operator==( const contour2d::identifier &lhs, const contour2d::identifier &rhs) throw()
        {
            return (lhs.coord==rhs.coord) && (lhs.delta==rhs.delta);
        }

        // identifier::hasher
        contour2d:: identifier:: hasher:: hasher() throw() : h()
        {
        }

        contour2d:: identifier:: hasher:: hasher :: ~hasher() throw()
        {
        }

        size_t contour2d::identifier:: hasher:: operator()( const identifier &id ) throw()
        {
            h.set();
            h.run(&id.coord,sizeof(coordinate));
            h.run(&id.delta,sizeof(offset));
            return h.key<size_t>();
        }

        // unique point
        contour2d:: unique_point:: unique_point( const identifier &i, const point &p ) throw() :
        uid(i), pos(p)
        {
        }

        contour2d:: unique_point:: ~unique_point() throw()
        {
        }

        const contour2d:: identifier & contour2d:: unique_point:: key() const throw()
        { return uid; }

        // segment
        contour2d:: segment:: segment( const shared_point &A, const shared_point &B ) throw() :
        next(0), prev(0), a(A), b(B)
        {
        }

        contour2d:: segment:: ~segment() throw()
        {}

        // segments
        contour2d:: segments:: segments(const size_t id) throw() :
        segment::list_type(),
        indx(id)
        {}

        contour2d:: segments:: ~segments() throw() {}

        const size_t & contour2d:: segments:: key() const throw() { return indx; }

        
    }
}

