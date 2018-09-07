
#include "y/chem/equilibria.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/math/kernel/lu.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
        bool Equilibria:: normalize(array<double> &C)
        {

            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = C[j];
            }

            if(!balance(Corg))
            {
                std::cerr << "[normalize] unable to balance initial concentrations" << std::endl;
                return false;
            }

            while(true)
            {
                // try to find a valid starting point
                computeGammaAndPhi(Corg);
                tao::_mmul_rtrn(W,Phi,Nu);
                if( !LU::build(W) )
                {
                    std::cerr << "second chance" << std::endl;
                    if(!tryShift(Corg))
                    {
                        std::cerr << "[normalize]  unable to find a starting point" << std::endl;
                        return false;
                    }

                    computeGammaAndPhi(Corg);
                    tao::_mmul_rtrn(W,Phi,Nu);
                    if( !LU::build(W) )
                    {
                        std::cerr << "[normalize] singular system" << std::endl;
                        return false;
                    }
                }

                // compute the new position
                tao::neg(xi,Gamma);
                LU::solve(W,xi);
                tao::mul(step,tNu,xi);
                tao::add(Cnew,Corg,step);
                //std::cerr << "Cnew=" << Cnew << std::endl;

                // balance the new position
                if(!balance(Cnew))
                {
                    std::cerr << "[normalize] cannot balance" << std::endl;
                    return false;
                }

                // test convergence
                bool converged = true;
                for(size_t j=M;j>0;--j)
                {
                    if(active[j])
                    {
                        const double d = fabs( Cnew[j] - Corg[j] );
                        const double c = numeric<double>::ftol * max_of<double>(fabs(Cnew[j]),fabs(Corg[j]));
                        if(d>c)
                        {
                            converged = false;
                        }
                        Corg[j] = Cnew[j];
                    }
                    else
                    {
                        assert( fabs(Cnew[j]-Corg[j])<=0 );
                    }
                }

                if(converged)
                {
                    //std::cerr << "converged" << std::endl;
                    break;
                }
            }

            for(size_t j=M;j>0;--j)
            {
                C[j] = Corg[j];
            }

            return true;
        }
    }

}

