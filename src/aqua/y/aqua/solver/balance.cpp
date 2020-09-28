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
        
        double Solver:: ob_fast( const Array &C ) const throw()
        {
            assert(C.size()==M);
            for(size_t i=M;i>0;--i)
            {
                const double Ci = C[i];
                if(Ci<0)
                {
                    Caux[i] = square_of(Ci);
                }
                else
                {
                    Caux[i] = 0;
                }
            }
            hsort(Caux,comparison::decreasing<double>);
            double sum = 0;
            for(size_t i=M;i>0;--i)
            {
                sum += Caux[i];
            }
            return sum*0.5;
        }
        
        double Solver:: ob_step(const Array &C) const throw()
        {
            assert(C.size()==M);
            
            // compute terms and raw step
            for(size_t i=M;i>0;--i)
            {
                const double Ci = C[i];
                if(Ci<0)
                {
                    Caux[i] = square_of(Ci);
                    Cstp[i] = -Ci;
                }
                else
                {
                    Caux[i] = 0;
                    Cstp[i] = 0;
                }
            }
            
            // compute chemical step
            quark::mul(xi,Nu,Cstp);
            quark::mul(Cstp,tNu,xi);
            
            // evaluate objective function
            hsort(Caux,comparison::decreasing<double>);
            double sum = 0;
            for(size_t i=M;i>0;--i)
            {
                sum += Caux[i];
            }
            return sum*0.5;
        }
        
        double Solver:: ob_call(const double x) const throw()
        {
            quark::muladd(Ctry,Corg,x,Cstp);
            return ob_fast(Ctry);
        }
        
        double Solver:: BalanceProxy:: operator()(const double x)   throw()
        {
            assert(self);
            return self->ob_call(x);
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
                double E0 = ob_step(Corg);
                std::cerr << "E0="  << E0   << std::endl;
                std::cerr << "C ="  << C << std::endl;
                std::cerr << "Corg=" << Corg << std::endl;
                std::cerr << "Cstp=" << Cstp << std::endl;
                if(E0<=0)
                {
                    //----------------------------------------------------------
                    // everyone is ok
                    //----------------------------------------------------------
                    goto SUCCESS;
                }
                else
                {
                    assert(E0>0);
                    //----------------------------------------------------------
                    // at this point, E0>0 and Cstp is computed
                    //----------------------------------------------------------
                    BalanceProxy    EE = { this };
                CYCLE:
                    double          E1 = EE(1);
                    triplet<double> x  = { 0,   1,  1 };
                    triplet<double> E  = { E0, E1, E1 };
                    std::cerr << "Corg=" << Corg << std::endl;
                    std::cerr << "Cstp=" << Cstp << std::endl;
                    if(E1<E0)
                    {
                        //------------------------------------------------------
                        // expand
                        //------------------------------------------------------
                        do { x.c += 1.0; } while( (E.c=EE(x.c)) < E1 );
                        E1 = EE( x.b = minimize::run(EE,x,E,minimize::direct));
                    }
                    else
                    {
                        //------------------------------------------------------
                        // shrink
                        //------------------------------------------------------
                        E1 = EE( x.b = minimize::run(EE,x,E,minimize::inside));
                    }
                    std::cerr << "E1=" << E1 << "@" << x.b << std::endl;
                    //----------------------------------------------------------
                    // check convergence
                    //----------------------------------------------------------
                    if(E1<=0)
                    {
                        
                        quark::set(Corg,Ctry);
                        goto SUCCESS;
                    }
                    else
                    {
                        const double dE    = fabs(E1-E0);
                        const bool   E_cvg = (dE+dE) <= numeric<double>::ftol * (E1+E0);
                        const bool   C_cvg = __find<double>::convergence(Corg,Ctry);
                        std::cerr << "cvg: E: " << E_cvg << " C: " << C_cvg << std::endl;
                        E0 = ob_step(Corg);
                        if(E_cvg||C_cvg)
                        {
                            exit(1);
                        }
                        goto CYCLE;
                    }
                    
                }
                
            SUCCESS:
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
