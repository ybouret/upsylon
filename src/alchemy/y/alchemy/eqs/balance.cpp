#include "y/alchemy/reactor.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/type/utils.hpp"
#include "y/sort/sorted-sum.hpp"
#include "y/ios/ocstream.hpp"
#include "y/alea.hpp"
#include "y/mkl/kernel/gram-schmidt.hpp"
#include "y/code/textual.hpp"

namespace upsylon
{
    using namespace mkl;
    
    namespace Alchemy
    {
       
        
       
        
        bool Reactor:: balance(Addressable &C) throw()
        {
            if(N>0)
            {
                assert(NA>0);
                //______________________________________________________________
                //
                // balance leading conditions
                // _____________________________________________________________
                if( !balanceLeading(C) ) return false;

                //______________________________________________________________
                //
                // reduced starting point
                // _____________________________________________________________
                return balanceSeeking(C);
            }
            else
            {
                return true;
            }
        }
        
        
        
    }
    
}


