
#include "y/yap/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>


namespace upsylon
{
    namespace yap
    {
        natural natural:: sub(const word_type *lhs, const size_t lnw,
                              const word_type *rhs, const size_t rnw)
        {

            static const char fn[] = "yap::natural::sub: ";
            typedef signed_int<core_size>::type carry_t;
            static  const  carry_t              radix(word_radix);

            if(lnw<rnw)
            {
                throw libc::exception(EDOM,"%slhs<rhs [level-1]",fn);
            }

            assert(lnw>=rnw);
            const size_t space = lnw*word_size;
            natural      ans(space,as_capacity); assert( check(ans,"zero@sub") );
            word_type   *s = ans.word;
            //__________________________________________________________________
            //
            // firt loop: common words
            //__________________________________________________________________
            carry_t   carry = 0;
            for( size_t i=rnw; i>0; --i )
            {
                const carry_t a = *(lhs++); // L[i];
                const carry_t b = *(rhs++); // R[i];
                carry_t       d = (a-b)-carry;
                if( d < 0 )
                {
                    d    += radix;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<radix);
                *(s++) = word_type(d);
            }


            //__________________________________________________________________
            //
            // second loop: propagate carry
            //__________________________________________________________________
            for( size_t i=lnw-rnw; i>0; --i )
            {
                const carry_t a = *(lhs++);  //L[i];
                carry_t       d = a-carry;
                if( d < 0 )
                {
                    d    += radix;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<radix);
                *(s++) = word_type(d);
            }

            if(carry!=0)
            {
                throw libc::exception(EDOM,"%slhs<rhs [level-2]",fn);
            }

            ans.bytes=space;
            ans.update();
            assert( check(ans,"return@sub") );
            return ans;
        }


        natural & natural:: operator--()
        {
            static word_type one = 1;
            natural          tmp = sub(word,words,&one,1);
            xch(tmp);
            return *this;
        }

        natural  natural:: operator--(int)
        {
            static word_type one = 1;
            const  natural   ans(*this);
            {
                natural          tmp = sub(word,words,&one,1);
                xch(tmp);
            }
            return ans;
        }

    }

}
