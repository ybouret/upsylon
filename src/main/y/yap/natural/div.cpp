#include "y/yap/natural.hpp"
#include "y/exceptions.hpp"
#include "y/type/utils.hpp"
#include <cerrno>
#include <iostream>

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
            switch(scmp(num,den))
            {
                case __negative: assert(num<den);  return natural(0); // early return
                case __zero:     assert(num==den); return natural(1); // early return
                case __positive: assert(num>den);  break;
            }

            //------------------------------------------------------------------
            // we look for num = q * den + rem
            // start with q = (1<<nbits)
            //------------------------------------------------------------------
            size_t       nbits    = max_of<size_t>(num.bits()-den.bits(),1);
            {
                natural probe = den;
                probe.shl(nbits);

            FIND_PROBE:
                switch(scmp(num,probe))
                {
                    case __negative: assert(num<probe);
                        break;

                    case __zero:     assert(num==probe);
                        return exp2(nbits); // early return

                    case __positive: assert(num>probe);
                        ++nbits;
                        (void) probe.shl(1);
                        goto FIND_PROBE;
                }

                //------------------------------------------------------------------
                // probe*den > num > (probe>>1) * den
                //------------------------------------------------------------------
                assert(num<probe);
                assert(num>(probe>>1));
                assert(probe==(den<<nbits));
            }

            //------------------------------------------------------------------
            // bisection
            //------------------------------------------------------------------
            natural lower = exp2(nbits-1);
            natural upper = exp2(nbits);

            while(true)
            {
                natural       probe = add(lower,upper);probe.shr(1);
                const natural trial = probe * den;
                switch(scmp(trial,num))
                {
                        // exact: early return
                    case __zero:
                        return probe;

                        // probe=>lower
                    case __negative: assert(trial<num);
                        if(probe==lower) return lower;
                        lower.xch(probe);
                        break;

                        // probe=>upper
                    case __positive: assert(num<trial);
                        if(probe==upper) return lower;
                        upper.xch(probe);
                        break;
                }
            }

        }
    }
    
}

