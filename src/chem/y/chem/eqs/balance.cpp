
#include "y/chem/equilibria.hpp"
#include "y/math/kernel/tao.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
        bool Equilibria:: balance(array<double> &C)
        {
            // initialize
            size_t nxs = 0;
            for(size_t j=M;j>0;--j)
            {
                excess[j] = 0;
                const double Cj = Cini[j]  = C[j];
                if(active[j]&&Cj<0)
                {
                    ++nxs;
                    excess[j] = -Cj;
                }
            }

            while(nxs)
            {
                std::cerr << "xs=" << excess << std::endl;
                tao::mul(dC, Bal, excess);
                std::cerr << "dC=" << dC << std::endl;
                break;
            }

            return false;

        }
    }

}

