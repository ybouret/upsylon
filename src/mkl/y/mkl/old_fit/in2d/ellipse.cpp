#include "y/math/fit/in2d/ellipse.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/kernel/diag-symm.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            Ellipse:: ~Ellipse() throw()
            {

            }

            Ellipse:: Ellipse() : Conic(),
            center(),
            radii(),
            Q(2,2),
            R(2,2),
            lam(2,0.0)
            {
                C.ld(0);
                C[2][2] = -1;
                C[1][3] = C[3][1] = 2;
            }

            void Ellipse:: reset() throw()
            {
                bzset(center);
                bzset(radii);
                Q.Id();
                R.Id();
                lam.ld(0);
            }

            void Ellipse:: factorize()
            {

                reset();
                try
                {
                    //----------------------------------------------------------
                    //
                    // build the symmetric Quadratic matrix
                    //
                    //----------------------------------------------------------
                    Q[1][1] = q[1];
                    Q[2][2] = q[3];
                    Q[1][2] = Q[2][1] = q[2]/2;

                    //----------------------------------------------------------
                    //
                    // diagonalize
                    //
                    //----------------------------------------------------------
                    if(!diag_symm::build(Q,lam,R))
                    {
                        throw exception("Ellipse: singular quadratic matrix");
                    }

                    //----------------------------------------------------------
                    //
                    // check consistency
                    //
                    //----------------------------------------------------------
                    if(lam[1]*lam[2]<=0)
                    {
                        throw exception("Ellipse: invalid possible radii");
                    }

                    //----------------------------------------------------------
                    //
                    // check sign
                    //
                    //----------------------------------------------------------
                    if(lam[1]<0)
                    {
                        assert(lam[2]<0);
                        for(size_t i=6;i>0;--i) q[i] = -q[i];
                        Q.neg();
                        lam[1] = -lam[1];
                        lam[2] = -lam[2];
                    }

                    //----------------------------------------------------------
                    //
                    // lam[1] <= lam[2]
                    //
                    //----------------------------------------------------------
                    if(lam[1]>lam[2])
                    {
                        cswap(lam[1],lam[2]);
                        R.swap_cols(1,2);
                    }

                    //----------------------------------------------------------
                    //
                    // compute the center of the ellipse
                    //
                    //----------------------------------------------------------
                    vector<double> L(2,0);
                    L[1] = q[4];
                    L[2] = q[5];
                    if(!LU::build(Q))
                    {
                        throw exception("Ellipse: unexpected inversion failure");
                    }
                    vector<double> J(2,0);
                    J[1] = -0.5 * L[1];
                    J[2] = -0.5 * L[2];
                    LU::solve(Q,J);
                    center.x = J[1];
                    center.y = J[2];


                    //----------------------------------------------------------
                    //
                    // compute rho^2
                    //
                    //----------------------------------------------------------
                    const double f    = q[6];
                    const double rho2 = max_of<double>(-0.5 * tao::dot<double>(J,L) - f,0);
                    radii.x = sqrt(rho2/lam[1]);
                    radii.y = sqrt(rho2/lam[2]);
                }
                catch(...)
                {
                    reset();
                    throw;
                }
            }

            const matrix<double> & Ellipse:: rotation() const throw()
            {
                return R;
            }

        }
    }
}
