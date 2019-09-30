//! \file
#ifndef Y_GEOM_ISO2D_SCANNER_INCLUDED
#define Y_GEOM_ISO2D_SCANNER_INCLUDED 1

#include "y/geometry/iso2d/levels.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            //! scanner for building LevelSet
            struct Scanner
            {

                const double     *d; //!< [0:4] values
                const Coordinate *c; //!< [0:4] values
                const Vertex     *v; //!< [0:4] values
                mutable Level_   *l; //!< target level

                //__________________________________________________________________
                //
                //! a low-level contour algorithm
                //__________________________________________________________________
                /**
                 data            ! matrix/field of data to contour
                 ilb,iub,jlb,jub ! index bounds of data matrix[j][i]
                 x               ! data matrix column coordinates
                 y               ! data matrix row coordinates
                 z               ! contour levels
                 */
                template<
                typename FIELD,
                typename ARRAY
                > static inline
                void Run(LevelSet              &ls,
                         const FIELD           &data,
                         const unit_t           ilb,
                         const unit_t           iub,
                         const unit_t           jlb,
                         const unit_t           jub,
                         const ARRAY           &x,
                         const ARRAY           &y,
                         const Contour::Levels &z )
                {

                    const size_t nc = z.size();
                    ls.create(nc);
                    if(nc<=0) return;

                    const double zmin  = z.head(), zmax  = z.tail(); assert(zmin<=zmax);
                    const unit_t jlbp1 = jlb+1,    ilbp1 = ilb+1;


                    for(unit_t j=jlb,jp1=jlbp1;j<jub;++j,++jp1)
                    {
                        const double y0 = double(y[j]);
                        const double y1 = double(y[jp1]);
                        const double ym = 0.5*(y0+y1);

                        for(unit_t i=ilb,ip1=ilbp1;i<iub;++i,++ip1)
                        {
                            //------------------------------------------------------
                            //
                            // outer loop: scan the square
                            //
                            //------------------------------------------------------


                            //------------------------------------------------------
                            // collect local values
                            //------------------------------------------------------
                            const double d1 = data[j  ][i];    double dmin=d1, dmax=d1;
                            const double d2 = data[j  ][ip1];  dmin = min_of(dmin,d2); dmax=max_of(dmax,d2);
                            const double d3 = data[jp1][ip1];  dmin = min_of(dmin,d3); dmax=max_of(dmax,d3);
                            const double d4 = data[jp1][i  ];  dmin = min_of(dmin,d4); dmax=max_of(dmax,d4);

                            if(dmax<zmin||dmin>zmax)
                            {
                                continue; // no possible intersection in this square
                            }

                            const double global_d[5] = { Contour::Average(d1,d2,d3,d4),d1,d2,d3,d4 };

                            const Coordinate c[5] =
                            {
                                Coordinate(i,  j,   Coordinate::Half),
                                Coordinate(i,  j,   Coordinate::Full),
                                Coordinate(ip1,j,   Coordinate::Full),
                                Coordinate(ip1,jp1, Coordinate::Full),
                                Coordinate(i,  jp1, Coordinate::Full)
                            };

                            const double x0   = double(x[i]);
                            const double x1   = double(x[ip1]);
                            const Vertex v[5] =
                            {
                                Vertex(0.5*(x0+x1),ym),
                                Vertex(x0,y0),
                                Vertex(x1,y0),
                                Vertex(x1,y1),
                                Vertex(x0,y1)
                            };

                            //------------------------------------------------------
                            //
                            // inner loop: over the levels
                            //
                            //------------------------------------------------------
                            LevelSet::iterator it=ls.begin();
                            for(size_t k=1;k<=nc;++k,++it)
                            {
                                assert((**it).index==k);
                                //--------------------------------------------------
                                // build the local field values to test against 0
                                //--------------------------------------------------
                                const double z_k = z[k];
                                if(z_k>dmax||z_k<dmin) continue; // no possible intersection
                                const double d[5] =
                                {
                                    global_d[0]-z_k,
                                    global_d[1]-z_k,
                                    global_d[2]-z_k,
                                    global_d[3]-z_k,
                                    global_d[4]-z_k,
                                };
                                Scanner ctx = { d,c,v,&(**it) };

                                //--------------------------------------------------
                                // scan the triangles
                                //--------------------------------------------------
                                ctx.scanTriangles();
                            }

                        }
                    }
#if !defined(NDEBUG)
                    ls.check();
#endif
                }

            private:
                void scanTriangles() const;
            };

        }

    }

}

#endif

