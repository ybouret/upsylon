#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"
#include "y/alea.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Alchemy
    {

        
        bool Reactor:: balance1(Addressable &C) throw()
        {
            assert(N>0);
            assert(NA>0);
            bool balanced = true;
            {
                Addressable &Xi = aliasing::_(xi); assert(N==xi.size());
                for(const Equilibrium::Node *node = eqs->head();node;node=node->next)
                {
                    const Equilibrium &eq = ***node;
                    eqs.print(std::cerr << "guard ",eq) << std::endl;
                    const Guard::State st = guards[eq.indx]->solve(C,NuT,Xi,aliasing::_(ok));
                    std::cerr << "  " << Guard::StateText(st) << std::endl;
                    if(Guard::IsJammed==st)
                    {
                        balanced = false;
                    }
                }
            }
            
            lib.display(std::cerr << "Cbal=", C) << std::endl;
            return balanced;
        }


        bool Reactor:: balance(Addressable &C) throw()
        {
            lib.display(std::cerr << "C=",C) << std::endl;
            tao::ld( aliasing::_(ok),true);
            if(N>0)
            {
                assert(NA>0);
                showConditions(std::cerr,C);

                if( !balance1(C) )
                {
                    lib.display(std::cerr << "ok=", ok)  << std::endl;
                    return false;
                }
                std::cerr << "Balanced Leading!" << std::endl;

                Addressable &Xi = aliasing::_(xi);

                for(size_t i=N;i>0;--i)
                {
                    Xi[i] = alea.symm<double>();
                }
                std::cerr << "xi0=" << xi << std::endl;

                for(size_t i=guards.size();i>0;--i)
                {
                    guards[i]->limit(Xi,C);
                }
                std::cerr << "xi1=" << xi  << std::endl;

                showConditions(std::cerr,C);
                
                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


