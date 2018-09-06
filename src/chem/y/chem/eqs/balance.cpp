
#include "y/chem/equilibria.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/math/opt/bracket.hpp"
#include "y/math/opt/minimize.hpp"
#include "y/math/utils.hpp"

namespace upsylon
{
    using namespace math;

    namespace Chemical
    {
        double Equilibria:: computeExcess(array<double> &C)
        {
            static const double ctol = numeric<double>::tiny;
            beta2.free();
            for(size_t j=M;j>0;--j)
            {
                beta[j] = 0;
                const double Cj = C[j];
                if(active[j]&&Cj<0)
                {
                    const double Cj2 = Cj*Cj;
                    if(Cj2<=ctol)
                    {
                        C[j] = 0;
                    }
                    else
                    {
                        beta[j] = -Cj;
                        beta2.push_back_(Cj2);
                    }
                }
            }
            return sorted_sum(beta2);
        }


        namespace
        {
            struct __balance
            {
                Equilibria          * _cs;
                const array<double> * _Cini;
                array<double>       * _Ctry;
                const array<double> * _dC;

                inline double operator()( const double alpha )
                {
                    assert(_cs); assert(_Cini); assert(_Ctry); assert(_dC);
                    tao::setprobe(* _Ctry, * _Cini, alpha, *_dC);
                    return _cs->computeExcess(*_Ctry);
                }

            };
        }

        bool Equilibria:: balance(array<double> &C)
        {
            static const double _end = 1;

            //__________________________________________________________________
            //
            // initialize search
            //__________________________________________________________________
            for(size_t j=M;j>0;--j)
            {
                Cini[j] = C[j];
            }

            __balance E = { this, &Cini, &Ctry, &dC  };

            double Eini = computeExcess(Cini);
            while( Eini > 0 )
            {
                //std::cerr << "Cini  = " << Cini << std::endl;
                //std::cerr << "beta  = " << beta << std::endl;
                std::cerr << "Eini  = " << Eini << "/" << E(0) << std::endl;

                //______________________________________________________________
                //
                // compute E descent, normalized/nu
                //______________________________________________________________
                for(size_t i=N;i>0;--i)
                {
                    xi[i] = tao::_dot<double>(Nu[i],beta)/nu2[i];
                }
                tao::mul(dC,tNu,xi);
                triplet<double> aa   = {0,0,0};
                triplet<double> EE   = {Eini,Eini,Eini};
                const double    Eend = E(_end);

                //______________________________________________________________
                //
                // locate minimum
                //______________________________________________________________
                if(Eend<Eini)
                {
                    // expand
                    aa.b=_end; EE.b = Eend;
                    bracket::expand(E,aa,EE);
                }
                else
                {
                    // backtrack
                    aa.c=_end; EE.c = Eend;
                    bracket::inside(E,aa,EE);
                }
                //______________________________________________________________
                //
                // minimize accordingly
                //______________________________________________________________
                const double alpha = max_of<double>(0,minimize::run(E,aa,EE));
                const double Etry  = E(alpha);
                if(Etry>=Eini)
                {
                    break; // minimum is reached
                }
                Eini = Etry;
                tao::_set(Cini,Ctry);
            }

            //__________________________________________________________________
            //
            // check values
            //__________________________________________________________________
            std::cerr << "Cini=" << Cini << std::endl;
            double Cmax = 0;
            size_t na   = 0;
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    ++na;
                    const double AC = fabs(Cini[j]);
                    if(AC>Cmax) Cmax = AC;
                }
            }
            const double Ctol = Cmax * na * numeric<double>::ftol;
            
            std::cerr << "Cmax=" << Cmax << std::endl;
            std::cerr << "na  =" << na << std::endl;
            std::cerr << "Ctol=" << Ctol << std::endl;
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    const double CC = Cini[j];
                    if(fabs(CC)<=Ctol)
                    {
                        Cini[j] = 0;
                    }
                    else
                    {
                        if(CC<0)
                        {
                            std::cerr << "unable to find valid C#" << j << std::endl;
                            return false;
                        }
                    }
                }
            }
            for(size_t j=M;j>0;--j)
            {
                C[j] = Cini[j];
            }
            return true;

        }
    }

}

