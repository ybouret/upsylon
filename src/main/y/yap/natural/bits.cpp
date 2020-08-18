
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
                    size_t target = source-s;

                }
            }
            return *this;
        }

        
    }
}

