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

#define Y_AQUA_PRINTLN(MSG) do { if(balanceVerbose) { std::cerr << pfx << MSG << std::endl; } } while(false)

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
                quark::mul(Cstar,boot.F,Lambda);
                Y_AQUA_PRINTLN("Lam  = "<<Lambda);
                for(size_t j=M;j>0;--j)
                {
                    Corg[j] = (Cstar[j] /= boot.d);
                }
                balance(Corg,boot);
                


            }
            else
            {

            }

        }
    }
}
