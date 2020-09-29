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
                    std::cerr << "already good" << std::endl;
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
                    std::cerr << "B0=" << B0 << " / " << Bfunc(Corg) << std::endl;

                    std::cerr << "C   =" << C    << std::endl;
                    std::cerr << "Corg=" << Corg << std::endl;
                    std::cerr << "drvs=" << Ctry << std::endl;
                    std::cerr << "delB=" << Cstp << std::endl;

                    //----------------------------------------------------------
                    //
                    // rescaling
                    //
                    //----------------------------------------------------------
                    {
                        const double S2 = Norm2(Cstp);
                        std::cerr << "S2=" << S2 << std::endl;
                        if(S2<=0)
                        {
                            std::cerr << "unable to balance" << std::endl;
                            return false;
                        }
                        const double C2 = Norm2(Corg);
                        std::cerr << "C2=" << C2 << std::endl;
                        const double fac = sqrt(C2/S2);
                        for(size_t j=M;j>0;--j)
                        {
                            Cstp[j] *= fac;
                        }
                    }

                    std::cerr << "Cstp=" << Cstp << std::endl;

                    //----------------------------------------------------------
                    //
                    // optimize
                    //
                    //----------------------------------------------------------
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
                            std::cerr << "expand" << std::endl;
                            std::cerr << x << " => " << B << std::endl;
                            while(true)
                            {
                                assert(B.b<=B.a);
                                x.shift( x.c+1  );
                                B.shift( F(x.c) );
                                std::cerr << x << " => " << B << std::endl;
                                if(B.c>=B.b)
                                    break;
                            }
                            B1 = F( x1 = minimize::run(F,x,B,minimize::direct) );
                        }
                        else
                        {
                            std::cerr << "shrink" << std::endl;
                            B1 = F( x1 = minimize::run(F,x,B,minimize::inside) );
                        }
                    }

                    //----------------------------------------------------------
                    //
                    // check result
                    //
                    //----------------------------------------------------------
                    std::cerr << "B1=" << B1 << "@" << x1 << std::endl;
                    std::cerr << "Ctry=" << Ctry << std::endl;
                    
                    if(B1>0)
                    {
                        // check C convergence and update
                        const bool   ccvg = __find<double>::convergence(Corg,Ctry);

                        // check B convergence and update
                        const double dB   = fabs(B1-B0);
                        const bool   bcvg = (dB+dB) <= numeric<double>::ftol * (B0+B1);
                        std::cerr << "ccvg=" << ccvg << std::endl;
                        std::cerr << "bcvg=" << bcvg << std::endl;

                        if(ccvg||bcvg)
                        {
                            std::cerr << "converged..." << std::endl;
                            exit(1);
                        }

                        B0 = Bdrvs(Corg);
                        goto CYCLE;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // success
                        //------------------------------------------------------
                        std::cerr << "success" << std::endl;
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
                std::cerr << "C=" << C << std::endl;
                return true;
            }
            
        }
        
    }
    
}
