#include "y/aqua/solver.hpp"
#include "y/mkl/kernel/quark.hpp"
#include "y/sort/heap.hpp"
#include "y/ios/ocstream.hpp"
#include "y/mkl/opt/bracket.hpp"
#include "y/mkl/opt/minimize.hpp"

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
                if(used[i]&&Ci<0)
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
                if(used[i]&&Ci<0)
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
            static const double etol = numeric<double>::ftol;

            if(N<=0) return true;

            for(size_t j=M;j>0;--j)
            {
                Corg[j] = C[j];
            }
            std::cerr << "C   =" << C << std::endl;
            std::cerr << "Corg=" << Corg << std::endl;

            // initialize step
            double E0 = ob_step(Corg);
            std::cerr << "E0=" << E0 << std::endl;
            if(E0<=0) goto SUCCESS; // all good!

            while(true)
            {
                assert(E0>0);
                std::cerr << "Cstp=" << Cstp << std::endl;
                {
                    ios::ocstream fp("balance.dat");
                    for(double x=0;x<=2;x += 0.01)
                    {
                        fp("%g %g\n",x,ob_call(x));
                    }
                }

                double          E1 = ob_call(1.0);
                std::cerr << "E1=" << E1 << "@" << 1 << std::endl;
                if(E1<=0)
                {
                    E0 = E1;
                    goto SUCCESS; // early return
                }


                triplet<double> x = { 0,  1,  1  };
                triplet<double> E = { E0, E1, E1 };
                BalanceProxy    F = { this };

                if(E1<E0)
                {
                    std::cerr << "expand" << std::endl;
                    do { x.c += 1.0; } while( (E.c=F(x.c)) <= E1 );
                    E1=F( x.b=minimize::run(F, x, E, minimize::direct) );
                }
                else
                {
                    std::cerr << "bracket" << std::endl;
                    E1=F( x.b=minimize::run(F, x, E, minimize::inside) );
                }
                std::cerr << "E1=" << E1 << "@" << x.b << std::endl;
                std::cerr << "Ctry=" << Ctry << std::endl;
                // check


                // update
                quark::set(Corg,Ctry);
                if(E1<=0)
                {
                    E0 = 0;
                    goto SUCCESS;
                }

                if( fabs(E0-E1) <= etol * fabs(E0) )
                {
                    std::cerr << "converged" << std::endl;
                    exit(1);
                }

                // ready for next loop
                E0 = ob_step(Corg);
            }


        SUCCESS:
            assert(E0<=0);
            for(size_t j=M;j>0;--j)
            {
                if(used[j])
                {
                    C[j] = max_of(Corg[j],0.0);
                }
            }
            return true;
        }

    }

}
