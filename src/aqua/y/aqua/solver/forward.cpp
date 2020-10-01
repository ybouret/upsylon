
#include "y/aqua/solver.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/minimize.hpp"
#include "y/mkl/utils.hpp"

namespace upsylon
{

    namespace Aqua
    {
        using namespace mkl;


        double Solver:: Q_only(const Array &C) throw()
        {
            computeQ(C);
            return quark::mod2<double>::of(Q);
        }

        double Solver:: Q_call(const double x) throw()
        {
            for(size_t j=M;j>0;--j)
            {
                Ctry[j] = max_of(0.0,Cini[j]+x*Cstp[j]);
            }
            return Q_only(Ctry);
        }

        double Solver:: Q_proxy ::operator()(const double x) throw()
        {
            assert(self);
            return self->Q_call(x);
        }


        static const char fn[] = "[forward] ";
#define Y_AQUA_PRINTLN(MSG) do { if(forwardVerbose) { std::cerr << fn << MSG << std::endl; } } while(false)

        bool Solver:: forward(addressable<double> &C, size_t &cycles) throw()
        {
            assert(C.size()>=M);

            //------------------------------------------------------------------
            //
            //
            // initialize Cfwd with active concentrations
            //
            //
            //------------------------------------------------------------------
            cycles = 0;
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    assert(C[j]>=0);
                    Cini[j] = C[j];
                }
                else
                {
                    Cini[j] = 0;
                }
            }

            Q_proxy F  = { this };


            //------------------------------------------------------------------
            //
            //
            // cycles
            //
            //
            //------------------------------------------------------------------
            while(true)
            {
                ++cycles;
                //--------------------------------------------------------------
                //
                //
                // compute status: Q and Phi
                //
                //
                //--------------------------------------------------------------
                Y_AQUA_PRINTLN("#\t<cycle " << cycles << ">");
                Y_AQUA_PRINTLN("Cini = "<<Cini);
                computeS(Cini);
                if(!computeW())
                {
                    Y_AQUA_PRINTLN("singular system!");
                    return false;
                }
                const double Q0 = Q_only(Cini);
                Y_AQUA_PRINTLN("Q    = " << Q);
                Y_AQUA_PRINTLN("Q0   = " << Q0);

                //--------------------------------------------------------------
                //
                //
                // compute extent
                //
                //
                //--------------------------------------------------------------
                quark::neg(xi,Q);
                LU::solve(W,xi);

                //--------------------------------------------------------------
                //
                //
                // compute new position
                //
                //
                //--------------------------------------------------------------
                for(size_t j=M;j>0;--j)
                {
                    Cend[j] = Cini[j] + quark::dot<double>::of(tNu[j],xi);
                }
                Y_AQUA_PRINTLN("Ctmp = "<<Cend);

                //--------------------------------------------------------------
                //
                //
                // balance new position
                //
                //
                //--------------------------------------------------------------
                size_t balanceCycles = 0;
                if(!balance(Cend,balanceCycles))
                {
                    Y_AQUA_PRINTLN("unable to balance");
                    return false;
                }
                Y_AQUA_PRINTLN("Cend = "<<Cend);


                //--------------------------------------------------------------
                //
                //
                // checking where we landed
                //
                //
                //--------------------------------------------------------------
                if(balanceCycles>0)
                {
                    //----------------------------------------------------------
                    //
                    // started from a very sensitive point
                    //
                    //----------------------------------------------------------
                    Y_AQUA_PRINTLN("# <<balanceCycles=" << balanceCycles << ">>");
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // more regular point: don't overshoot!
                    //
                    //----------------------------------------------------------
                    double x1 = 1;
                    double Q1 = Q_only(Cend);
                    if(Q1>=Q0)
                    {
                        //------------------------------------------------------
                        // prepare step to probe
                        //------------------------------------------------------
                        quark::sub(Cstp, Cend, Cini);

                        //------------------------------------------------------
                        // probe
                        //------------------------------------------------------
                        {
                            triplet<double> x  = { 0,  x1, x1 };
                            triplet<double> f  = { Q0, Q1, Q1};
                            Q1 = F( x1 = minimize::run(F,x,f,minimize::inside) );
                        }

                        //------------------------------------------------------
                        // update
                        //------------------------------------------------------
                        quark::set(Cend,Ctry);
                    }
                    Y_AQUA_PRINTLN("Q1    = " << Q1 << " @ " << x1 );

                    if(Q1<=0)
                    {
                        Y_AQUA_PRINTLN("converged Q=" << Q << " @ " << Cend);
                        break;
                    }
                }



                //--------------------------------------------------------------
                //
                //
                // check concentrations convergence
                //
                //
                //--------------------------------------------------------------
                bool converged = true;
                for(size_t j=M;j>0;--j)
                {
                    const double old = Cini[j]; assert(old>=0);
                    const double now = Cend[j]; assert(now>=0);
                    const double err = fabs(old-now);
                    if( err < numeric<double>::tiny )
                    {
                        //------------------------------------------------------
                        // don't move, keep Cini
                        //------------------------------------------------------
                    }
                    else
                    {
                        //------------------------------------------------------
                        // check status
                        //------------------------------------------------------
                        if(converged && (err > numeric<double>::ftol * max_of(old,now)) )
                        {
                            converged = false;
                        }
                        Cini[j] = now;
                    }
                }


                if(converged)
                {
                    Y_AQUA_PRINTLN("converged Q=" << Q << " @ " << Cend);
                    break;
                }
                //--------------------------------------------------------------
                //
                //
                // ready for next cycle, Cini=Cend
                //
                //
                //--------------------------------------------------------------
                
            }

            //------------------------------------------------------------------
            //
            //
            // update initial concentrations
            //
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    assert(Cend[j]>=0);
                    C[j] = Cend[j];
                }
            }
            return true;
        }

    }

}

