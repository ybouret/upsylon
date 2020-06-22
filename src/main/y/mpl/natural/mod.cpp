
#include "y/mpl/natural.hpp"
#include "y/exceptions.hpp"
#include "y/type/utils.hpp"

#include <cerrno>
#include <cstring>

namespace upsylon
{
    namespace mpl
    {
        natural natural:: __mod(const uint8_t *num, const size_t nn,
                                const uint8_t *den, const size_t nd)
        {
            if(nd<=0) throw libc::exception(EDOM,"mpl: modulo by zero");

            const int cmp = compare_blocks(num, nn, den, nd);
            if(cmp<0)
            {
                //______________________________________________________________
                //
                //
                // special case: num < den => num
                //
                //______________________________________________________________
                natural ans(nn,as_capacity);
                memcpy(ans.byte,num,(ans.bytes)=nn);
                Y_MPN_CHECK(&ans);
                return ans;
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
                            probe.shl();
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
                        natural       mid = lo+hi; mid.shr();
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
                                return natural(); // special case, 0
                            }
                        }
                    }
                    // lo is the quotient
                    const natural qd = __mul( lo.byte, lo.bytes, den, nd);
                    assert( compare_blocks(qd.byte,qd.bytes,num,nn) <= 0 );
                    return __sub(num,nn,qd.byte,qd.bytes);
                }
                else
                {
                    //__________________________________________________________
                    //
                    //
                    // special case num == den => zero!
                    //
                    //__________________________________________________________
                    return natural();
                }

            }
        }

        bool  natural::  __dvs(const uint8_t *num, const size_t nn,
                               const uint8_t *den, const size_t nd)
        {
            if(nd<=0) throw libc::exception(EDOM,"mpn.is_divisible  by zero");

            const int cmp = compare_blocks(num, nn, den, nd);
            if(cmp<0)
            {
                //______________________________________________________________
                //
                //
                // special case: num < den => num => true if (num==0)
                //
                //______________________________________________________________
                return (nn<=0); //!< meaning num==0
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
                            probe.shl();
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
                        natural       mid = lo+hi; mid.shr();
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
                                return true; // special case, 0 => true
                            }
                        }
                    }
                    // lo is the quotient
                    const natural qd = __mul( lo.byte, lo.bytes, den, nd);
                    const int     dd = compare_blocks(qd.byte,qd.bytes,num,nn);
                    assert( dd  <= 0 );
                    return (0==dd);
                }
                else
                {
                    //__________________________________________________________
                    //
                    //
                    // special case num == den => zero! => true
                    //
                    //__________________________________________________________
                    return true;
                }

            }
        }

    }
}

