#include "y/aqua/solver.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/utils.hpp"

namespace upsylon
{
    
    namespace Aqua
    {
        using namespace mkl;


        static const char fn[] = "[balance] ";
#define Y_AQUA_PRINTLN(MSG) do { if(balanceVerbose) { std::cerr << fn << MSG << std::endl; } } while(false)

        double Solver::Norm2(const Array &C) throw()
        {
            assert(C.size() == M);

            for(size_t j=M;j>0;--j)
            {
                Caux[j] = square_of(C[j]);
            }
            hsort(Caux,comparison::decreasing<double>);
            double C2 = 0;
            for(size_t j=M;j>0;--j)
            {
                C2 += Caux[j];
            }
            return C2;
        }

        static double b_func(const double x) throw()
        {
            return x<=0 ? -x : 0;
        }

        static int b_drvs(const double x) throw()
        {
            return x<0 ? -1 : 0;
        }

        double Solver:: Bfunc(const Array &C) throw()
        {
            double B = 0;
            for(size_t j=M;j>0;--j)
            {
                Caux[j] = b_func(C[j]);
            }
            hsort(Caux,comparison::decreasing<double>);
            for(size_t j=M;j>0;--j)
            {
                B += Caux[j];
            }
            return B;
        }

        double Solver:: Bdrvs(const Array &C) throw()
        {
            // compute for B and B'
            for(size_t j=M;j>0;--j)
            {
                const double Cj =  C[j];
                Caux[j]         =  b_func(Cj);
                Ctry[j]         = -b_drvs(Cj);
            }
            hsort(Caux,comparison::decreasing<double>);
            double B  = 0;
            for(size_t j=M;j>0;--j)
            {
                B  += Caux[j];
            }

            // compute unscaled step
            quark::mul(Cstp,Nu2,Ctry);

            return B;
        }

        double Solver:: Bcall(const double x) throw()
        {
            quark::muladd(Ctry,Corg,x,Cstp);
            return Bfunc(Ctry);
        }

        double Solver:: BalanceProxy:: operator()(const double x) throw()
        {
            assert(self);
            return self->Bcall(x);
        }

        bool Solver:: balance(addressable<double> &C) throw()
        {
            assert(C.size()>=M);
            if(N<=0)
            {
                //--------------------------------------------------------------
                // trivial case
                //--------------------------------------------------------------
                Y_AQUA_PRINTLN("no equilibrium");
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // initialize, keeping only active components
                //
                //--------------------------------------------------------------
                for(size_t i=M;i>0;--i)
                {
                    if(used[i])
                    {
                        Corg[i] = C[i];
                    }
                    else
                    {
                        Corg[i] = 0;
                    }
                }
                
                //--------------------------------------------------------------
                //
                // evaluate value and initial step
                //
                //--------------------------------------------------------------
                double B0 = Bdrvs(Corg);
                if(B0<=0)
                {
                    Y_AQUA_PRINTLN("already");
                    goto SUCCESS;
                }
                else
                {
                    BalanceProxy F = { this };

                CYCLE:
                    //----------------------------------------------------------
                    //
                    // at this point, B0 and unscaled Cstp are evaluated, B0>0
                    //
                    //----------------------------------------------------------
                    Y_AQUA_PRINTLN("B0="<<B0);
                    Y_AQUA_PRINTLN("Corg="<<Corg);
                    //Y_AQUA_PRINTLN("drvs=" << Ctry);
                    //Y_AQUA_PRINTLN("delB=" << Cstp);

                    //----------------------------------------------------------
                    //
                    // rescaling
                    //
                    //----------------------------------------------------------
                    {
                        const double S2 = Norm2(Cstp);
                        if(S2<=0)
                        {
                            Y_AQUA_PRINTLN("unable!");
                            return false;
                        }
                        const double C2 = Norm2(Corg);
                        const double fac = sqrt(C2/S2);
                        for(size_t j=M;j>0;--j)
                        {
                            Cstp[j] *= fac;
                        }
                    }
                    Y_AQUA_PRINTLN("Cstp=" << Cstp);

                    //----------------------------------------------------------
                    //
                    // optimize
                    //
                    //----------------------------------------------------------
                    if(false)
                    {
                        ios::ocstream fp("balance.dat");
                        for(double x=0;x<=2.0;x+=0.01)
                        {
                            fp("%g %g\n",x,F(x));
                        }

                    }

                    double          x1 = 1;
                    double          B1 = F(1);
                    {
                        triplet<double> x  = { 0,  x1, x1 };
                        triplet<double> B  = { B0, B1, B1 };
                        if(B1<B0)
                        {
                            Y_AQUA_PRINTLN("expanding");
                            while(true)
                            {
                                assert(B.b<=B.a);
                                x.shift( x.c+1  );
                                B.shift( F(x.c) );
                                if(B.c>=B.b)
                                    break;
                            }
                            B1 = F( x1 = minimize::run(F,x,B,minimize::direct) );
                        }
                        else
                        {
                            Y_AQUA_PRINTLN("shrinking");
                            B1 = F( x1 = minimize::run(F,x,B,minimize::inside) );
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // check result
                    //
                    //----------------------------------------------------------
                    Y_AQUA_PRINTLN("B1   = " << B1 << "@" << x1);
                    Y_AQUA_PRINTLN("Ctry = " << Ctry);
                    
                    if(B1>0)
                    {
                        // check C convergence and update
                        const bool   ccvg = __find<double>::convergence(Corg,Ctry);

                        // check B convergence
                        const double dB   = fabs(B1-B0);
                        const bool   bcvg = (dB+dB) <= numeric<double>::ftol * (B0+B1);
                        Y_AQUA_PRINTLN( "ccvg=" << ccvg << " | bcvg=" << bcvg);

                        if(ccvg||bcvg)
                        {
                            std::cerr << "converged..." << std::endl;
                            exit(1);
                        }

                        // update B0 and unscaled Cstp
                        B0 = Bdrvs(Corg);
                        goto CYCLE;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // success
                        //------------------------------------------------------
                        Y_AQUA_PRINTLN("success");
                        B0 = 0;
                        quark::set(Corg,Ctry);
                        goto SUCCESS;
                    }

                }

                //--------------------------------------------------------------
                //
                // numeric success
                //
                //--------------------------------------------------------------
            SUCCESS:
                assert(B0<=0);
                for(size_t i=M;i>0;--i)
                {
                    if(used[i])
                    {
                        C[i] = max_of(Corg[i],0.0);
                    }
                }
                return true;
            }
            
        }
        
    }
    
}
