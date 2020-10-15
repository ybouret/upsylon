
#include "y/aqua/engine.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
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

        static const char PFX[] = "[forward] ";

#define Y_AQUA_PRINTLN(MSG) do { if(forwardVerbose) { std::cerr << PFX << MSG << std::endl; } } while(false)

        void Engine:: computeK(const double t)
        {
            for(size_t i=N;i>0;--i)
            {
                K[i] = equilibria[i]->K(t);
            }
        }

        void Engine:: updateKs() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                const Equilibrium &eq = *equilibria[i];
                double             C0 = 1;
                if(eq.dn!=0)
                {
                    C0 = pow(K[i],eq.idn);
                }
                Ks[i] = C0;
            }
        }

        void Engine:: computeQ(const accessible<double> &C) throw()
        {
            for(size_t i=N;i>0;--i)
            {
                Q[i] = equilibria[i]->computeQ(K[i],C);
            }
        }

        void Engine:: computeJ(const accessible<double> &C) throw()
        {
            for(size_t i=N;i>0;--i)
            {
                Q[i] = equilibria[i]->computeQ(J[i], K[i], C);
            }
        }


        double Engine:: QValue() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                Caux[i] = square_of(Q[i]);
            }
            return sumCaux(N);
        }

        double Engine:: QCheck(const double x) throw()
        {
            tao::muladd(Ctry,Cini,x,step);
            for(size_t j=M;j>0;--j)
            {
                Ctry[j] = Cini[j]+x*step[j];
            }
            computeQ(Ctry);
            return QValue();
        }


        double Engine::QProxy:: operator()(const double x) throw()
        {
            assert(self);
            return self->QCheck(x);
        }


        bool Engine:: forward(addressable<double> &C) throw()
        {
            //------------------------------------------------------------------
            //
            //
            // initialize, concentration must be valid
            //
            //
            //------------------------------------------------------------------
            forwardCycles = 0;
            totalBalances = 0;
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    Cini[j] = C[j]; assert(C[j]>=0);
                }
                else
                {
                    Cini[j] = 0;
                }
            }
            Y_AQUA_PRINTLN("Cini = "<<Cini);
            bool   alreadySwept = false;
            QProxy F            = { this };
            balanceCycles       = 0;

            //------------------------------------------------------------------
            //
            //
            // start cycles
            //
            //
            //------------------------------------------------------------------
        CYCLE:
            ++forwardCycles;
            Y_AQUA_PRINTLN("# << forward cycle " << forwardCycles << " | balances=" << totalBalances << " (+" << balanceCycles << ") >>");

            //------------------------------------------------------------------
            //
            // compute local Jacobian
            //
            //------------------------------------------------------------------
            computeJ(Cini);
            tao::mmul_trn(W,J,Nu);
            if( !LU::build(W) )
            {
                Y_AQUA_PRINTLN("singular system level-1");
                if(alreadySwept || !sweep(Cini) )
                {
                    Y_AQUA_PRINTLN("singular system level-2");
                    return false;
                }
                else
                {
                    alreadySwept = true;
                    goto CYCLE;
                }
            }

            alreadySwept = false;

            //------------------------------------------------------------------
            //
            // compute Newton's step
            //
            //------------------------------------------------------------------
            tao::neg(xi,Q);
            LU::solve(W,xi);
            tao::mul(step,tNu,xi);
            Y_AQUA_PRINTLN("Q    = "<<Q);
            Y_AQUA_PRINTLN("xi   = "<<xi);
            Y_AQUA_PRINTLN("step = "<<step);

            //------------------------------------------------------------------
            //
            // update to balanced position
            //
            //------------------------------------------------------------------
            tao::add(Cend,Cini,step);
            Y_AQUA_PRINTLN("Cend = "<<Cend);
            const bool balanced = balance(Cend);
            totalBalances += balanceCycles;
            if(!balanced)
            {
                Y_AQUA_PRINTLN("singular system balance");
                return false;
            }

            if(balanceCycles>0)
            {
                Y_AQUA_PRINTLN("balanced = " << balanceCycles);
                Y_AQUA_PRINTLN("Cend     = " << Cend);
                if(!sweep(Cend))
                {
                    Y_AQUA_PRINTLN("singular balanced system");
                    return false;
                }
                Y_AQUA_PRINTLN("Cend     = " << Cend);
                tao::set(Cini,Cend);
                goto CYCLE;
            }

            tao::sub(step,Cend,Cini);
            Y_AQUA_PRINTLN("step = "<<step);




            //------------------------------------------------------------------
            //
            // minimize Q to avoir oscillations
            //
            //------------------------------------------------------------------
            const double    Q0 = QValue();
            double          x1 = 1;
            double          Q1 = F(x1);
            Y_AQUA_PRINTLN("Q0=" <<Q0 << "; Q1="<<Q1);
            if(Q1>=Q0)
            {
                triplet<double> x  = { 0,x1,x1 };
                triplet<double> q  = { Q0,Q1,Q1};
                if(Q1>=Q0)
                {
                    Q1 = F( x1 = minimize::run(F,x,q,minimize::inside) );
                    Y_AQUA_PRINTLN("#backtrack @" << x1);
                }
                tao::set(Cend,Ctry);
            }
            

            //------------------------------------------------------------------
            //
            // check convergence
            //
            //------------------------------------------------------------------
            const bool converged = __find<double>::convergence(Cini,Cend);
            if(converged)
            {
                Y_AQUA_PRINTLN("#converged");
                for(size_t j=M;j>0;--j)
                {
                    C[j] = Cini[j];
                }
                return true;
            }

            goto CYCLE;
        }


    }

}


