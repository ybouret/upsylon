//! \file
#ifndef Y_GEOMETRY_CONTOUR2D_INCLUDED
#define Y_GEOMETRY_CONTOUR2D_INCLUDED 1

#include "y/type/point2d.hpp"
#include "y/type/ints.hpp"

namespace upsylon
{
    namespace geometry
    {

        struct contour2d
        {

            typedef point2d<double> point; //!< alias
            typedef uint32_t        res_t; //!< resolution
            static  const res_t     res_max = limit_of<res_t>::maximum;
            typedef point2d<res_t>  vertex;



            //! a low-level contour algorithm
            /**
             d               ! matrix/field of data to contour
             ilb,iub,jlb,jub ! index bounds of data matrix
             x               ! data matrix column coordinates
             y               ! data matrix row coordinates
             z               ! contour levels in increasing order, nc=z.size()
             */
            template<
            typename FIELD,
            typename ARRAY
            > static inline
            void scan(const FIELD         &d,
                      const unit_t         ilb,
                      const unit_t         iub,
                      const unit_t         jlb,
                      const unit_t         jub,
                      const ARRAY         &x,
                      const ARRAY         &y,
                      const array<double> &z
                      )
            {

                static const vertex v0(res_max>>1,res_max>>1);
                static const vertex v1(0,0);
                static const vertex v2(0,res_max);
                static const vertex v3(res_max,res_max);
                static const vertex v4(res_max,0);
                double              f[5] = {0,0,0,0,0};
                double              g[5] = {0,0,0,0,0};

                // initialize
                const size_t nc  = z.size();
                if(nc<=0)
                {
                    return;
                }
                const double zlo = z[1];
                const double zhi = z[nc];
#if !defined(NDEBUG)
                for(size_t i=1;i<nc;++i)
                {
                    assert(z[i]<=z[i+1]);
                }
#endif
                for(unit_t j0=jlb,j1=jlb+1;j0<jub;++j0,++j1)
                {
                    const double y0=y[j0];
                    const double y1=y[j1];
                    for(unit_t i0=ilb,i1=i0+1;i0<iub;++i0,++i1)
                    {
                        // load global field
                        double dmin = (f[1]=d[j0][i0]),      dmax=dmin;
                        dmin =  min_of(f[2]=d[j1][i0],dmin); dmax=max_of(dmax,f[2]);
                        dmin =  min_of(f[3]=d[j1][i1],dmin); dmax=max_of(dmax,f[3]);
                        dmin =  min_of(f[4]=d[j0][i1],dmin); dmax=max_of(dmax,f[4]);

                        // get rid of trivial case
                        if(dmin<zlo||dmax>zhi)
                        {
                            continue; // no possible intersection
                        }
                        const double x0 = x[i0];
                        const double x1 = x[i1];

                        
                        // loop over levels
                        for(size_t k=nc;k>0;--k)
                        {
                            // build the local field
                            const double zk = z[k];
                            g[1] = f[1]-zk;
                            g[2] = f[2]-zk;
                            g[3] = f[3]-zk;
                            g[4] = f[4]-zk;
                            g[0] = 0.25*(g[1]+g[2]+g[3]+g[4]);
                        }
                    }
                }
            }

        };

    }
}

#endif

