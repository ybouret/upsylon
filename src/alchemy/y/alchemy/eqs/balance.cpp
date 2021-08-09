#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"

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
                cond( aliasing::_(xi),C);
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

                    if(Cj2<=0)
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
            tao::mul( aliasing::_(xi),Nu,Cbad);
            std::cerr << "xi0 =" << xi << std::endl;
            RestrainXi(C);
            return sorted_sum(Csqr)*0.5;

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

                tao::set( aliasing::_(Ctry),C);

                const double Psi0 = Psi(aliasing::_(Ctry));
                std::cerr << "Psi0=" << Psi0 << std::endl;

                


                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


