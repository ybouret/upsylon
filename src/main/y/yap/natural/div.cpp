#include "y/yap/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace yap
    {
        natural natural:: divide(const natural &num, const natural &den)
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
            switch(natural::scmp(num,den))
            {
                case __negative: assert(num<den);  return natural(0); // early return
                case __zero:     assert(num==den); return natural(1); // early return
                case __positive: assert(num>den);  break;
            }

            // we look for num = q * den + rem
            // start with q = (1<<nbits)
            natural probe = den;
            size_t  nbits = 1;
            probe.shl(1);

        FIND_PROBE:
            switch(natural::scmp(num,probe))
            {
                case __negative: assert(num<probe);  break;
                case __zero:     assert(num==probe); return natural::exp2(nbits); // early return
                case __positive: assert(num>probe); ++nbits; (void) probe.shl(1); goto FIND_PROBE;
            }

            assert(num<probe);
            assert(num>(probe>>1));
            return natural();
        }
    }
    
}

