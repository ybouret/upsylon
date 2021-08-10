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


        
        bool Reactor:: balance(Addressable &C) throw()
        {
            std::cerr << "C=" << C << std::endl;
            if(N>0)
            {
                assert(NA>0);
                showConditions(std::cerr,C);


                double Psi0 = Psi(C);
                std::cerr << "Psi0=" << Psi0 << std::endl;

                {
                    ios::ocstream fp("psi.dat");
                    for(double u=0;u<=2.0;u+=0.02)
                    {
                        fp("%g %g\n", u, Psi(C,u) );
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


