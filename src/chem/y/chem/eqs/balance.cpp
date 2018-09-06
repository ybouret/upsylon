
#include "y/chem/equilibria.hpp"
#include "y/math/kernel/tao.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/math/opt/bracket.hpp"
#include "y/math/opt/minimize.hpp"

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

            // initialize
            for(size_t j=M;j>0;--j)
            {
                Cini[j] = C[j];
            }

            __balance E = { this, &Cini, &Ctry, &dC  };

            double Eini = computeExcess(Cini);
            while( Eini > 0 )
            {
                std::cerr << "Cini  = " << Cini << std::endl;
                std::cerr << "beta  = " << beta << std::endl;
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
                std::cerr << "xi    = " << xi << std::endl;
                std::cerr << "dC    = " << dC << std::endl;
                triplet<double> aa   = {0,0,0};
                triplet<double> EE   = {Eini,Eini,Eini};
                const double    Eend = E(_end);
                if(Eend<Eini)
                {
                    // expand
                    std::cerr << "|_expand search" << std::endl;
                    aa.b=_end; EE.b = Eend;
                    bracket::expand(E,aa,EE);
                }
                else
                {
                    // backtrack
                    std::cerr << "|_backtrack search" << std::endl;
                    aa.c=_end; EE.c = Eend;
                    bracket::inside(E,aa,EE);
                }
                //std::cerr << "alpha=" << aa << ", E=" << EE << std::endl;
                const double alpha = max_of<double>(0,minimize::run(E,aa,EE));
                const double Etry  = E(alpha);
                std::cerr << "alpha=" << alpha << std::endl;
                std::cerr << "Etry =" << Etry  << "@" << Ctry << std::endl;
                if(Etry>=Eini)
                {
                    break;
                }
                Eini = Etry;
                tao::_set(Cini,Ctry);
            }

            if(Eini<=0)
            {
                std::cerr << "Numerically NULL" << std::endl;
                for(size_t j=M;j>0;--j)
                {
                    if(active[j])
                    {
                        C[j] = max_of<double>(0,Cini[j]);
                    }
                }
                return true;
            }
            else
            {
                return false;
            }

        }
    }

}

