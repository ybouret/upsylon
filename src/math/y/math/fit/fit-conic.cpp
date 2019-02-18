
#include "y/math/fit/fit-conic.hpp"
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
            v(0),
            S(6,6),
            M(6,6),
            M0(6,6),
            prv(3,SIZE),
            wr( prv.next() ),
            wi( prv.next() ),
            q(  prv.next() )
            {

                {
                    C[1][1] = C[2][2] = C[3][3] = 1;
                    v=1;
                }
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
                size_t l=0;
                S.ld(0);
                for(size_t i=6;i>0;--i)
                {
                    for(size_t j=i;j>0;--j)
                    {
                        S[i][j] = S[j][i] = sums[++l];
                    }
                }
                std::cerr << "S=" << S << std::endl;
                if( !LU::build(S) )
                {
                    std::cerr << "Singular Distribution" << std::endl;
                    return false;
                }

                //______________________________________________________________
                //
                // compute M=inv(S)*C and save it into M0
                //
                //______________________________________________________________
                std::cerr << "C=" << C << std::endl;
                M.assign(C);


                LU::solve(S,M);
                std::cerr << "M=" << M << std::endl;

                M0.assign(M);

                //______________________________________________________________
                //
                // diagonalize the matrix
                //
                //______________________________________________________________
                size_t nr = 0;
                if(!diagonalize::eig(M,wr,wi,nr))
                {
                    std::cerr << "Couldn't Diagonalize M" << std::endl;
                    return false;
                }

                if(nr<=0)
                {
                    std::cerr << "No Real EigenValue" << std::endl;
                    return false;
                }

                std::cerr << "wr=" << wr << std::endl;
                std::cerr << "wi=" << wi << std::endl;



                matrix<double> ev(nr,6);
                diagonalize::eigv(ev,M0,wr);
                std::cerr << "ev=" << ev << std::endl;
                for(size_t i=1;i<=nr;++i)
                {
                    const double   mu = wr[i];
                    array<double> &U  = ev[i];
                    std::cerr << std::endl;
                    std::cerr << "mu=" << mu << ", U=" << U << std::endl;
                    tao::mul(wi,C,U);
                    const double UCU = tao::dot<double>(U,wi);
                    std::cerr << "UCU=" << UCU << std::endl;
                    if(UCU>0)
                    {
                        std::cerr << "\t#possible..." << std::endl;
                        tao::mulset(q,1.0/sqrt(UCU),U);
                        std::cerr << "\tq=" << q << std::endl;
                        tao::mul(wi,C,q);
                        std::cerr << "\t\tqCq=" << tao::dot<double>(q,wi) << std::endl;
                        tao::mul(wi,S,q);
                        std::cerr << "\t\tqSq=" << tao::dot<double>(q,wi) << std::endl;
                    }

                }
                exit(0);

#if 0
                const double inv_lam = wr[nr];
                if(inv_lam<=0)
                {
                    std::cerr << "Negative Highest Eigenvalue" << std::endl;
                }
                std::cerr << "lambda=1/" << inv_lam << std::endl;
                matrix<double> ev(nr,6);
                diagonalize::eigv(ev,M0,wr);
                array<double> &U = ev[nr];
                std::cerr << "U=" << U << std::endl;
                tao::mul(wi,C,U);
                const double UCU = tao::dot<double>(U,wi);
                std::cerr << "UCU=" << UCU << std::endl;
                if(UCU<=0)
                {
                    return false;
                }
                tao::mulset(q,1.0/sqrt(UCU),U);
                std::cerr << "conic=" << q << std::endl;
                return true;
#endif
            }

        }

    }
}

