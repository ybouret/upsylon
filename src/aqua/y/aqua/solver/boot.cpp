#include "y/aqua/solver.hpp"
#include "y/aqua/boot.hpp"
#include "y/exception.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/opt/minimize.hpp"

#include "y/ios/ocstream.hpp"

namespace upsylon
{
    namespace Aqua
    {
        using namespace mkl;

        static const char fn[] = "Solver::boot: ";
        static const char pfx[] = "[booting] ";

#define Y_AQUA_PRINTLN(MSG) do { if(bootingVerbose) { std::cerr << pfx << MSG << std::endl; } } while(false)

        bool Solver:: balance(addressable<double> &C, const Boot &boot) throw()
        {
            return balance(C,boot.S,boot.tS);
        }


        void Solver:: boot(addressable<double> &C,
                           const Boot          &boot)
        {
            //------------------------------------------------------------------
            //
            // check validity
            //
            //------------------------------------------------------------------

            assert(boot.P.rows==boot.size);
            if(boot.size+N!=M)
            {
                throw exception("%s#species=%u != #equilibrium=%u + #constraints=%u",fn, unsigned(M), unsigned(N), unsigned(boot.size) );
            }

            //------------------------------------------------------------------
            //
            // solve
            //
            //------------------------------------------------------------------
            assert(Nc==boot.size);
            assert(N==boot.S.rows);

            if(Nc>0)
            {
                //--------------------------------------------------------------
                //
                // let us compute Cstar and Corg
                //
                //--------------------------------------------------------------
                lightweight_array<double> Lambda( *Cswp, Nc);
                boot.fill(Lambda);
                Y_AQUA_PRINTLN("Lam  = "<<Lambda);
                quark::mul(Cstar,boot.F,Lambda);
                for(size_t j=M;j>0;--j)
                {
                    Corg[j] = (Cstar[j] /= boot.d);
                }
                if(!balance(Corg,boot))
                {
                    throw exception("%sfailed to balance initial state",fn);
                }
                Y_AQUA_PRINTLN("Corg = "<<Corg);
                

            }
            else
            {

            }

        }
    }
}
