
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

        static const char PFX[] = "[balance] ";

#define Y_AQUA_PRINTLN(MSG) do { if(balanceVerbose) { std::cerr << PFX << MSG << std::endl; } } while(false)


        double Engine:: BalanceProxy:: operator()(const double x) throw()
        {
            assert(self);
            return self->BalanceCheck(x);
        }


        double Engine:: sumCaux(const size_t m) throw()
        {
            switch(m)
            {
                case 0:  return 0;
                case 1:  return Caux[1];
                default: break;
            }
            hsort(*Caux,m,comparison::decreasing<double>);
            double sum = 0;
            for(size_t j=m;j>0;--j)
            {
                sum += Caux[j];
            }
            return sum;
        }

        double Engine:: BalanceValue() throw()
        {
            static const double zcut = numeric<double>::tiny;

            size_t num = 0;
            for(size_t j=M;j>0;--j)
            {
                Cxs[j]     = 0;
                illegal[j] = false;
                const double Cj = Corg[j];
                if(Cj<0)
                {
                    const double C2 = Cj*Cj;
                    if(C2<=zcut)
                    {
                        Corg[j] = 0;
                    }
                    else
                    {
                        illegal[j]      = true;
                        Cxs[j]          = -Cj;
                        Caux[++num] =  C2;
                    }
                }
            }
            return  sqrt(sumCaux(num));
        }

        double Engine:: BalanceCheck(const double x) throw()
        {
            static const double zcut = numeric<double>::tiny;
            size_t num = 0;
            for(size_t j=M;j>0;--j)
            {
                const double Cj = Corg[j] + x * Cstp[j];
                const double C2 = Cj*Cj;

                if(C2<=zcut)
                {
                    Ctry[j] = 0;
                    continue;
                }
                else
                {
                    Ctry[j] = Cj;
                    if(illegal[j])
                    {
                        Caux[++num] = C2;
                    }
                    else
                    {
                        if(Cj<0)
                        {
                            Caux[++num] = C2;
                        }
                    }
                }
            }
            return  sqrt(sumCaux(num));
        }


        bool Engine:: BalanceDelta() throw()
        {
            static const double zcut = numeric<double>::tiny;

            size_t num = 0;
            for(size_t j=M;j>0;--j)
            {
                Cstp[j] = 0;
                if(active[j])
                {
                    const  array<Int> &cof = Prj[j];
                    for(size_t k=M;k>0;--k)
                    {
                        Ctry[k] = cof[k] * Cxs[k];
                    }
                    hsort(Ctry,comparison::decreasing_abs<double>);
                    double sum = 0;
                    for(size_t k=M;k>0;--k)
                    {
                        sum += Ctry[k];
                    }
                    const double delta  = sum/det;
                    const double delta2 = square_of(delta);
                    if(delta2<=zcut)
                    {
                        continue;;
                    }
                    Cstp[j]     = delta;
                    Caux[++num] = delta2;
                }

            }
            Y_AQUA_PRINTLN("Cstp = " << Cstp);
            const double fwd = sumCaux(num)/M;
            Y_AQUA_PRINTLN("fwd  = " << fwd << " #" << (fwd>0) );
            return (fwd>0);
        }


        bool Engine::balance(addressable<double> &C) throw()
        {
            assert(C.size()>=M);
            balanceCycles = 0;

            //------------------------------------------------------------------
            //
            //
            // load local values
            //
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    Corg[j] = C[j];
                }
                else
                {
                    Corg[j] = 0;
                }
            }
            Y_AQUA_PRINTLN("Corg = " << Corg);

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            double B0 = BalanceValue();
            Y_AQUA_PRINTLN("B0   = " << B0);
            Y_AQUA_PRINTLN("Cxs  = " << Cxs);
            Y_AQUA_PRINTLN("ill  = " << illegal);
            if(B0<=0)
            {
                Y_AQUA_PRINTLN("#already balanced!");
                return true;
            }

            //------------------------------------------------------------------
            //
            //
            // perform cycles
            //
            //
            //------------------------------------------------------------------
            BalanceProxy F = { this };

        CYCLE:
            ++balanceCycles;
            Y_AQUA_PRINTLN(" # << CYCLE=" << balanceCycles << ">>" );
            //------------------------------------------------------------------
            //
            // compute step
            //
            //------------------------------------------------------------------
            if(!BalanceDelta())
            {
                //--------------------------------------------------------------
                // numeric convergence on step
                //--------------------------------------------------------------
                quark::set(Ctry,Corg);
                goto CONVERGED;
            }
            else
            {
                //--------------------------------------------------------------
                // follow the step
                //--------------------------------------------------------------
                if(false)
                {
                    ios::ocstream fp("balance.dat");
                    for(double x=-1;x<=5.0;x+=0.01)
                    {
                        fp("%.20g %.20g\n", x, F(x) );
                    }
                }

                double x1 = 1;
                double B1 = F(x1);
                Y_AQUA_PRINTLN("B1   = " << B1);

                //--------------------------------------------------------------
                // correct the step
                //--------------------------------------------------------------
                {
                    triplet<double> x = { 0,  x1, x1 };
                    triplet<double> b = { B0 ,B1, B1 };
                    if(B1>=B0)
                    {
                        Y_AQUA_PRINTLN("#shrink");
                        B1 = F( x1 = minimize::run(F,x,b,minimize::inside));
                    }
                    else
                    {
                        Y_AQUA_PRINTLN("#expand");
                        do
                        {
                            b.c = F( x.c += 1.0 );
                        }
                        while(b.c<B1);
                        B1 = F( x1 = minimize::run(F,x,b,minimize::direct));
                    }
                    Y_AQUA_PRINTLN("B1   = " << B1 << " # @" << x1);;
                }
                Y_AQUA_PRINTLN("Ctry = " << Ctry);

                if(B1<=0)
                {
                    //----------------------------------------------------------
                    // tighten before success
                    //----------------------------------------------------------
                    Y_AQUA_PRINTLN("#tighten");
                    B0 = 0;
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
                    } while( (x1-x0) > 1e-4 );
                    B1 = F(x1);
                    goto SUCCESS;
                }
                else
                {
                    //----------------------------------------------------------
                    // test convergence
                    //----------------------------------------------------------
                    {
                        const bool Ccvg = __find<double>::convergence(Corg,Ctry);
                        if(Ccvg)
                        {
                            Y_AQUA_PRINTLN("# converged in C");
                            goto CONVERGED;
                        }
                    }

                    {

                        const bool Bcvg = fabs(B0-B1) <= numeric<double>::ftol * fabs( max_of(B0,B1) );
                        if(Bcvg)
                        {
                            Y_AQUA_PRINTLN("# converged in B");
                            goto CONVERGED;
                        }
                    }

                    //----------------------------------------------------------
                    // update for next cycle
                    //----------------------------------------------------------
                    B0 = BalanceValue(); //@ Corg=Ctry
                    if(balanceCycles>1000)
                    {
                        return false;
                    }
                    goto CYCLE;
                }
            }

        SUCCESS:
            Y_AQUA_PRINTLN("#success @" << Ctry);
            assert(B0<=0);
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    C[j] = max_of(Ctry[j],0.0);
                }
            }
            Y_AQUA_PRINTLN("C=" << C );
            return true;

        CONVERGED:
            Y_AQUA_PRINTLN("#converged @" << Ctry);
            size_t npos = 0;
            for(size_t j=M;j>0;--j)
            {
                const double Cj = Ctry[j];
                if(Cj>0)
                {
                    Caux[++npos] = Cj;
                }
            }
            if(npos<=0)
            {
                Y_AQUA_PRINTLN("#no active positive values");
                return false;
            }
            else
            {
                lightweight_array<double> Cpos(*Caux,npos);
                hsort(Cpos,comparison::increasing<double>);
                const size_t nmid = npos>>1;
                const double Cmed = (0!= (npos&1) ) ? Cpos[1+nmid] : 0.5*(Cpos[nmid]+Cpos[1+nmid]);
                const double Cmin = Cmed * numeric<double>::ftol;
                Y_AQUA_PRINTLN("Cpos = " << Cpos << " #=" << npos << ", Cmed=" << Cmed << ", Cmin=" << Cmin);
                for(size_t j=M;j>0;--j)
                {
                    const double Cj = Ctry[j];
                    if(Cj<0)
                    {
                        if(fabs(Cj)>Cmin)
                        {
                            Y_AQUA_PRINTLN("#unable to balance " << C);
                            return false;
                        }
                    }
                    else
                    {
                        if(fabs(Cj)<=Cmin)
                        {
                            Ctry[j] = 0;
                        }
                    }
                }
            }
            B0 = 0;
            goto SUCCESS;
        }



    }

}


