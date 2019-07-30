
#include "y/math/fit/in2d/conic.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/exception.hpp"
#include "y/math/kernel/diagonalize.hpp"

namespace upsylon
{
    namespace math
    {
        namespace Fit
        {
            Conic:: ~Conic() throw()
            {
            }

            Conic:: Conic() :
            Shape2D(21),
            sums(count),
            C(6,6),
            S(6,6),
            M(6,6),
            M0(6,6),
            prv(3,SIZE),
            wr( prv.next() ),
            wi( prv.next() ),
            q(  prv.next() )
            {
                C[1][1] = C[2][2] = C[3][3] = 1;
            }

            bool Conic:: __compute()
            {

                //______________________________________________________________
                //
                // prepare sums
                //
                //______________________________________________________________
                {
                    Arrays &self = *this;
                    double *s    = *sums;
                    for(size_t i=0;i<count;++i)
                    {
                        s[i] = sorted_sum(self[i]);
                    }
                }

                //______________________________________________________________
                //
                // dispatch into S matrix
                //
                //______________________________________________________________
                S.ld(0);
                {
                    size_t l=0;
                    for(size_t i=6;i>0;--i)
                    {
                        for(size_t j=i;j>0;--j)
                        {
                            S[i][j] = S[j][i] = sums[++l];
                        }
                    }
                }
                if( !LU::build(S) )
                {
                    return false;
                }

                //______________________________________________________________
                //
                // compute M=inv(S)*C and save it into M0
                //
                //______________________________________________________________
                M.assign(C);
                LU::solve(S,M);
                M0.assign(M);

                //______________________________________________________________
                //
                // diagonalize the matrix
                //
                //______________________________________________________________
                size_t nr = 0;
                if(!diagonalize::eig(M,wr,wi,nr))
                {
                    return false; // not possible
                }

                if(nr<=0)
                {
                    return false; // no real eigenvalue
                }

                //______________________________________________________________
                //
                // find the eigenvectors, values are ordered
                //
                //______________________________________________________________
                matrix<double> ev(nr,6);
                diagonalize::eigv(ev,M0,wr);

                for(size_t i=nr;i>0;--i)
                {
                    const double mu = wr[i];
                    if(mu<=0) continue;

                    array<double> &U  = ev[i];
                    tao::mul(wi,C,U);
                    const double UCU = tao::dot<double>(U,wi);
                    if(UCU<=0) continue;
                    tao::mulset(q,1.0/sqrt(UCU),U);
                    return true;
                }

                //______________________________________________________________
                //
                // no matching vector...
                //
                //______________________________________________________________
                return false;
            }

            

        }

    }
}

