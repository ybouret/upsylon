#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        bool Reactor:: sweep(Addressable &C) throw()
        {
            size_t i=0;
            for(const Equilibrium::Node *node=eqs->head();node;node=node->next)
            {
                ++i;
                if(swept[i]) continue;
                const Equilibrium &eq = ***node;
                if(eq.solve(K[i],C))
                {
                    aliasing::_(swept[i]) = true;
                    return true;
                }
            }

            return false;
        }

        bool Reactor:: solve(Addressable &C, const double t) throw()
        {

            //__________________________________________________________________
            //
            // first step find a regular point
            //__________________________________________________________________
            addressable<bool> &flags = aliasing::_(swept);
            if(!isRegular(C,t))
            {
                tao::ld(flags,false);
                while(sweep(C))
                {
                    if(isRegular(C)) goto REGULAR;
                }
                return false;
            }

        REGULAR:
            std::cerr << "RegularC=" << C << std::endl;

            // compute full Newton's step
            tao::neg(aliasing::_(xi),Gam);
            LU::solve(W,aliasing::_(xi));
            std::cerr << "xi=" << xi << std::endl;
            tao::mul( aliasing::_(dC),NuT,xi);
            std::cerr << "dC=" << dC << std::endl;

            for(size_t j=M;j>0;--j)
            {
                if(!active[j]) continue;
                

            }



            return false;
        }

    }

}


