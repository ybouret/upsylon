
#include "y/aqua/solver.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/utils.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;
        static const char fn[] = "[forward] ";
#define Y_AQUA_PRINTLN(MSG) do { if(forwardVerbose) { std::cerr << fn << MSG << std::endl; } } while(false)

        bool Solver:: forward(addressable<double> &C) throw()
        {
            assert(C.size()>=M);

            //------------------------------------------------------------------
            //
            // initialize Cfwd with active concentrations
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    Cini[j] = C[j];
                }
                else
                {
                    Cini[j] = 0;
                }
            }

            //------------------------------------------------------------------
            //
            // cycle
            //
            //------------------------------------------------------------------
            size_t cycle = 0;
            while(true)
            {
                ++cycle;

                //--------------------------------------------------------------
                //
                // compute status: Q and Phi
                //
                //--------------------------------------------------------------
                Y_AQUA_PRINTLN("#\t<cycle " << cycle << ">");
                Y_AQUA_PRINTLN("Cini = "<<Cini);
                computeS(Cini);
                if(!computeW())
                {
                    Y_AQUA_PRINTLN("singular system!");
                    return false;
                }
                Y_AQUA_PRINTLN("Q   = "<<Q);
                //Y_AQUA_PRINTLN("Phi = "<<Phi);

                //--------------------------------------------------------------
                //
                // compute extenet
                //
                //--------------------------------------------------------------
                quark::neg(xi,Q);
                LU::solve(W,xi);

                //--------------------------------------------------------------
                //
                // compute new position
                //
                //--------------------------------------------------------------
                for(size_t i=N;i>0;--i)
                {
                    Cend[i] = Cini[i] + quark::dot<double>::of(tNu[i],xi);
                }
                Y_AQUA_PRINTLN("Ctmp = "<<Cend);
                if(!balance(Cend))
                {
                    return false;
                }
                Y_AQUA_PRINTLN("Cend = "<<Cend);
                const bool cvg = __find<double>::convergence(Cini,Cend);
                if(cvg)
                {
                    Y_AQUA_PRINTLN("converged");
                    break;
                }
            }

            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    C[j] = Cini[j];
                }
            }

            return true;
        }

    }

}

