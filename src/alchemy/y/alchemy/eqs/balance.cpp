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


                return false;
            }
            else
            {
                return true;
            }
        }

        

    }

}


