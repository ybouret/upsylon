
#include "y/aqua/engine.hpp"
#include "y/mkl/kernel/quark.hpp"
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
            
            computeJ(C);
            quark::mmul_rtrn(W,J,Nu);
            

            return false;
        }


    }

}


