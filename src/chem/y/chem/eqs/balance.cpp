
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
#if 1
        double Equilibria:: computeExcess(array<double> &C)
        {
            beta2.free();
            for(size_t j=M;j>0;--j)
            {
                beta[j] = 0;
                const double Cj = C[j];
                if(active[j]&&Cj<0)
                {
                    const double Cj2 = Cj*Cj;
                    if(Cj2<=numeric<double>::minimum)
                    {
                        C[j] = 0;
                    }
                    else
                    {
                        beta[j] = -Cj;
                        beta2.push_back(Cj2);
                    }
                }
            }
            return sorted_sum(beta2);
        }
#else
        double Equilibria:: computeExcess(array<double> &C)
        {
            beta2.free();
            for(size_t j=M;j>0;--j)
            {
                beta[j] = 0;
                const double Cj = C[j];
                if(active[j]&&Cj<0)
                {
                    const double ACj = fabs(Cj);
                    if(ACj<=numeric<double>::minimum)
                    {
                        C[j] = 0;
                    }
                    else
                    {
                        beta[j] = 1;
                        beta2.push_back(ACj);
                    }
                }
            }
            return sorted_sum(beta2);
        }
#endif

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


                tao::mul(xi,Nu,beta);
                tao::mul(dC,tNu,xi);
                std::cerr << "xi    = " << xi << std::endl;
                std::cerr << "dC    = " << dC << std::endl;

                double alpha = 0;
                bool   init  = true;
                bal.free();
                for(size_t j=M;j>0;--j)
                {
                    const double Cj = Cini[j];
                    if(active[j]&&Cj<0)
                    {
                        const double Dj = dC[j];
                        if(Dj>0)
                        {
                            const double atemp = -Cj/Dj;
                            if(init)
                            {
                                init  = false;
                                alpha = atemp;
                                bal.push_back_(j);
                            }
                            else
                            {
                                if(atemp>=alpha)
                                {
                                    if(atemp>alpha)
                                    {
                                        bal.free();
                                        bal.push_back_(j);
                                        alpha = atemp;
                                    }
                                    else
                                    {
                                        bal.push_back_(j);
                                    }
                                }
                                // else do nothing
                            }
                            std::cerr << "Correcting  C[#" << j << "]=" << Cj << " with " << Dj << " -> " << atemp << std::endl;
                        }
                    }
                }
                if(init)
                {
                    std::cerr << "Unable to correct concentrations" << std::endl;
                    return false;
                }
                assert(alpha>0);
                std::cerr << "alpha=" << alpha << ", bal=" << bal << std::endl;
                double Etry = E(alpha);
                std::cerr << "Etry=" << Etry << "@Ctry=" << Ctry << std::endl;

                if( Etry <= 0 )
                {
                    // success!
                    for(size_t j=M;j>0;--j)
                    {
                        C[j] = Ctry[j];
                    }
                    for(size_t i=bal.size();i>0;--i) C[bal[i]] = 0;
                    return true;
                }
                else if( Etry < Eini )
                {
                    // forward
                    tao::_set(Cini,Ctry);
                    for(size_t i=bal.size();i>0;--i) Cini[bal[i]] = 0;
                    Eini = computeExcess(Cini);
                }
                else
                {
                    std::cerr << "Need to Backtrack!!!" << std::endl;
                    triplet<double> aa = { 0,0,alpha };
                    triplet<double> EE = { Eini,Eini,Etry };
                    bracket::inside(E,aa,EE);
                    alpha = minimize::run(E,aa,EE);
                    std::cerr << "alpha=" << alpha << std::endl;
                    Eini  = E(alpha);
                    std::cerr << "Eopt=" << Eini << "@Copt=" << Ctry << std::endl;
                    tao::_set(Cini,Ctry);

                }

            }
            
            return true;

        }
    }

}

