
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
                    std::cerr << "trying " << mu << std::endl;
                    if(mu<=0) continue;

                    array<double> &U  = ev[i];
                    tao::mul(wi,C,U);
                    const double UCU = tao::dot<double>(U,wi);
                    std::cerr << "\tUCU=" << UCU << std::endl;
                    if(UCU<=0) continue;
                    tao::mulset(q,1.0/sqrt(UCU),U);
                    std::cerr << "conic=" << q << std::endl;
                    return true;
                }
                
                return false;


            }

            const matrix<double> & Conic:: constraint() const throw()
            {
                return C;
            }


        }

    }
}

