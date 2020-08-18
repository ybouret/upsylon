#include "y/yap/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace yap
    {
        natural natural:: _div(const natural &num, const natural &den)
        {
            static const char fn[] = "natural::division";
            
            //------------------------------------------------------------------
            // check valid denominator
            //------------------------------------------------------------------
            if(den.bytes<=0) throw libc::exception(EDOM,"%s by zero",fn);
            assert(den>0);
            
            //------------------------------------------------------------------
            // initial status
            //------------------------------------------------------------------
            const int c = natural::cmp(num,den);
            switch(c)
            {
                case -1: assert(num<den);  return natural(0);
                case  0: assert(num==den); return natural(1);
                case  1: assert(num>den);  break;
                default:
                    throw exception("%s corrupted comparison level-1",fn);
            }
            
            return natural();
        }
    }
    
}

