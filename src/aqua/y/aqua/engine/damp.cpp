
#include "y/aqua/engine.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{

    namespace Aqua
    {

        using namespace mkl;

        bool Engine:: damp(addressable<double> &dC, const accessible<double> &C) throw()
        {
            computeJ(C);
            tao::mmul_trn(W,J,Nu);
            if(!LU::build(W))
            {
                return false;
            }
            else
            {
                tao::set(Cdmp,dC);
                tao::mul(xi,J,Cdmp);
                tao::add(xi,Q);
                LU::solve(W,xi);
                tao::mul_sub(Cdmp,tNu,xi);
                tao::upload(dC,Cdmp);
                return true;
            }
        }

    }

}
