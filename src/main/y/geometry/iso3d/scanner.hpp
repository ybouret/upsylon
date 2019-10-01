//! \file
#ifndef Y_GEOMETRY_ISO3D_SCANNER_INCLUDED
#define Y_GEOMETRY_ISO3D_SCANNER_INCLUDED 1

#include "y/geometry/iso3d/facets.hpp"
#include "y/geometry/contour.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso3D {

            //! algorithm to build 3D isosurface
            struct Scanner
            {
                const Vertex      *v; //!< physical vertex field
                const Coordinate  *c; //!< logical coordinate field
                const double      *d; //!< local data field

                //! run algorithm to deduce
                template <
                typename FIELD,
                typename ARRAY>
                void Run(const FIELD &data,
                         const unit_t ilo,
                         const unit_t ihi,
                         const unit_t jlo,
                         const unit_t jhi,
                         const unit_t klo,
                         const unit_t khi,
                         const ARRAY &x,
                         const ARRAY &y,
                         const ARRAY &z,
                         const Contour::Levels &w
                         )
                {

                    const size_t nc = w.size();
                    if(nc<=0) return;
                    const double wmin  = w.head();
                    const double wmax  = w.tail();
                    const unit_t klop1 = klo+1;
                    const unit_t jlop1 = jlo+1;
                    const unit_t ilop1 = ilo+1;
                    for(unit_t k=klo,kp1=klop1;k<khi;++k,++kp1)
                    {
                        const double z0 = double(z[k]   );
                        const double z1 = double(z[kp1] );

                        for(unit_t j=jlo,jp1=jlop1;j<jhi;++j,++jp1)
                        {
                            const double y0 = double(y[j]);
                            const double y1 = double(y[jp1]);

                            for(unit_t i=ilo,ip1=ilop1;i<ihi;++i,++ip1)
                            {
                                const double x0 = double(x[i]);
                                const double x1 = double(x[ip1]);

                                const Vertex v[8]=
                                {
                                    Vertex(x0,y0,z0),  Vertex(x0,y1,z0),  Vertex(x1,y1,z0),  Vertex(x1,y0,z0),
                                    Vertex(x0,y0,z1),  Vertex(x0,y1,z1),  Vertex(x1,y1,z1),  Vertex(x1,y0,z1),
                                };

                                const Coordinate c[8] =
                                {
                                    Coordinate(i,j,k),   Coordinate(i,jp1,k),   Coordinate(ip1,jp1,k),   Coordinate(ip1,j,k),
                                    Coordinate(i,j,kp1), Coordinate(i,jp1,kp1), Coordinate(ip1,jp1,kp1), Coordinate(ip1,j,kp1)
                                };

                                const double g[8] =
                                {
                                    data[k][j][i],   data[k][jp1][i],   data[k][jp1][ip1],   data[k][j][ip1],
                                    data[kp1][j][i], data[kp1][jp1][i], data[kp1][jp1][ip1], data[kp1][j][ip1],
                                };
                                double gmin = g[0], gmax=g[0];
                                for(size_t u=1;u<7;++u)
                                {
                                    gmin = min_of(gmin,g[u]);
                                    gmax = max_of(gmax,g[i]);
                                }
                                if(gmin>wmax||gmax<wmin)
                                {
                                    continue; // no possible facet in this cube
                                }

                                for(size_t l=1;l<=nc;++l)
                                {
                                    const double iso = w[l];
                                    if(iso<gmin||iso>gmax)
                                    {
                                        continue; // no possible facet for this level
                                    }
                                    const double d[8] = { g[0]-iso, g[1]-iso, g[2]-iso, g[3]-iso, g[4]-iso, g[5]-iso, g[6]-iso, g[7]-iso };

                                    const Scanner ctx = { v,c,d };
                                    ctx.scanCube();
                                }


                            }

                        }

                    }

                }

            private:
                void scanCube() const;

            };

        }

    }
}

#endif

