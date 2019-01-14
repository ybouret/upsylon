#include "y/geometry/iso2d.hpp"

namespace upsylon
{
    namespace geometry
    {

        unsigned iso2d:: sign_flag( const double value ) throw()
        {
            if(value<0)      return sign_negative;
            else if(value>0) return sign_positive;
            else             return sign_zero;
        }

        iso2d::vertex   iso2d::zfind(const vertex &pa, const double va,
                                     const vertex &pb, const double vb) throw()
        {
            assert(va*vb<=0);
            double num = -va;
            double den = vb-va;
            if(den<0)
            {
                num = -num;
                den = -den;
            }
            const double lam = num/den;
            return vertex( pa.x + lam * (pb.x-pa.x), pa.y + lam * (pb.y-pa.y) );
        }

        //
        iso2d:: identifier:: identifier(const unit_t ii, const unit_t jj, const unsigned pp) throw() :
        i(ii), j(jj), p(pp) {}

        iso2d:: identifier:: ~identifier() throw()
        {
        }

        iso2d:: identifier:: identifier(const identifier &other) throw() :
        i(other.i), j(other.j), p(other.p)
        {}

        //
        iso2d:: identifier:: hasher::  hasher() throw() : H() {}
        iso2d:: identifier:: hasher:: ~hasher() throw() {}

        size_t iso2d:: identifier:: hasher:: operator()(const identifier &id) throw()
        {
            H.set();
            H.run_type(id.i);
            H.run_type(id.j);
            H.run_type(id.p);
            return H.key<size_t>();
        }

        //
        iso2d:: unique_point :: unique_point(const identifier &id,
                                             const vertex     &v) throw() :
        tag(id),
        vtx(v)
        {
        }

        iso2d:: unique_point :: ~unique_point() throw()
        {
        }

        const iso2d::identifier & iso2d::unique_point:: key() const throw() { return tag; }


        //
        iso2d:: segment:: segment(const shared_point &A, const shared_point &B) throw() :
        next(0),
        prev(0),
        a(A),
        b(B)
        {
        }

        iso2d:: segment:: ~segment() throw()
        {
        }

        //
        iso2d:: shared_points:: shared_points() throw(): segments() {}

        iso2d:: shared_points:: ~shared_points() throw() {}

    }

}



