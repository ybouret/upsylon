
//! \file
#ifndef Y_MATH_SIGNAL_SMOOTH_INCLUDED
#define Y_MATH_SIGNAL_SMOOTH_INCLUDED 1

#include "y/math/signal/extend.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {

        //! filtering by local Savistky-Golay
        template <typename T>
        class smooth
        {
        public:

            typedef point2d<T>       point_type; //!< alias
            typedef list<point_type> point_list; //!< alias
            point_list       points; //!< collected points
            const size_t     order;  //!< fit order
            const size_t     coefs;  //!< order+1
            const point_type origin; //!< (0,0)

            //! setup
            explicit smooth(const size_t deg) :
            points(),
            order(deg),
            coefs(order+1),
            origin(0,0)
            {
            }

            //! desrtuctor
            virtual ~smooth() throw()
            {
            }

            //! compute the filtered value, with optional derivative
            inline T operator()(const unit_t          i,
                                const extend<double> &data,
                                const T               dxLo,
                                const T               dxUp,
                                T                    *drvs=0 )
            {

                const unit_t nmin = (drvs!=0) ? 1 : 0;
                points.free();
                points << origin; //!< p0-p0
                const point_type p0 = data(i);

                // collect points before current value
                {
                    unit_t  j  = i-1;
                    unit_t  n  = 0;
                    const T dx = fabs(dxLo);
                    do
                    {
                        const point_type p  = data(j);
                        const point_type dp = p-p0;
                        if( n<nmin || fabs(dp.x)<=dx )
                        {
                            points << dp;
                            ++n;
                            --j;
                        }
                        else
                            break;
                    }
                    while(true);
                }

                // collect points after current value
                {
                    unit_t  j  = i+1;
                    unit_t  n  = 0;
                    const T dx = fabs(dxUp);

                    do
                    {
                        const point_type p  = data(j);
                        const point_type dp = p-p0;
                        if(n<nmin|| fabs(dp.x)<=dx )
                        {
                            points << dp;
                            ++n;
                            ++j;
                        }
                        else
                            break;
                    }
                    while(true);
                }

                // check what we have
                const size_t np       = points.size();
                const size_t nc       = min_of(coefs,np);
                const bool   has_drvs = (nc>1);

                //! polynomial fit
                matrix<double> mu(nc,nc);
                vector<double> a(nc);

                size_t ii=np;
                for( typename point_list::iterator it=points.begin();ii>0;--ii,++it)
                {
                    const point_type p = *it;
                    for(size_t j=1,jm=0;j<=nc;++j,++jm)
                    {
                        const T xpjm = ipower(p.x,jm);
                        a[j] += p.y * xpjm;
                        for(size_t k=j,km=jm;k<=nc;++k,++km)
                        {
                            const T xpkm = ipower(p.x,km);
                            mu[j][k] += xpjm*xpkm;
                        }
                    }
                }
                for(size_t j=1;j<nc;++j)
                {
                    for(size_t k=j+1;k<=nc;++k)
                    {
                        mu[k][j] = mu[j][k];
                    }
                }
                if(! LU::build(mu) )
                {
                    throw exception("smooth: unexpected singular moments");
                }

                LU::solve(mu,a);
                if(has_drvs&&drvs)
                {
                    *drvs = a[2];
                }
                return p0.y + a[1];
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(smooth);
        };

    }
}

#endif

