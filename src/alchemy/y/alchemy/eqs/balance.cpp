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

        
        
        
        
        bool Reactor:: balance(Addressable &C) throw()
        {
            lib.display(std::cerr << "C=",C) << std::endl;
            if(N>0)
            {
                assert(NA>0);
                showConditions(std::cerr,C);


                {
                    Addressable &Xi = aliasing::_(xi);
                    for(const Equilibrium::Node *node = eqs->head();node;node=node->next)
                    {
                        const Equilibrium &eq = ***node;                          eqs.print(std::cerr << "guard ",eq) << std::endl;
                        const Guard::State st = guards[eq.indx]->solve(C,NuT,Xi); std::cerr << "  " << Guard::StateText(st) << std::endl;

                    }
                }

                lib.display(std::cerr << "C=", C) << std::endl;

                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


