#include "y/math/fit/shapes2d.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {

        shape2d:: ~shape2d() throw()
        {
        }

        shape2d:: shape2d( const size_t num_arrays ) :
        shape2d_arrays(num_arrays)
        {
        }

    }

}

namespace upsylon
{
    namespace math
    {
        fit_circle:: ~fit_circle() throw()
        {
        }

        fit_circle:: fit_circle() :
        shape2d(8),
        mu(3,3),
        rhs(3),
        x( next()  ),
        y( next()  ),
        x2( next() ),
        y2( next() ),
        xy( next() ),
        z(  next() ),
        zx( next() ),
        zy( next() ),
        xc(0),
        yc(0),
        radius(0)
        {
        }

        bool fit_circle:: __compute()
        {
            mu[1][1] = sorted_sum(x2);
            mu[2][2] = sorted_sum(y2);
            mu[3][3] = size();

            mu[1][2] = mu[2][1] = sorted_sum(xy);
            mu[1][3] = mu[3][1] = sorted_sum(x);
            mu[2][3] = mu[3][2] = sorted_sum(y);

            if(!LU::build(mu))
            {
                xc=yc=radius=0;
                return false;
            }

            rhs[1] = sorted_sum(zx);
            rhs[2] = sorted_sum(zy);
            rhs[3] = sorted_sum(z);
            LU::solve(mu,rhs);
            const double a = rhs[1];
            const double b = rhs[2];
            const double c = rhs[3];
            xc     = 0.5*a;
            yc     = 0.5*b;
            radius = sqrt(c + xc*xc + yc*yc);
            return true;
        }

    }
}

