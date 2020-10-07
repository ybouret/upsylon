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
                        if( Xj <= numeric<double>::tiny )
                        {
                            Xj   = 0;
                            C[j] = 0;
                        }
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
                        if(Xj<=numeric<double>::tiny)
                        {
                            Xj   = 0;
                            Dj   = 0;
                            C[j] = 0;
                        }

                    }
                }
                Caux[j] = Xj;
                Ctry[j] = Dj;
            }


            quark::mul(xi,boot.S,Ctry);
            quark::mul(Cstp,boot.tS,xi);
            Y_AQUA_PRINTLN("Caux = "<<Caux);
            Y_AQUA_PRINTLN("Grad = "<<Ctry);
            Y_AQUA_PRINTLN("Step = "<<Cstp);
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

        
        bool   Solver:: bootBalance(const Boot &boot) throw()
        {
            //------------------------------------------------------------------
            // initialize
            //------------------------------------------------------------------
            lastBalanceCycles=0;
            double B0 = BootDrvs(Corg,boot);
            Y_AQUA_PRINTLN("Corg = "<<Corg);
            Y_AQUA_PRINTLN("B0   = "<<B0);
            if(B0<=0)
            {
                Y_AQUA_PRINTLN("# valid!");
                return true;
            }

            BootProxy F = { this };
        CYCLE:
            ++lastBalanceCycles;
            Y_AQUA_PRINTLN("# <cycle " << lastBalanceCycles << ">" );
            // at this point, B0 and unscaled Cstp are computed
            if(!rescale(B0))
            {
                return false;
            }
            Y_AQUA_PRINTLN("Cstp = "<<Cstp);

            {
                ios::ocstream fp("boot.dat");
                for(double x=0;x<=3;x+=0.01)
                {
                    fp("%g %g\n", x, F(x));
                }
            }

            //------------------------------------------------------------------
            //
            // move
            //
            //------------------------------------------------------------------
            double x1 = 1;
            double B1 = F(x1);

            if(B1<B0)
            {
                // accept
                Y_AQUA_PRINTLN("# expand");
                triplet<double> x = { 0,  x1, x1*3 };
                triplet<double> b = { B0, B1, F(x.c) };
                if(b.c>=B1)
                {
                    Y_AQUA_PRINTLN("# optimum");
                    B1 = F( x1 = minimize::run(F,x,b,minimize::inside) );
                }
                else
                {
                    Y_AQUA_PRINTLN("# forward");
                    x1 = x.c;
                    B1 = b.c;
                }

            }
            else
            {
                // B1 >= B0
                Y_AQUA_PRINTLN("# shrink");
                triplet<double> x = { 0,  x1, x1 };
                triplet<double> b = { B0, B1, B1 };
                B1 = F( x1 = minimize::run(F,x,b,minimize::inside) );
            }

            //------------------------------------------------------------------
            //
            // check results
            //
            //------------------------------------------------------------------
            if(B1<=0)
            {
                //--------------------------------------------------------------
                // backtrack on success
                //--------------------------------------------------------------
                double x0 = 0;
                do
                {
                    const double xm = 0.5*(x0+x1);
                    const double Bm = F(xm);
                    if(Bm<=0)
                    {
                        x1 = xm;
                    }
                    else
                    {
                        x0 = xm;
                    }
                } while( fabs(x1-x0) > 1e-4 );
                B0 = F(x1); assert(B0<=0);
                quark::set(Corg,Ctry);
                goto SUCCESS;
            }
            else
            {
                //--------------------------------------------------------------
                // check convergence and ready for next cycle
                //--------------------------------------------------------------
                Y_AQUA_PRINTLN("#testing");
                bool Ccvg = true;
                for(size_t j=M;j>0;--j)
                {
                    const double old = Corg[j];
                    const double now = Ctry[j];
                    if( Ccvg && (fabs(old-now)>numeric<double>::ftol * max_of( fabs(old), fabs(now) )) )
                    {
                        Ccvg = false;
                    }
                    Corg[j] = now;
                }
                Y_AQUA_PRINTLN("Corg = " << Corg << " # @" << x1);
                const double dB   = fabs(B1-B0);
                const bool   Bcvg = (dB <= numeric<double>::ftol * max_of(B1,B0));
                Y_AQUA_PRINTLN("dB   = "<<dB);
                Y_AQUA_PRINTLN("#convergence: C:" << Ccvg << " B:" << Bcvg);
                if(Bcvg||Ccvg)
                {
                    goto CONVERGED;
                }
                B0 = BootDrvs(Corg,boot);
            }

            goto CYCLE;

        SUCCESS:
            Y_AQUA_PRINTLN("# success");
            assert(B0<=0);
            Y_AQUA_PRINTLN("Corg = " << Corg << " # @" << x1);
            return true;

        CONVERGED:
            Y_AQUA_PRINTLN("# converged");

            exit(1);
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

                bootBalance(boot);
                


            }
            else
            {

            }

        }
    }
}
