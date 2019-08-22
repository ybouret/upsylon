#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace mpl
    {
        natural natural:: __div(const uint8_t *num, const size_t nn,
                                const uint8_t *den, const size_t nd)
        {
            if(nd<=0)
            {
                throw libc::exception(EDOM,"mpl: division by zero");
            }

            const int cmp = compare_blocks(num, nn, den, nd);
            if(cmp<0)
            {
                //______________________________________________________________
                //
                //
                // special case: num < den => 0
                //
                //______________________________________________________________
                return natural();
            }
            else
            {
                if(cmp>0)
                {
                    //__________________________________________________________
                    //
                    //
                    // generic case num > den
                    //
                    //__________________________________________________________


                    //__________________________________________________________
                    //
                    // bracket result
                    //__________________________________________________________
                    size_t p = 1;
                    {
                        natural probe = __add(den,nd,den,nd); //! 2^(p=1) * den
                        while( compare_blocks(probe.byte,probe.bytes,num,nn) <= 0 )
                        {
                            ++p;
                            (void) probe.shl();
                        }
                        assert(  compare_blocks(probe.byte,probe.bytes,num,nn) >= 0 );
                    }

                    natural hi = natural::exp2(   p );
                    natural lo = natural::exp2( --p );

                    //__________________________________________________________
                    //
                    // bissection
                    //__________________________________________________________
                    while( p-- > 0 )
                    {

                        natural       mid = lo+hi; (void) mid.shr();
                        const natural tmp = __mul( mid.byte, mid.bytes, den, nd);
                        const int     chk = compare_blocks(tmp.byte,tmp.bytes,num,nn);
                        if(chk<0)
                        {
                            lo.xch(mid);
                        }
                        else
                        {
                            if(chk>0)
                            {
                                hi.xch(mid);
                            }
                            else
                            {
                                return mid; //!< special case
                            }
                        }
                    }
                    return lo;
                }
                else
                {
                    //__________________________________________________________
                    //
                    //
                    // special case num == den
                    //
                    //__________________________________________________________
                    return natural(1);
                }
            }
        }
    }
}
