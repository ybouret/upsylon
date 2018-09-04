#include "y/chem/equilibria.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/fcn/zfind.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
        namespace
        {
            struct __single_solver
            {
                const Equilibrium   *pEq;
                double               K;
                const array<double> *pCini;
                const array<int>    *pNu;
                array<double>       *pCtry;
                
                double operator()(double xi)
                {
                    assert(pEq);
                    assert(K>0);
                    assert(pCini);
                    assert(pNu);
                    assert(pCtry);
                    array<double> &C = *pCtry;
                    tao::setprobe(C, *pCini, xi, *pNu );
                    return pEq->Gamma(K,C);
                }
            };
        }

        bool Equilibria::solveSingle(const size_t iEq, array<double> &C)
        {
            // initialize data and function to be zero
            for(size_t i=M;i>0;--i)
            {
                Cini[i] = C[i];
            }
            Equilibrium      &eq = *rxn[iEq];
            const array<int> &nu =  Nu[iEq];
            __single_solver F = { &eq, K[iEq], &Cini, &nu, &Ctry };

            std::cerr << "solve Single " << eq.name << std::endl;
            double F0 = F(0);
            std::cerr << "|_F0[" << iEq << "]=" << F0 << "/" << F.pEq->name << std::endl;

            if(F0>0)
            {
                std::cerr << "|_forward" << std::endl;
                return false;
            }
            else if(F0<0)
            {
                std::cerr << "|_reverse" << std::endl;
                return false;
            }
            else
            {
                // ok, already solved!
                return true;
            }
        }
    }
}
