
#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mpl
    {
        natural natural:: __sub(const uint8_t *l, const size_t nl,
                                const uint8_t *r, const size_t nr)
        {
            typedef long int carry_t;

            if(nl<nr)
            {
                throw libc::exception(EDOM,"mpl.natural.sub(lhs<rhs) [level-1]");
            }

            assert(nl>=nr);

            natural ans(nl,as_capacity);
            uint8_t *s = ans.byte;

            //__________________________________________________________________
            //
            // firt loop: common bytes
            //__________________________________________________________________
            carry_t   carry = 0;
            for( size_t i=nr; i>0; --i )
            {
                const carry_t a = *(l++); // L[i];
                const carry_t b = *(r++); // R[i];
                carry_t       d = (a-b)-carry;
                if( d < 0 )
                {
                    d    += 256;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<256);
                *(s++) = static_cast<uint8_t>(d);
            }


            //__________________________________________________________________
            //
            // second loop: propagate carry
            //__________________________________________________________________
            for( size_t i=nl-nr; i>0; --i )
            {
                const carry_t a = *(l++);  //L[i];
                carry_t       d = a-carry;
                if( d < 0 )
                {
                    d    += 256;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<256);
                *(s++) = static_cast<uint8_t>(d);
            }

            if(carry!=0)
            {
                throw libc::exception(EDOM,"mpl.natural.sub(lhs<rhs) [level-2]");
            }

            ans.bytes=nl;
            ans.update();
            return ans;
        }

    }

}

