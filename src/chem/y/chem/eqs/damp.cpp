

#include "y/chem/equilibria.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    namespace Chemical
    {
        using namespace mkl;

        bool Equilibria:: damp(array<double> &delta, const array<double> &C)
        {
            assert(delta.size()>=M);
            assert(C.size()>=M);

            computeGammaAndPhi(C);
            tao::mmul(W,Phi,tNu);
            if(!LU::build(W)) return false; //!< singular composition
            
            for(size_t i=N;i>0;--i)
            {
                xi[i] = -( Gamma[i] + tao::dot<double>::of(Phi[i],delta) );
            }
            LU::solve(W,xi);
            tao::mul_add(delta, tNu, xi);
            return true;
        }

    }

}
