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
                        }
                        break;
                        case Sentry::HasOnlyLEQ: {
                            const Primary &pmax = sentry.getMax(C);
                            lib.print( std::cerr << "max for ", lib(pmax.sp) ) << " = " << C[pmax.sp] << std::endl;
                        } break;
                        case Sentry::IsBothWays: break;
                    }

                    sentry.solve(C);
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


