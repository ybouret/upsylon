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

        double Solver::sumCaux() throw()
        {
            hsort(Caux,comparison::decreasing<double>);
            double ans = 0;
            for(size_t j=M;j>0;--j)
            {
                ans += Caux[j];
            }
            return ans;
        }

        double Solver::  B_only(const Array &C) throw()
        {

            for(size_t j=M;j>0;--j)
            {
                const double Cj = C[j];
                if(Cj<0)
                {
                    Caux[j] = -Cj;
                }
                else
                {
                    Caux[j] = 0;
                }
            }
            return sumCaux();
        }

        double Solver:: B_drvs(const Array &C) throw()
        {
            for(size_t j=M;j>0;--j)
            {
                const double Cj = C[j];
                if(Cj<0)
                {
                    Caux[j] = -Cj;
                    Ctry[j] =  1;
                }
                else
                {
                    Caux[j] = 0;
                    Ctry[j] = 0;
                }
            }
            quark::mul(Cstp,Nu2,Ctry);
            return sumCaux();
        }


        
        double Solver:: NormSq(const Array &C) throw()
        {
            for(size_t j=M;j>0;--j)
            {
                Caux[j] = square_of(C[j]);
            }
            return sumCaux();
        }


        bool Solver::rescale() throw()
        {
            const double S2 = NormSq(Cstp);
            Y_AQUA_PRINTLN("S2="<<S2);
            if(S2<=0)
            {
                Y_AQUA_PRINTLN("blocked@"<<Corg);
                return false;
            }
            else
            {
                for(size_t j=M;j>0;--j)
                {
                    const double Cj = Corg[j];
                    if(Cj<0)
                    {
                        Caux[j] = square_of(Cj);
                    }
                    else
                    {
                        Caux[j] = 0;
                    }
                }
                const double C2 = sumCaux();
                Y_AQUA_PRINTLN("C2="<<C2);
                const double factor = sqrt(C2/S2);
                for(size_t j=M;j>0;--j)
                {
                    Cstp[j] *= factor;
                }
                Y_AQUA_PRINTLN("Cstp=" << Cstp);
                return true;
            }

        }

        double Solver:: B_proxy:: operator()(const double x) throw()
        {
            assert(self);
            return self->B_call(x);
            
        }

        double Solver:: B_call(const double x) throw()
        {
            quark::muladd(Ctry, Corg, x, Cstp);
            //std::cerr << "Ctry@" << x << "=" << Ctry << std::endl;
            return B_only(Ctry);
        }

        bool Solver:: balance(addressable<double> &C) throw()
        {
            assert(C.size()>=M);
            if(N<=0)
            {
                //--------------------------------------------------------------
                //
                // trivial case
                //
                //--------------------------------------------------------------
                Y_AQUA_PRINTLN("no equilibrium");
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // setup
                //
                //--------------------------------------------------------------

                // copy values
                for(size_t j=M;j>0;--j)
                {
                    if(used[j])
                    {
                        Corg[j] = C[j];
                    }
                    else
                    {
                        Corg[j] = 0;
                    }
                }

                // initialize B0 and full step
                double B0 = B_drvs(Corg);
                Y_AQUA_PRINTLN("Corg=" << Corg);
                Y_AQUA_PRINTLN("drvs=" << Ctry);
                Y_AQUA_PRINTLN("step=" << Cstp);
                Y_AQUA_PRINTLN("B0  =" << B0  );
                if(B0<=0)
                {
                    //----------------------------------------------------------
                    //
                    // no need to look up
                    //
                    //----------------------------------------------------------
                    Y_AQUA_PRINTLN("already");
                    goto SUCCESS;
                }
                else
                {
                    B_proxy F = { this };
                CYCLE:
                    //----------------------------------------------------------
                    //
                    // compute effective rescaled step
                    //
                    //----------------------------------------------------------
                    if(!rescale())
                        return false;

                    //----------------------------------------------------------
                    //
                    // initialize withh full step
                    //
                    //----------------------------------------------------------
                    double x1 = 1;
                    double B1 = F(x1);
                    Y_AQUA_PRINTLN("B1  =" << B1  );

#if 1
                    ios::ocstream::overwrite("balance.dat");
                    if(true)
                    {
                        ios::ocstream fp("balance.dat");
                        for(double x=0;x<=3.0;x+=0.001)
                        {
                            fp("%g %g\n",x,F(x));
                        }
                    }
#endif

                    //----------------------------------------------------------
                    //
                    // minimisation
                    //
                    //----------------------------------------------------------
                    {
                        triplet<double> x = { 0,  x1, x1 };
                        triplet<double> B = { B0, B1, B1 };

                        if(B1>=B0)
                        {
                            //--------------------------------------------------
                            // with inside bracketing
                            //--------------------------------------------------
                            Y_AQUA_PRINTLN(" # => shrinking");
                            B1 = F( x1 = minimize::run(F,x,B,minimize::inside) );
                        }
                        else
                        {
                            //--------------------------------------------------
                            // B1 < B0 : local expansion
                            //--------------------------------------------------
                            Y_AQUA_PRINTLN(" # => expanding");
                            while(true)
                            {
                                x.b = x.c;
                                B.b = B.c;
                                x.c += 1.0;
                                B.c  = F(x.c);
                                if(B.c>=B.b)
                                {
                                    break;
                                }
                            }
                            B1 = F( x1 = minimize::run(F,x,B,minimize::direct) );
                        }
                        Y_AQUA_PRINTLN("B1  = " << B1  << " @ " << x1);
                    }


                    //----------------------------------------------------------
                    //
                    // backtracking : find the smallest step
                    //
                    //----------------------------------------------------------
                    double x0 = 0;
                    while(true)
                    {
                        const double xm = 0.5*(x0+x1);
                        const double Bm = F(xm);
                        if(Bm>B1)
                        {
                            x0 = xm;
                        }
                        else
                        {
                            x1 = xm;
                        }
                        if( fabs(x1-x0) <= 1e-4 ) break;
                    }

                    //----------------------------------------------------------
                    //
                    // finalizing
                    //
                    //----------------------------------------------------------
                    B1 = F(x1);
                    Y_AQUA_PRINTLN("B1  = " << B1  << " @ " << x1);

                    if(B1<=0)
                    {
                        //------------------------------------------------------
                        // exact numeric success
                        //------------------------------------------------------
                        Y_AQUA_PRINTLN("success!");
                        quark::set(Corg,Ctry);
                        goto SUCCESS;
                    }
                    else
                    {

                        //------------------------------------------------------
                        // test convergence
                        //------------------------------------------------------
                        
                        quark::set(Corg,Ctry);
                        B0 = B_drvs(Corg);
                        goto CYCLE;
                    }

                    return false;
                }

            SUCCESS:
                Y_AQUA_PRINTLN("Cend="<<Corg);
                for(size_t j=M;j>0;--j)
                {
                    if(used[j])
                    {
                        C[j] = max_of(0.0,Corg[j]);
                    }
                }
                return true;
            }
        }
        
    }
    
}
