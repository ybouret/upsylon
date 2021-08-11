#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {
        

        void Reactor:: restrain(Addressable      &extents,
                                const Accessible &concentrations) const throw()
        {
            for(size_t i=cond.size();i>0;--i)
            {
                cond[i](extents,concentrations);
            }
        }

        bool Reactor:: topology(const Accessible &C) throw()
        {
            Limits::Array &_ = aliasing::_(limits); assert(N==limits.size());
            for(size_t i=N;i>0;--i) _[i].reset();
            for(size_t i=cond.size();i>0;--i)
            {
                cond[i](_,C);
            }
            bool res = true;
            for(size_t i=N;i>0;--i)
            {
                if(!_[i].mobility()) res = false;
            }
            return res;
        }
        

        double Reactor:: Psi(const Accessible &C) throw()
        {
            // build vector of invalid concentrations
            Csqr.free();
            for(size_t j=M;j>0;--j)
            {
                double &CC = aliasing::_(Cpsi[j]);
                CC = 0;
                if(active[j])
                {
                    const double Cj = C[j];
                    if(Cj<0)
                    {
                        Csqr.push_back_(Cj*Cj);
                        CC = -Cj;
                    }
                }
            }

            // build corresponding free extent
            tao::mul(aliasing::_(Xpsi),Nu,Cpsi);

            std::cerr << "Cpsi=" << Cpsi << std::endl;
            std::cerr << "Xpsi=" << Xpsi << std::endl;

            return sorted_sum(Csqr)*0.5;
        }

        double Reactor:: Psi(const Accessible &C, const double u) throw()
        {
            tao::mulset(aliasing::_(Xtry),u,Xpsi);
            restrain(aliasing::_(Xtry),C);

            std::cerr << "xi = " << Xpsi << " => " << Xtry << std::endl;

            tao::mul_add( aliasing::_(Ctry), C, NuT, Xtry );
            std::cerr << "Ctry=" << Ctry << std::endl;

            Csqr.free();
            for(size_t j=M;j>0;--j)
            {
                if(active[j])
                {
                    const double Cj = C[j];
                    double      &CC = aliasing::_(Ctry)[j];
                    if(CC<0)
                    {
                        if(Cj>=0)
                        {
                            assert( die("shouldn't happen") );
                        }
                        Csqr.push_back_(CC*CC);
                    }
                }
            }

            return sorted_sum(Csqr)*0.5;
        }

        namespace
        {
            struct Balancing
            {
                const Accessible &C0;
                Reactor          &cs;

                double operator()(double u) throw()
                {
                    return cs.Psi(C0,u);
                }

            };
        }
        
        bool Reactor:: balance(Addressable &C) throw()
        {
            std::cerr << "C=" << C << std::endl;
            if(N>0)
            {
                assert(NA>0);
                showConditions(std::cerr,C);
                const bool mobile = topology(C);
                for(size_t i=1;i<=N;++i)
                {
                    const Equilibrium &eq = eqs(i);
                    eqs.print(std::cerr,eq) << " : ";
                    const Limits &L = limits[i];
                    L.show(std::cerr) << std::endl;
                }

                if(!mobile)
                {
                    std::cerr << "BAD!" << std::endl;
                }
                else
                {
                    std::cerr << "GOOD!" << std::endl;
                }

                double Psi0 = Psi(C);
                std::cerr << "Psi0=" << Psi0 << std::endl;


                return 0;
                Balancing F = { C, *this };
                {
                    ios::ocstream fp("psi.dat");
                    for(double u=0;u<=10.0;u+=0.1)
                    {
                        fp("%g %g\n", u, F(u) );
                    }
                }
                


                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


