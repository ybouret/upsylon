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


        double Solver:: maxCaux() const throw()
        {
            double ans = 0;
            for(size_t j=M;j>0;--j)
            {
                ans = max_of(ans,Caux[j]);
            }
            return ans;
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


            double CC = 0;
            {
                size_t j=M;
                while(j>0)
                {
                    const double Cj = Corg[j--];
                    if(Cj<0)
                    {
                        CC = -Cj;
                        break;
                    }
                }

                while(j>0)
                {
                    const double Cj = Corg[j--];
                    if(Cj<0)
                    {
                        CC = min_of(-Cj,CC);
                    }
                }
            }
            const double fac  = CC/sqrt(S2);
            Y_AQUA_PRINTLN("CC   = "<<CC);
            Y_AQUA_PRINTLN("fac  = "<<fac);
            quark::rescale(Cstp,fac);
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
                Y_AQUA_PRINTLN("B0   = "<<B0);
                Y_AQUA_PRINTLN("G0   = "<<Ctry);
                Y_AQUA_PRINTLN("Cstp = "<<Cstp);

                if(B0<=0)
                {
                    //----------------------------------------------------------
                    // early return
                    //----------------------------------------------------------
                    Y_AQUA_PRINTLN("#already!");
                    return true;
                }
                else
                {
                    typedef  triplet<double>  Triplet;
                    B_proxy F = { this };
                    size_t  cycle = 0;
                    //----------------------------------------------------------
                    // at this point: B0 and unscaled step are computed
                    //----------------------------------------------------------
                CYCLE:
                    ++cycle;
                    Y_AQUA_PRINTLN(" #\t<cycle " << cycle << " >");
                    if(!rescale())
                    {
                        //------------------------------------------------------
                        // realy blocked
                        //------------------------------------------------------
                        return false; //!blocked
                    }
                    else
                    {
                        //------------------------------------------------------
                        // probe
                        //------------------------------------------------------
                        Y_AQUA_PRINTLN("Corg = "<<Corg);
                        Y_AQUA_PRINTLN("Cstp = "<<Cstp);

                        if(true)
                        {
                            ios::ocstream fp("balance.dat");
                            for(double x=0;x<=3.0;x+=0.01)
                            {
                                fp("%.20g %.20g\n",x,F(x));
                            }
                        }

                        double x1 = 1;
                        double B1 = F(x1);
                        Y_AQUA_PRINTLN("B1   = "<<B1);

                        if(B1>=B0)
                        {
                            Y_AQUA_PRINTLN("#shrink");
                            Triplet x = {0,x1,x1};
                            Triplet B = {B0,B1,B1};
                            B1 = F( x1 = minimize::run(F,x,B,minimize::inside) );
                        }
                        else
                        {
                            // B1 < B0
                            Y_AQUA_PRINTLN("#expand");
                            Triplet x = {0,x1,x1*numeric<double>::gold};
                            Triplet B = {B0,B1,F(x.c)};
                            if(B.c>=B.b)
                            {
                                Y_AQUA_PRINTLN("#found optimum");
                                B1 = F( x1 = minimize::run(F,x,B,minimize::direct) );
                            }
                            else
                            {
                                Y_AQUA_PRINTLN("#forward");
                                B1 = B.c; //!< and Ctry is computed
                                x1 = x.c; //!< at this value
                            }
                        }
                        Y_AQUA_PRINTLN("B1   = "<<B1 << " # @" << x1);

                        if(B1<=0)
                        {
                            //--------------------------------------------------
                            // backward
                            //--------------------------------------------------
                            {
                                Y_AQUA_PRINTLN("#backtrack");
                                double x0 = 0;
                                do {
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
                                } while(fabs(x1-x0)>1e-4);
                            }
                            B1 = F(x1); assert(B1<=0);
                            Y_AQUA_PRINTLN("B1   = "<<B1 << " # @" << x1);
                            quark::set(Corg,Ctry);
                            B0 = 0;
                            goto SUCCESS;
                        }
                        else
                        {
                            //--------------------------------------------------
                            // test convergence and update for next cycle
                            //--------------------------------------------------
                            Y_AQUA_PRINTLN("#testing");
                            for(size_t j=M;j>0;--j)
                            {
                                const double old = Corg[j];
                                const double now = Ctry[j];
                                const double err = fabs(old-now);
                                Corg[j] = now;
                            }
                            const double dB   = fabs(B1-B0);
                            const bool   Bcvg = (dB <= numeric<double>::ftol * max_of(B1,B0));
                            Y_AQUA_PRINTLN("dB   = "<<dB << " #cvg : " << Bcvg);
                            B0 = B_drvs(Corg);
                            goto CYCLE;
                        }
                        return false;

                    SUCCESS:
                        assert(B0<=0);
                        Y_AQUA_PRINTLN("success @ " << Corg);
                        for(size_t j=M;j>0;--j)
                        {
                            if(used[j])
                            {
                                C[j] = Corg[j];
                            }
                        }
                        return true;
                    }
                }
            }

        }







    }
    
}
