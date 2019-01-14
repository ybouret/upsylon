#include "y/geometry/iso2d.hpp"

namespace upsylon
{
    namespace geometry
    {
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
        

    }

}



