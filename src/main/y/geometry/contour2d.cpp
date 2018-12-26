#include "y/geometry/contour2d.hpp"
#include "y/exception.hpp"

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

        contour2d::point   contour2d::zfind(const point &pa, const double va,
                                            const point &pb, const double vb) throw()
        {
            assert(va*vb<=0);
            const double num = -va;
            const double den = vb-va;
            const double lam = num/den;
            return point( pa.x + lam * (pb.x-pa.x), pa.y + lam * (pb.y-pa.y) );
        }

        ////////////////////////////////////////////////////////////////////////

        contour2d:: segment:: segment(const point &_, const point &__) throw() :
        next(0), prev(0), a(_), b(__)
        {}

        contour2d:: segment:: ~segment() throw() {}

        ////////////////////////////////////////////////////////////////////////
        contour2d:: level_set:: level_set(const size_t idx) throw() : index(idx) {}
        contour2d:: level_set:: ~level_set() throw() {}
        const size_t & contour2d:: level_set:: key() const throw() { return index; }

        ////////////////////////////////////////////////////////////////////////
        contour2d:: level_sets:: level_sets(const size_t n) :
        level_set::database(n,as_capacity) {}

        contour2d:: level_sets:: ~level_sets() throw() {}


        void contour2d:: level_sets:: call(double x0,
                                           double y0,
                                           double x1,
                                           double y1,
                                           size_t idx,
                                           double,
                                           void *args)
        {
            assert(args);
            level_sets &self = *static_cast<level_sets*>(args);
            level_set  &ls   = self[idx];
            (void)ls;
            const point a(x0,y0);
            const point b(x1,y1);
            ls.push_back( new segment(a,b) );
        }


        contour2d:: level_set &  contour2d:: level_sets:: operator[](const size_t idx)
        {
            level_set::pointer *ppls = search(idx);
            if(ppls)
            {
                return **ppls;
            }
            else
            {
                level_set *pls = new level_set(idx);
                const level_set::pointer q = pls;
                if(!insert(q))
                {
                    throw exception("unexpected muliple level-set#%u",unsigned(idx));
                }
                return *pls;
            }
        }

        const contour2d::level_set & contour2d::level_sets:: operator[](const size_t idx) const
        {
            const level_set::pointer *ppls = search(idx);
            if(!ppls)
            {
                throw exception("no level-set#%u",unsigned(idx));

            }
            return **ppls;
        }


    }
}

