
//! \file
#ifndef Y_MKL_SIGNAL_SMOOTH_INCLUDED
#define Y_MKL_SIGNAL_SMOOTH_INCLUDED 1

#include "y/mkl/signal/extend.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/exception.hpp"
#include "y/ptr/arc.hpp"
#include "y/associative/map.hpp"
#include "y/core/ipower.hpp"

namespace upsylon {
    
    namespace mkl {

        //! filtering by local Savistky-Golay
        template <typename T>
        class smooth
        {
        public:

            typedef matrix<T>              matrix_type; //!< alias
            typedef arc_ptr<matrix_type>   matrix_ptr;  //!< alias
            typedef map<size_t,matrix_ptr> matrix_map;  //!< alias
            typedef point2d<T>             point_type;  //!< alias
            typedef list<point_type>       point_list;  //!< alias

            point_list       points;   //!< collected points
            const size_t     order;    //!< fit order
            const size_t     coefs;    //!< order+1
            matrix_map       matrices; //!< store data
            const point_type origin;   //!< (0,0)

            //! setup
            explicit smooth(const size_t deg) :
            points(),
            order(deg),
            coefs(order+1),
            matrices(),
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
                matrix_type    &mu = get_matrix(nc);
                array<double>  &a  = mu.r_aux2;
                mu.ld(0);
                a.ld(0);

                size_t ii=np;
                for( typename point_list::iterator it=points.begin();ii>0;--ii,++it)
                {
                    const point_type p = *it;
                    for(size_t j=1,jm=0;j<=nc;++j,++jm)
                    {
                        const T xpjm = ipower<double>(p.x,jm);
                        a[j] += p.y * xpjm;
                        for(size_t k=j,km=jm;k<=nc;++k,++km)
                        {
                            const T xpkm = ipower<double>(p.x,km);
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
            matrix_type & get_matrix(const size_t n)
            {
                assert(n>0);
                matrix_ptr *ppM = matrices.search(n);
                if(!ppM)
                {
                    //std::cerr << "-- new matrix #" << n << std::endl;
                    matrix_ptr pM = new matrix_type(n,n);
                    if(!matrices.insert(n,pM))
                    {
                        throw exception("smooth: unexpected matrix insert failure for #%u",unsigned(n));
                    }
                    return *pM;
                }
                else
                {
                    return **ppM;
                }
            }
        };

    }
}

#endif

