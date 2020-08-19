#include "y/yap/natural.hpp"
#include "y/exceptions.hpp"
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
            size_t  nbits = 1;   // TODO: optimize
            {
                natural probe = den;
                probe.shl(1);

            FIND_PROBE:
                switch(scmp(num,probe))
                {
                    case __negative: assert(num<probe);  break;
                    case __zero:     assert(num==probe); return exp2(nbits); // early return
                    case __positive: assert(num>probe); ++nbits; (void) probe.shl(1); goto FIND_PROBE;
                }

                //------------------------------------------------------------------
                // probe*den > num > (probe>>1) * den
                //------------------------------------------------------------------
                assert(num<probe);
                assert(num>(probe>>1));
                assert(probe==(den<<nbits));
                //std::cerr << num << " between " << (probe>>1) << " and " << probe << std::endl;
            }
            natural lower = exp2(nbits-1);
            natural upper = exp2(nbits);
            std::cerr << "quotient between " << lower << " and " << upper << std::endl;
            {
                natural       probe = add(lower,upper);probe.shr(1);
                const natural trial = probe * den;
                switch(scmp(trial,num))
                {
                    case __zero:       return probe; // exact: early return
                    case __negative: assert(trial<num); lower.xch(probe); break;
                    case __positive: assert(num<trial); upper.xch(probe); break;
                }

            }

            return natural();
        }
    }
    
}

