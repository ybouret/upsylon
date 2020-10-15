
#include "y/aqua/engine.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{

    namespace Aqua
    {

        using namespace mkl;

        bool Engine:: dampWith(const accessible<double> &C) throw()
        {
            computeJ(C);
            tao::mmul_trn(W,J,Nu);
            if(!LU::build(W))
            {
                return false;
            }
            else
            {
                tao::mul(xi,J,Cdmp);
                tao::add(xi,Q);
                LU::solve(W,xi);
                tao::mul_sub(Cdmp,tNu,xi);
                return true;
            }
        }

        bool Engine:: damp(addressable<double> &dC, const accessible<double> &C) throw()
        {
            tao::set(Cdmp,dC);
            if( dampWith(C) )
            {
                tao::upload(dC,Cdmp);
                return true;
            }
            else
            {
                return false;
            }
        }

        bool Engine:: feed(addressable<double> &C, const accessible<double> &dC) throw()
        {
            tao::set(Cdmp,dC);
            if( dampWith(C) )
            {
                // update local C
                tao::add(Cdmp,C);
                if(!balance(Cdmp))
                {
                    return false;
                }
                else
                {
                    if(!forward(Cdmp))
                    {
                        return false;
                    }
                    else
                    {
                        tao::upload(C,Cdmp);
                        return true;
                    }
                }
            }
            else
            {
                return false;
            }
        }

    }

}
