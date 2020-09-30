
#include "y/aqua/solver.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;

        bool Solver:: forward(const Equilibria    &eqs,
                              addressable<double> &C) throw()
        {
            assert(C.size()>=M);

            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    Cfwd[j] = C[j];
                }
                else
                {
                    Cfwd[j] = 0;
                }
            }
            std::cerr << "Cini=" << Cfwd << std::endl;

            for(size_t cycle=0;cycle<10;++cycle)
            {
                computePhi(eqs,Cfwd);
                if(!computeW())
                {
                    return false;
                }

                computeQ(eqs,Cfwd);
                std::cerr << "Q=" << Q << std::endl;

                quark::neg(xi,Q);
                LU::solve(W,xi);
                //std::cerr << "xi=" << xi << std::endl;

                quark::mul_add(Cfwd,tNu,xi);
                std::cerr << "Cend=" << Cfwd << std::endl;
                if(!balance(Cfwd))
                {
                    return false;
                }
                std::cerr << "Cnew=" << Cfwd << std::endl;

            }



            return true;
        }

    }

}

