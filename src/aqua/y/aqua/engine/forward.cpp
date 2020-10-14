
#include "y/aqua/engine.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/utils.hpp"
#include "y/ios/align.hpp"

namespace upsylon
{
    namespace Aqua
    {
        using namespace mkl;

        static const char PFX[] = "[forward] ";

#define Y_AQUA_PRINTLN(MSG) do { if(forwardVerbose) { std::cerr << PFX << MSG << std::endl; } } while(false)

        void Engine:: computeK(const double t)
        {
            for(size_t i=N;i>0;--i)
            {
                K[i] = equilibria[i]->K(t);
            }
        }

        void Engine:: updateKs() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                const Equilibrium &eq = *equilibria[i];
                double             C0 = 1;
                if(eq.dn!=0)
                {
                    C0 = pow(K[i],eq.idn);
                }
                Ks[i] = C0;
            }
        }

        void Engine:: computeQ(const accessible<double> &C) throw()
        {
            for(size_t i=N;i>0;--i)
            {
                Q[i] = equilibria[i]->computeQ(K[i],C);
            }
        }

        void Engine:: computeJ(const accessible<double> &C) throw()
        {
            for(size_t i=N;i>0;--i)
            {
                Q[i] = equilibria[i]->computeQ(J[i], K[i], C);
            }
        }

        bool Engine:: forward(addressable<double> &C) throw()
        {
            //------------------------------------------------------------------
            //
            //
            // initialize, concentration must be valid
            //
            //
            //------------------------------------------------------------------
            forwardCycles = 0;
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    Cini[j] = C[j]; assert(C[j]>=0);
                }
                else
                {
                    Cini[j] = 0;
                }
            }
            Y_AQUA_PRINTLN("Cini = "<<Cini);
            bool alreadySwept = false;

            //------------------------------------------------------------------
            //
            //
            // start cycles
            //
            //
            //------------------------------------------------------------------
        CYCLE:
            ++forwardCycles;
            Y_AQUA_PRINTLN(" << cycle " << forwardCycles << " >>");

            //------------------------------------------------------------------
            //
            // compute local Jacobian
            //
            //------------------------------------------------------------------
            computeJ(Cini);
            quark::mmul_rtrn(W,J,Nu);
            if( !LU::build(W) )
            {
                Y_AQUA_PRINTLN("singular system level-1");
                if(alreadySwept || !sweep(Cini) )
                {
                    Y_AQUA_PRINTLN("singular system level-2");
                    return false;
                }
                else
                {
                    alreadySwept = true;
                    goto CYCLE;
                }
            }

            alreadySwept = false;

            //------------------------------------------------------------------
            //
            // compute Newton's step
            //
            //------------------------------------------------------------------
            quark::neg(xi,Q);
            LU::solve(W,xi);
            quark::mul(step,tNu,xi);
            Y_AQUA_PRINTLN("Q    = "<<Q);
            Y_AQUA_PRINTLN("xi   = "<<xi);
            Y_AQUA_PRINTLN("step = "<<step);

            //------------------------------------------------------------------
            //
            // update to balanced position
            //
            //------------------------------------------------------------------
            quark::add(Cend,Cini,step);
            Y_AQUA_PRINTLN("Cend = "<<Cend);
            if(!balance(Cend))
            {
                Y_AQUA_PRINTLN("singular system balance");
                return false;
            }

            //------------------------------------------------------------------
            //
            // cehck convergence
            //
            //------------------------------------------------------------------
            const bool converged = __find<double>::convergence(Cini,Cend);
            if(converged)
            {
                Y_AQUA_PRINTLN("#converged");
                return true;
            }

            goto CYCLE;
        }


    }

}


