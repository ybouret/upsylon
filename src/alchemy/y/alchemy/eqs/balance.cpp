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
                    std::cerr << "For " << eqs(i).name << std::endl;
                    const Sentry &sentry = *sentries[i];
                    size_t sp=0;
                    const double xi_min = sentry.getMin(sp,C);
                    std::cerr << "xi_min=" << xi_min << " @" << sp << std::endl;
                    const double xi_max = sentry.getMax(sp,C);
                    std::cerr << "xi_max=" << xi_max << " @" << sp << std::endl;

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


