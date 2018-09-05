
#include "y/chem/equilibria.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/sort/sorted-sum.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
        double Equilibria:: computeExcess(array<double> &C)
        {
            beta2.free();
            for(size_t j=M;j>0;--j)
            {
                beta[j] = 0;
                const double Cj = C[j];
                if(active[j]&&Cj<0)
                {
                    const double Cj2 = Cj*Cj;
                    if(Cj2<=numeric<double>::minimum)
                    {
                        C[j] = 0;
                    }
                    else
                    {
                        beta[j] = -Cj;
                        beta2.push_back(Cj2);
                    }
                }
            }
            return sorted_sum(beta2);
        }


        bool Equilibria:: balance(array<double> &C)
        {
            // initialize
            for(size_t j=M;j>0;--j)
            {
                Cini[j] = C[j];
            }

            double Eini = computeExcess(Cini);
            while( Eini > 0 )
            {
                std::cerr << "beta  = " << beta << std::endl;
                std::cerr << "Eini  = " << Eini << std::endl;
                tao::mul(dC,Bal,beta);
                std::cerr << "dC    = " << dC << std::endl;
                break;
            }
            return false;

        }
    }

}

