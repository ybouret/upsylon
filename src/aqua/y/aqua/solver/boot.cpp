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

        double Solver:: BootOnly(Array &C) throw()
        {

            for(size_t j=M;j>0;--j)
            {
                double Xj = 0;
                if(active[j])
                {
                    const double Cj = C[j];
                    if(Cj<0)
                    {
                        Xj = -Cj;
                    }
                }
                Caux[j] = Xj;
            }
            return sumCaux();
        }

        double Solver:: BootDrvs(Array &C, const Boot &boot) throw()
        {
            for(size_t j=M;j>0;--j)
            {
                double Xj = 0;
                double Dj = 0;
                if(active[j])
                {
                    const double Cj = C[j];
                    if(Cj<0)
                    {
                        Xj = -Cj;
                        Dj = 1;
                    }
                }
                Caux[j] = Xj;
                Ctry[j] = Dj;
            }
            std::cerr << "Caux=" << Caux << std::endl;
            std::cerr << "Grad=" << Ctry << std::endl;
            quark::mul(xi,boot.S,Ctry);
            quark::mul(Cstp,boot.tS,xi);
            std::cerr << "Cstp=" << Cstp << std::endl;
            return sumCaux();
        }


        double Solver::BootCall(const double x) throw()
        {
            quark::muladd(Ctry,Corg,x,Cstp);
            return BootOnly(Ctry);
        }

        double Solver::BootProxy:: operator()(const double x) throw()
        {
            assert(self);
            return self->BootCall(x);
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
                std::cerr << "Lambda=" << Lambda << std::endl;
                quark::mul(Cstar,boot.F,Lambda);
                for(size_t j=M;j>0;--j)
                {
                    Corg[j] = (Cstar[j] /= boot.d);
                }

                std::cerr << "Corg=" << Corg << std::endl;
                double B0 = BootDrvs(Corg,boot);
                std::cerr << "B0=" << B0 << std::endl;
                


            }
            else
            {

            }

        }
    }
}
