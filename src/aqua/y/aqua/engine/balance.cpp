
#include "y/aqua/engine.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/utils.hpp"

namespace upsylon
{
    namespace Aqua
    {
        using namespace mkl;

        static const char PFX[] = "[balance] ";

#define Y_AQUA_PRINTLN(MSG) do { if(balanceVerbose) { std::cerr << PFX << MSG << std::endl; } } while(false)

        double Engine:: BalanceInit(addressable<double> &C) throw()
        {
            illness = 0;
            for(size_t j=M;j>0;--j)
            {
                Ctry[j]    = 0;
                illegal[j] = false;
                if(active[j])
                {
                    const double Cj = C[j];
                    if(Cj<0)
                    {
                        ++illness;
                        Caux[illness] = -Cj;
                        Ctry[j]       = 1.0;
                        illegal[j]    = true;
                    }
                }
            }
            Y_AQUA_PRINTLN("ill  = " << illegal << " # " << illness);
            if(illness>0)
            {

                quark::mul(xi,Nu,Ctry);
                for(size_t i=N;i>0;--i)
                {
                    xi[i] /= nu2[i];
                    const Extents X(*equilibria[i],C,*Ctry);
                    std::cerr << "for <" << (*equilibria[i]).name << "> : " << X << std::endl;
                }
                quark::mul(Cstp,tNu,xi);
                Y_AQUA_PRINTLN("Grad = " << Ctry);
                Y_AQUA_PRINTLN("xi   = " << xi  );
                Y_AQUA_PRINTLN("Cstp = " << Cstp);


                hsort(*Caux,illness,comparison::decreasing<double>);
                double sum = 0;
                for(size_t j=illness;j>0;--j)
                {
                    sum += Caux[j];
                }
                return sum;

            }
            else
            {
                return 0;
            }
        }

        double Engine:: BalanceProxy:: operator()(const double x) throw()
        {
            assert(self);
            return self->BalanceCall(x);
        }


        double Engine:: BalanceCall(const double x) throw()
        {
            quark::muladd(Ctry, Corg,x, Cstp);
            for(size_t j=M;j>0;--j)
            {
                Caux[j] = 0;
                if(active[j])
                {
                    const double Cj = Ctry[j];
                    if(illegal[j])
                    {
                        Caux[j] = -Cj;
                    }
                    else
                    {
                        if(Cj<0)
                        {
                            Caux[j] = -Cj;
                        }
                    }

                }
            }

            hsort(Caux,comparison::decreasing<double>);
            double sum = 0;
            for(size_t j=illness;j>0;--j)
            {
                sum += Caux[j];
            }
            return sum;

        }

        bool Engine:: balance(addressable<double> &C) throw()
        {
            assert(C.size()>=M);
            balanceCycles = 0;
            
            // load local values
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


            double B0 = BalanceInit(Corg);
            Y_AQUA_PRINTLN("B0   = " << B0);
            if(B0<=0)
            {
                // initially good
                Y_AQUA_PRINTLN("# already balanced");
                return true;
            }
            else
            {
                // look up
                BalanceProxy F = { this };
            CYCLE:
                // rescale step
                assert(B0>0);

                double S2 = 0;
                for(size_t j=M;j>0;--j)
                {
                    S2 += square_of(Cstp[j]);
                }
                Y_AQUA_PRINTLN("S2   = " << S2);

                if(S2<=0)
                {
                    Y_AQUA_PRINTLN("# blocked!");
                    return false;
                }
                quark::rescale(Cstp,B0/sqrt(S2));
                Y_AQUA_PRINTLN("Cstp = " << Cstp);

                {
                    ios::ocstream fp("balance.dat");
                    for(double x=0;x<=4;x+=0.01)
                    {
                        fp("%g %g\n",x,F(x));
                    }
                }

                // find
                double x1 = 1;
                double B1 = F(x1);
                triplet<double> x = { 0,x1,x1 };
                triplet<double> b = { B0,B1,B1};
                if(B1>=B0)
                {
                    //----------------------------------------------------------
                    /**/ Y_AQUA_PRINTLN("# shrink");
                    //----------------------------------------------------------
                    B1 = F( x1 = minimize::run(F,x,b,minimize::inside) );
                }
                else
                {
                    //----------------------------------------------------------
                    /**/ Y_AQUA_PRINTLN("# expand"); //B1<B0
                    //----------------------------------------------------------
                    do
                    {
                        b.c = F( x.c+=1 );
                    }
                    while(b.c<B1);
                    B1 = F( x1 = minimize::run(F,x,b,minimize::direct) );
                }
                Y_AQUA_PRINTLN("B1   = " << B1 << " # @" << x1);
                Y_AQUA_PRINTLN("Ctry = " << Ctry);

                if(B1<=0)
                {
                    B0=0;
                    goto SUCCESS;
                }
                else
                {

                    const bool   Ccvg   = __find<double>::convergence(Corg,Ctry);
                    const double deltaB = fabs(B1-B0);
                    const bool   Bcvg   = deltaB <= numeric<double>::ftol * max_of(B0,B1);
                    if(Ccvg||Bcvg) goto CONVERGED;

                    // ready for another cycle
                    return false;
                    B0 = BalanceInit(Corg);
                    goto CYCLE;
                }

            SUCCESS:
                Y_AQUA_PRINTLN("# success");
                assert(B0<=0);
                for(size_t j=M;j>0;--j)
                {
                    if(active[j])
                    {
                        C[j] = max_of(Corg[j],0.0);
                    }
                }
                return true;

            CONVERGED:
                Y_AQUA_PRINTLN("# converged");

                exit(1);
                return true;


            }

        }

    }

}


