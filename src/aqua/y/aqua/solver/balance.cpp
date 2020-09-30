#include "y/aqua/solver.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/minimize.hpp"

namespace upsylon
{
    
    namespace Aqua
    {
        using namespace mkl;


        static const char fn[] = "[balance] ";
#define Y_AQUA_PRINTLN(MSG) do { if(balanceVerbose) { std::cerr << fn << MSG << std::endl; } } while(false)

        double Solver:: B_call(const double x) throw()
        {
            quark::muladd(Ctry, Corg,x, Cstp);
            return B_only(Ctry);
        }


        double Solver:: B_proxy :: operator()(const double x) throw()
        {
            assert(self);
            return self->B_call(x);
        }

        double Solver:: sumCaux() throw()
        {
            hsort(Caux, comparison::decreasing<double>);
            double sum = 0;
            for(size_t j=M;j>0;--j)
            {
                sum += Caux[j];
            }
            return sum;
        }

        double Solver:: B_only(const Array &C) throw()
        {
            for(size_t j=M;j>0;--j)
            {
                const double Cj = C[j];
                if(Cj<0)
                {
                    Caux[j]  = -Cj;
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
                    Caux[j]  = -Cj;
                    Ctry[j]  = 1;
                }
                else
                {
                    Caux[j] = 0;
                    Ctry[j] = 0;
                }
            }
            quark::mul(xi,Nu,Ctry);
            quark::mul(Cstp,tNu,xi);
            return sumCaux();
        }

        bool Solver:: rescale() throw()
        {
            const double S2 = quark::mod2<double>::of(Cstp);
            Y_AQUA_PRINTLN("S2   = "<<S2);

            if(S2<=0)
            {
                Y_AQUA_PRINTLN("blockded");
                return false;
            }


            return true;
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

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                double B0 = B_drvs(Corg);
                Y_AQUA_PRINTLN("Corg = "<<Corg);
                Y_AQUA_PRINTLN("B0   = "<<B0 << "/" << B_only(Corg));
                Y_AQUA_PRINTLN("G0   = "<<Ctry);
                Y_AQUA_PRINTLN("Cstp = "<<Cstp);

                if(B0<=0)
                {
                    Y_AQUA_PRINTLN("#already!");
                    return true;
                }
                else
                {
                    if(!rescale())
                    {
                        return false; //!blocked
                    }
                    else
                    {

                        return false;
                    }
                }
            }

        }



#if 0
        double Solver:: B_only(const Array &C) throw()
        {
            static const double c2min = numeric<double>::tiny;

            for(size_t j=M;j>0;--j)
            {
                Caux[j]  = 0;
                const double Cj = C[j];
                if(Cj<0)
                {
                    const double Cj2 = Cj*Cj;
                    if(Cj2>c2min)
                    {
                        Caux[j] = Cj2;
                    }
                }

            }
            return sumCaux();
        }

        double Solver:: B_drvs(const Array &C) throw()
        {
            static const double c2min = numeric<double>::tiny;

            for(size_t j=M;j>0;--j)
            {
                Caux[j] = 0;
                Ctry[j] = 0;
                const double Cj = C[j];
                if(Cj<0)
                {
                    const double Cj2 = Cj*Cj;
                    if(Cj2>c2min)
                    {
                        Caux[j] = Cj2;
                        Ctry[j] = -Cj;
                    }
                }
            }

            Y_AQUA_PRINTLN("Ctry = "<<Ctry);
            quark::mul(xi,Nu,Ctry);
            for(size_t i=N;i>0;--i) xi[i] /= nu2[i];
            quark::mul(Cstp,tNu,xi);
            Y_AQUA_PRINTLN("Cstp = "<<Cstp);

            return sumCaux();
        }
#endif



#if 0
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

                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                double B0 = B_drvs(Corg);
                Y_AQUA_PRINTLN("Corg = "<<Corg);
                Y_AQUA_PRINTLN("B0   = "<<B0 << "/" << B_only(Corg));
                if(B0<=0)
                {
                    Y_AQUA_PRINTLN("#already!");
                    return true;
                }
                else
                {
                    B_proxy F = { this };

                CYCLE:
                    //----------------------------------------------------------
                    // at this point, B0>0 and Cstp are computed
                    //----------------------------------------------------------
                    double x1 = 1;
                    double B1 = F(x1);
                    Y_AQUA_PRINTLN("B1   = "<<B1);

                    if(false)
                    {
                        ios::ocstream fp("balance.dat");
                        for(double x=0;x<=3.0;x+=0.01)
                        {
                            fp("%g %g\n",x,F(x));
                        }
                    }

                    if(B1>=B0)
                    {
                        Y_AQUA_PRINTLN("#shrink");
                        triplet<double> x = {0,1,1};
                        triplet<double> B = {B0,B1,B1};
                        B1 = F( x1=minimize::run(F,x,B,minimize::inside) );
                    }

                    Y_AQUA_PRINTLN("B1   = "<<B1 << " @ " << x1);

                    //----------------------------------------------------------
                    // backtracking
                    //----------------------------------------------------------
                    {
                        double x0 = 0;
                        do
                        {
                            const double xm = (x0+x1)*0.5;
                            const double Bm = F(xm);
                            if(Bm>B1)
                            {
                                x0=xm;
                            }
                            else
                            {
                                x1=xm;
                            }
                        }
                        while(fabs(x1-x0)>1e-4);
                    }
                    B1 = F(x1);
                    Y_AQUA_PRINTLN("B1   = "<<B1 << " @ " << x1);

                    if(B1<=0)
                    {
                        //------------------------------------------------------
                        // success!
                        //------------------------------------------------------
                        Y_AQUA_PRINTLN("success!");
                        goto SUCCESS;
                    }
                    else
                    {
                        //------------------------------------------------------
                        // check convergence
                        //------------------------------------------------------
                        quark::set(Corg,Ctry);
                        const double dB   = fabs(B1-B0);
                        const bool   bcvg = dB <= numeric<double>::ftol * B0;
                        Y_AQUA_PRINTLN("dB=" << dB << " | converged: " << bcvg);
                        if(bcvg)
                        {
                            goto CONVERGED;
                        }
                        B0 = B_drvs(Corg);
                        Y_AQUA_PRINTLN("Corg = "<<Corg);
                        Y_AQUA_PRINTLN("B0   = "<<B0);
                        goto CYCLE;
                    }


                }



            SUCCESS:
                for(size_t j=M;j>0;--j)
                {
                    if(used[j])
                    {
                        C[j] = max_of(0.0,Corg[j]);
                    }
                }
                return true;

            CONVERGED:
                Y_AQUA_PRINTLN("converged!");
                // find the maximum active concentration
                double Cmax = 0;
                size_t na   = 0;
                for(size_t j=M;j>0;--j)
                {
                    if(used[j])
                    {
                        ++na;
                        const double AC = fabs(Corg[j]);
                        if(AC>Cmax) Cmax = AC;
                    }
                }

                // compute the numeric tolerance
                const double Ctol = Cmax * na * numeric<double>::ftol;
                Y_AQUA_PRINTLN("Ctol = " << Ctol);

                // cut at this tolerance
                for(size_t j=M;j>0;--j)
                {
                    if(!used[j])
                        continue;

                    const double Cj = Corg[j];
                    if(Cj<0)
                    {
                        if(fabs(Cj)<=Ctol)
                        {
                            Corg[j] = 0;
                        }
                        else
                        {
                            Y_AQUA_PRINTLN("unable");
                            return false;
                        }
                    }
                }

                for(size_t j=M;j>0;--j)
                {
                    if(used[j])
                    {
                        C[j] = Corg[j];
                    }
                }
                Y_AQUA_PRINTLN("C_ok = " << Corg);
                return true;
            }
        }

#endif
        

    }
    
}
