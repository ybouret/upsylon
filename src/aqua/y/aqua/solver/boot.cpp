#include "y/aqua/solver.hpp"
#include "y/aqua/boot.hpp"
#include "y/exception.hpp"
#include "y/mkl/kernel/quark.hpp"

namespace upsylon
{
    namespace Aqua
    {
        using namespace mkl;

        static const char fn[] = "Solver::boot: ";

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
            if(Nc>0)
            {
                // let us compute Cstr
                lightweight_array<double> Lambda( *Cswp, Nc);
                boot.fill(Lambda);
                std::cerr << "Lambda=" << Lambda << std::endl;
                quark::mul(Cstr,boot.F,Lambda);
                for(size_t j=M;j>0;--j) Cstr[j] /= boot.d;
                std::cerr << "Cstar=" << Cstr << std::endl;
            }
            else
            {
                
            }

        }
    }
}
