
#include "y/yap/natural.hpp"
#include "y/code/round.hpp"

namespace upsylon
{
    namespace yap
    {
        natural natural::exp2(const size_t nbits)
        {
            const size_t ibit = nbits+1;
            const size_t imax = Y_ROUND8(ibit)>>3;
            const size_t itop = imax-1;
            natural      ans(imax,as_capacity);
            ans.get(itop) = bits_table::value[nbits&7];
            ans.bytes = imax;
            ans.update();
            assert(ans.bits()==ibit);
            return  ans;
        }

        bool natural:: get_bit(const word_type *w, const size_t ibit) throw()
        {
            assert(w);
            const size_t  i = ibit>>3;
            const uint8_t u = get_byte(w,i);
            return 0 != (u&bits_table::value[ibit&7]);
        }

        void natural:: set_bit(const word_type *w, const size_t ibit) throw()
        {
            assert(w);
            const size_t  i = ibit>>3;
            uint8_t      &u = get_byte(w,i);
            u |= bits_table::value[ibit&7];
        }

        void natural:: clr_bit(const word_type *w, const size_t ibit) throw()
        {
            assert(w);
            const size_t  i = ibit>>3;
            uint8_t      &u = get_byte(w,i);
            u &= bits_table::vmask[ibit&7];
        }

        bool natural:: bit(const size_t ibit) const throw()
        {
            assert(ibit<bits());
            return get_bit(word,ibit);
        }

        natural & natural:: shr(const size_t s) throw()
        {
            size_t source = bits();
            if(s>0)
            {
                if(s>=source)
                {
                    ldz();
                }
                else
                {
                    word_type *w = word;
                    {
                        size_t     wpos = 0;
                        size_t     rpos = s;
                        for(size_t i=source-s;i>0;--i)
                        {
                            const bool b = get_bit(w,rpos++);
                            if(b)
                            {
                                set_bit(w,wpos++);
                            }
                            else
                            {
                                clr_bit(w,wpos++);
                            }
                        }
                    }
                    for(size_t i=s;i>0;--i)
                    {
                        clr_bit(w,--source);
                    }
                    update();
                }
            }
            return *this;
        }
        
        natural & natural:: shl(const size_t s)
        {
            if(s>0)
            {
                const size_t cur_bits = bits();
                if(cur_bits>0)
                {
                    const size_t new_bits = cur_bits+s;
                    const size_t new_bytes = Y_BYTES_FOR(new_bits);
                    natural      shifted(new_bytes,as_capacity);
                    const word_type *source = word;
                    word_type       *target = shifted.word;
                    for(size_t i=0,j=s;i<cur_bits;++i,++j)
                    {
                        if(get_bit(source,i)) set_bit(target,j);
                    }
                    
                    shifted.bytes = new_bytes;
                    shifted.update();
                    xch(shifted);
                }
            }
            
            return *this;
        }
    
        natural operator >> (const natural &n, const size_t s)
        {
            natural ans(n); return ans.shr(s);
        }
        
        natural operator << (const natural &n, const size_t s)
        {
            natural ans(n); return ans.shl(s);
        }
        
        natural & natural:: operator<<=(const size_t s)
        {
            return shl(s);
        }
        
        natural & natural:: operator>>=(const size_t s) throw()
        {
            return shr(s);
        }
        
        

    }
}

