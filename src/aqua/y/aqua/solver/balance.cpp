#include "y/aqua/solver.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sort/heap.hpp"
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
                assert(Caux[j]>=0);
                sum += Caux[j];
            }
            return sum;
        }

        

        double Solver:: B_only(Array &C) throw()
        {
            static const double Cmin = ctiny;
            for(size_t j=M;j>0;--j)
            {
                Caux[j] = 0;
                const double Cj = -C[j];
                if(Cj>0)
                {
                    if(Cj<=Cmin)
                    {
                        C[j] = 0;
                    }
                    else
                    {
                        Caux[j] = Cj;
                    }
                }
            }
            return sumCaux();
        }

        double Solver:: B_drvs(Array &C, const iMatrix &Bspace, const iMatrix &Btrans) throw()
        {
            static const double Cmin = ctiny;
            for(size_t j=M;j>0;--j)
            {
                Caux[j] = 0;
                Ctry[j] = 0;
                const double Cj = -C[j];
                if(Cj>0)
                {
                    if(Cj<=Cmin)
                    {
                        C[j] = 0;
                    }
                    else
                    {
                        Caux[j]  = Cj;
                        Ctry[j]  = 1;
                    }
                }
            }

            quark::mul(xi,Bspace,Ctry);
            quark::mul(Cstp,Btrans,xi);


            return sumCaux();
        }

        bool Solver:: rescale(const double B0) throw()
        {
            //------------------------------------------------------------------
            // compute step scaling
            //------------------------------------------------------------------
            static const double Cmin = ctiny;
            const double S2 = quark::mod2<double>::of(Cstp);

            Y_AQUA_PRINTLN("S2   = "<<S2);

            if(S2<=0)
            {
                Y_AQUA_PRINTLN("# blocked!");
                return false;
            }

            //------------------------------------------------------------------
            // compute concentrations scaling
            //------------------------------------------------------------------
            const double fac  = fabs(B0)/sqrt(S2);
            Y_AQUA_PRINTLN("fac  = "<<fac);
            quark::rescale(Cstp,fac);
            for(size_t j=M;j>0;--j)
            {
                if(fabs(Cstp[j])<=Cmin) Cstp[j] = 0;
            }
            return true;
        }



        bool Solver:: balance(addressable<double> &C) throw()
        {
            return balance(C,Nu,tNu);
        }

        bool Solver:: balance(addressable<double> &C,
                              const iMatrix       &Bspace,
                              const iMatrix       &Btrans) throw()
        {
            static const double Cmin = ctiny;
            assert(C.size()>=M);
            lastBalanceCycles=0;
            if(N<=0)
            {
                //--------------------------------------------------------------
                //
                //
                // trivial case
                //
                //
                //--------------------------------------------------------------
                Y_AQUA_PRINTLN("no equilibrium");
                return true;
            }
            else
            {
                //--------------------------------------------------------------
                //
                //
                // setup: copy active values
                //
                //
                //--------------------------------------------------------------
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

                //--------------------------------------------------------------
                //
                //
                // initialize excess of negative concentrations+unscaled Cstp
                //
                //
                //--------------------------------------------------------------
                double B0 = B_drvs(Corg,Bspace,Btrans);
                Y_AQUA_PRINTLN("Corg = "<<Corg);
                Y_AQUA_PRINTLN("B0   = "<<B0);
                Y_AQUA_PRINTLN("G0   = "<<Ctry);
                Y_AQUA_PRINTLN("Cstp = "<<Cstp);

                if(B0<=0)
                {
                    //----------------------------------------------------------
                    //
                    //
                    // early return
                    //
                    //
                    //----------------------------------------------------------
                    Y_AQUA_PRINTLN("#already!");
                    return true;
                }
                else
                {
                    typedef  triplet<double>  Triplet;
                    B_proxy F = { this };
                    //----------------------------------------------------------
                    //
                    //
                    // at this point: B0>0 and unscaled step are computed
                    //
                    //
                    //----------------------------------------------------------
                CYCLE:
                    ++lastBalanceCycles;
                    Y_AQUA_PRINTLN("#\t<cycle " << lastBalanceCycles << " >");
                    if(!rescale(B0))
                    {
                        //------------------------------------------------------
                        //
                        // realy blocked
                        //
                        //------------------------------------------------------
                        return false;
                    }
                    else
                    {
                        //------------------------------------------------------
                        //
                        // probe
                        //
                        //------------------------------------------------------
                        Y_AQUA_PRINTLN("Corg = "<<Corg);
                        Y_AQUA_PRINTLN("Cstp = "<<Cstp);

                        double x1 = 1;
                        double B1 = F(x1);
                        Y_AQUA_PRINTLN("B1   = "<<B1);

                        if(B1>=B0)
                        {
                            //--------------------------------------------------
                            //
                            Y_AQUA_PRINTLN("#shrink");
                            //
                            //--------------------------------------------------
                            Triplet x = {0,x1,x1};
                            Triplet B = {B0,B1,B1};
                            B1 = F( x1 = minimize::run(F,x,B,minimize::inside) );
                        }
                        else
                        {
                            //--------------------------------------------------
                            //
                            Y_AQUA_PRINTLN("#expand");
                            //
                            //--------------------------------------------------
                            if(B1>0)
                            {
                                Triplet x = {0,x1,x1*numeric<double>::gold};
                                Triplet B = {B0,B1,F(x.c)};
                                if(B.c>=B.b)
                                {
                                    //----------------------------------------------
                                    Y_AQUA_PRINTLN("#found optimum");
                                    //----------------------------------------------
                                    B1 = F( x1 = minimize::run(F,x,B,minimize::direct) );
                                }
                                else
                                {
                                    //----------------------------------------------
                                    Y_AQUA_PRINTLN("#forward");
                                    //----------------------------------------------
                                    B1 = B.c; // and Ctry is computed
                                    x1 = x.c; // at this value
                                }
                            }
                        }
                        Y_AQUA_PRINTLN("B1   = "<<B1 << " # @" << x1);

                        if(B1<=0)
                        {
                            //--------------------------------------------------
                            //
                            // backward
                            //
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
                            //
                            // test convergence and update for next cycle
                            //
                            //--------------------------------------------------
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
                            const double dB   = fabs(B1-B0);
                            const bool   Bcvg = (dB <= numeric<double>::ftol * max_of(B1,B0));
                            Y_AQUA_PRINTLN("deltaB      = "<<dB);
                            Y_AQUA_PRINTLN("#convergence: C:" << Ccvg << " B:" << Bcvg);
                            if(Bcvg||Ccvg)
                            {
                                goto CONVERGED;
                            }
                            B0 = B_drvs(Corg,Bspace,Btrans);
                            goto CYCLE;
                        }
                        return false;

                    SUCCESS:
                        assert(B0<=0);
                        Y_AQUA_PRINTLN("success   @ " << Corg);
                        for(size_t j=M;j>0;--j)
                        {
                            if(active[j])
                            {
                                assert(Corg[j]>=0);
                                double Cj = Corg[j];
                                if(fabs(Cj)<=Cmin)
                                {
                                    Cj = 0;
                                }
                                C[j] = Cj;
                            }
                        }
                        return true;

                    CONVERGED:
                        Y_AQUA_PRINTLN("converged @ " << Corg);
                        //------------------------------------------------------
                        // compute ctol = epsilon * A * max|Corg|
                        //------------------------------------------------------
                        double ctol = 0;
                        for(size_t j=M;j>0;--j)
                        {
                            ctol = max_of(ctol,fabs(Corg[j]));
                        }
                        ctol *= numeric<double>::epsilon;
                        ctol *= A;

                        //------------------------------------------------------
                        // check that negative values are garbage
                        //------------------------------------------------------
                        for(size_t j=M;j>0;--j)
                        {
                            const double Cj = Corg[j];
                            if(Cj<0)
                            {
                                if(fabs(Cj)>ctol)
                                {
                                    // not a garbage...
                                    return false;
                                }
                                Corg[j]=0;
                            }
                        }
                        B0=0;
                        goto SUCCESS;
                    }
                }
            }

        }







    }
    
}
