#include "y/geometry/iso3d/surfaces.hpp"
#include "y/type/block/zset.hpp"
#include "y/exception.hpp"


namespace upsylon {

    namespace geometry {

        namespace Iso3D {


            static const char Fn[] = "Iso3D::Surface: ";

            Surface_:: ~Surface_() throw()
            {
                _bzset(index);
            }

            Surface_:: Surface_(const size_t k) throw() :
            counted(),
            Points(),
            index(k)
            {
            }

            const size_t & Surface_ :: key() const throw()
            {
                return index;
            }

            Point_ * Surface_:: single(const Coordinate &c, const Vertex &v )
            {
                const Edge edge(c);
                Point *pp = search(edge);
                if(pp)
                {
                    return &(**pp);
                }
                else
                {
                    Point_ *p = new Point_(edge,v);
                    const Point P = p;
                    if( !insert(P) )
                    {
                        throw exception("%sunexpected single insert failure",Fn);
                    }
                    return p;
                }
            }


            Point_ * Surface_:: couple(const Coordinate &ca,
                                       const Vertex     &va,
                                       const double      da,
                                       const Coordinate &cb,
                                       const Vertex     &vb,
                                       const double      db)
            {
                assert(ca!=cb);
                //assert(da*db<0);
                const Edge e(ca,cb);
                Point *pp = search(e);
                if(pp)
                {
                    return &(**pp);
                }
                else
                {
                    double num = da;
                    double den = da-db;
                    if(den<0)
                    {
                        den=-den;
                        num=-num;
                    }
                    const double lam = (num<=0) ? 0 : ( (num>=den) ? 1 : num/den);
                    const Vertex vz  = va+lam*(vb-va);
                    Point_ *p = new Point_(e,vz);
                    {
                        const Point tmp = p;
                        if(!insert(tmp)) throw exception("%sunexpected couple insert failure",Fn);
                    }
                    return p;
                }
            }


        }

    }
}


namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            Surfaces:: ~Surfaces() throw()
            {
            }

            Surfaces:: Surfaces() throw() : SurfacesType()
            {
            }

            void Surfaces:: create(size_t n)
            {
                free();
                ensure(n);
                for(size_t i=1;i<=n;++i)
                {
                    const Surface s = new Surface_(i);
                    if(!insert(s))
                    {
                        throw exception("unexpected Iso3D::Surface::insert(#%u)", unsigned(i));
                    }
                }

            }

            
        }

    }
}

