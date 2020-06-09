

#include "y/chem/equilibria.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    namespace Chemical
    {
        using namespace math;

        bool Equilibria:: damp(array<double> &delta, const array<double> &C)
        {
            assert(delta.size()>=M);
            assert(C.size()>=M);

            computeGammaAndPhi(C);
            quark::mmul(W,Phi,tNu);
            if(!LU::build(W)) return false; //!< singular composition
            
            for(size_t i=N;i>0;--i)
            {
                xi[i] = -( Gamma[i] + quark::dot<double>::of(Phi[i],delta) );
            }
            LU::solve(W,xi);
            quark::mul_add(delta, tNu, xi);
            return true;
        }

    }

}
