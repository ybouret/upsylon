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
        void    Reactor:: RestrainXi(const Accessible &C) throw()
        {
            for(size_t j=Cond.size();j>0;--j)
            {
                const Condition &cond = Cond[j];
                cond( aliasing::_(xi1), C);
            }
        }


        double  Reactor:: Psi(Addressable &C) throw()
        {

            Csqr.free();
            for(size_t j=M;j>0;--j)
            {
                aliasing::_(Cbad[j]) = 0;
                if(active[j])
                {
                    const double  Cj  = aliasing::_(C[j]);
                    const double  Cj2 = Cj*Cj;

                    if(Cj2<=C2min)
                    {
                        aliasing::_(C[j]) = 0;
                    }
                    else
                    {
                        if(Cj<0)
                        {
                            aliasing::_(Cbad[j]) = -Cj;
                            Csqr.push_back_(Cj2);
                        }
                    }
                }
            }
            std::cerr << "Cbad=" << Cbad << std::endl;
            std::cerr << "Csqr=" << Csqr << std::endl;
            tao::mul( aliasing::_(xi0),Nu,Cbad, aliasing::_(xi1));
            std::cerr << "xi0 = " << xi0 << std::endl;
            RestrainXi(C);
            std::cerr << "xi1 = " << xi1 << std::endl;
            tao::mul( aliasing::_(dC),NuT,xi1);
            std::cerr << "dC  = " << dC << std::endl;
            return sorted_sum(Csqr)*0.5;
        }

        double Reactor:: PsiTry(const Accessible &C, const double u) throw()
        {
            Addressable &CC = aliasing::_(Ctry);
            tao::muladd(CC,C,u,dC);
            Csqr.free();
            for(size_t j=M;j>0;--j)
            {
                if(!active[j]) continue;
                const double  Cj  = Ctry[j];
                const double  Cj2 = Cj*Cj;

                if(Cj2<=C2min)
                {
                    CC[j] = 0;
                }
                else
                {
                    if(Cj<0)
                    {
                        Csqr.push_back_(Cj2);
                    }
                }

            }
            return sorted_sum(Csqr)*0.5;
        }

        namespace
        {
            struct BalanceFunction
            {
                const Accessible &C0;
                Reactor          &cs;

                double operator()(const double u) throw()
                {
                    return cs.PsiTry(C0,u);
                }

            };
        }

        bool Reactor:: balance(Addressable &C) throw()
        {
            std::cerr << "C=" << C << std::endl;
            if(N>0)
            {
                assert(NA>0);
                for(const Species::Node *node=lib->head();node;node=node->next)
                {
                    const Species &sp = ***node;
                    showCondition(std::cerr,sp,C);
                }
                std::cerr << "Primary Conditions: " << std::endl;
                for(size_t i=Cond.size();i>0;--i)
                {
                    Cond[i].show(std::cerr,C) << std::endl;
                }

                BalanceFunction F    = { C, *this };
                const double    Psi0 = Psi(C);
                std::cerr << "Psi0=" << Psi0 << "/" << F(0) << std::endl;

                if(Psi0>0)
                {
                    const double xi2 = tao::dot<double>::of(xi0,xi1);
                    std::cerr << "xi2=" << xi2 << std::endl;
                    if(xi2<=0)
                    {
                        return false;
                    }

                    const double u0 = 2.0 * (Psi0/xi2);

                    ios::ocstream fp("psi.dat");
                    for(double x=0;x<=2.0;x+=0.01)
                    {
                        fp("%g %g\n", x*u0, F(x*u0) );
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


