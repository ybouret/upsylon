#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        bool Reactor:: movedSingle(Addressable &C) throw()
        {
            size_t i=0;
            for(const Equilibrium::Node *node=eqs->head();node;node=node->next)
            {
                ++i;
                if(moved[i]) continue;
                const Equilibrium &eq = ***node;
                if(eq.solve(K[i],C))
                {
                    aliasing::_(moved[i]) = true;
                    return true;
                }
            }

            return false;
        }

        bool  Reactor:: regularized(Addressable &C) throw()
        {
            tao::ld(aliasing::_(moved),false);
            while(movedSingle(C))
            {
                if(isRegular(C)) return true;
            }
            return false;
        }


        bool  Reactor:: findRegular(Addressable &C, const double t) throw()
        {
            return isRegular(C,t) ? true : regularized(C);
        }

        bool  Reactor:: findRegular(Addressable &C) throw()
        {
            return isRegular(C) ? true : regularized(C);
        }
        

        bool  Reactor:: equilibrate(Addressable &C) throw()
        {
            lib.display(std::cerr,C) << std::endl;

            //__________________________________________________________________
            //
            // compute full Newton's step
            //__________________________________________________________________
            tao::neg(aliasing::_(xi),Gam);
            LU::solve(W,aliasing::_(xi));
            std::cerr << "xi=" << xi << std::endl;
            tao::mul( aliasing::_(dC),NuT,xi);
            std::cerr << "dC=" << dC << std::endl;

            for(size_t j=M;j>0;--j)
            {
                if(!active[j]) continue;

            }

            return true;
        }

        bool Reactor:: solve(Addressable &C, const double t) throw()
        {
            assert(isValid(C));
            return findRegular(C,t) ? equilibrate(C) : false;
        }

        bool   Reactor:: solve(Addressable &C) throw()
        {
            assert(isValid(C));
            return findRegular(C) ? equilibrate(C) : false;
        }


    }

}


