
#include "y/chem/equilibria.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;

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

            //__________________________________________________________________
            //
            // balance concentrations
            //__________________________________________________________________
            if(!balance(Corg))
            {
                std::cerr << "[normalize] unable to balance initial concentrations" << std::endl;
                return false;
            }

            while(true)
            {
                //--------------------------------------------------------------
                //
                // try to find a valid starting point
                //
                //--------------------------------------------------------------
                computeGammaAndPhi(Corg);
                quark::mmul_rtrn(W,Phi,Nu);
                if( !LU::build(W) )
                {
                    std::cerr << "second chance" << std::endl;
                    if(!tryShift(Corg))
                    {
                        std::cerr << "[normalize]  unable to find a starting point" << std::endl;
                        return false;
                    }

                    computeGammaAndPhi(Corg);
                    quark::mmul_rtrn(W,Phi,Nu);
                    if( !LU::build(W) )
                    {
                        std::cerr << "[normalize] singular system" << std::endl;
                        return false;
                    }
                }

                //--------------------------------------------------------------
                //
                // compute the new position
                //
                //--------------------------------------------------------------
                quark::neg(xi,Gamma);
                LU::solve(W,xi);
                quark::mul(step,tNu,xi);
                quark::add(Cnew,Corg,step);
                
                //--------------------------------------------------------------
                //
                // balance the new position
                //
                //--------------------------------------------------------------
                if(!balance(Cnew))
                {
                    std::cerr << "[normalize] cannot balance" << std::endl;
                    return false;
                }

                //--------------------------------------------------------------
                //
                // test convergence
                //
                //--------------------------------------------------------------
                bool converged = true;
                for(size_t j=M;j>0;--j)
                {
                    if(active[j])
                    {
                        const double newC = Cnew[j];
                        const double oldC = Corg[j];
                        const double d    = fabs( newC - oldC );
                        const double c    = numeric<double>::ftol * max_of<double>(fabs(newC),fabs(oldC));
                        if(d>c)
                        {
                            converged = false;
                        }
                        Corg[j] = newC;
                    }
#ifndef NDEBUG
                    else
                    {
                        assert( fabs(Cnew[j]-Corg[j])<=0 );
                    }
#endif
                }

                if(converged)
                {
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

