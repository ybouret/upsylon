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

        const char Primary::Prefix[] = "@";
        
        
        
        
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

                lib.display(std::cerr,C) << std::endl;
                
#if 0
                //bool solvedPrimary = true;
                for(size_t i=1;i<=N;++i)
                {
                    const Sentry &sentry = *sentries[i];
                    std::cerr << "For " << eqs(i).name << " [" << sentry.typeText() << "]" << std::endl;
                    switch(sentry.type)
                    {
                        case Sentry::HasNoBound: break;
                        case Sentry::HasOnlyGEQ: {
                            const Primary &pmin = sentry.getMin(C);
                            lib.print( std::cerr << "min for ", lib(pmin.sp) ) << " = " << -C[pmin.sp] << std::endl;
                            const Sentry::State state = sentry.solveHasOnlyGEQ(C,NuT, aliasing::_(xi));
                            std::cerr << Sentry::StateText(state) << std::endl;
                        }
                        break;

                        case Sentry::HasOnlyLEQ: {
                            const Primary &pmax = sentry.getMax(C);
                            lib.print( std::cerr << "max for ", lib(pmax.sp) ) << " = " << C[pmax.sp] << std::endl;
                            const Sentry::State state = sentry.solveHasOnlyLEQ(C,NuT, aliasing::_(xi));
                            std::cerr << Sentry::StateText(state) << std::endl;
                        } break;

                        case Sentry::IsBothWays: {
                            const Sentry::State state = sentry.solveIsBothWays(C,NuT, aliasing::_(xi));
                            std::cerr << Sentry::StateText(state) << std::endl;
                        } break;
                    }

                }
                lib.display(std::cerr,C) << std::endl;
#endif

                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


