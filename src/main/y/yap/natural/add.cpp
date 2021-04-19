#include "y/yap/natural.hpp"

namespace upsylon
{
    namespace yap
    {
        natural natural:: add(const word_type *lhs, const size_t lnw,
                              const word_type *rhs, const size_t rnw)
        {
            assert(lhs);assert(rhs);
            //--------------------------------------------------------------
            // organize data
            //--------------------------------------------------------------
            const word_type *small_data = lhs;
            size_t           small_size = lnw;
            const word_type *large_data = rhs;
            size_t           large_size = rnw;
            if(large_size<small_size)
            {
                cswap(small_data,large_data);
                cswap(small_size,large_size);
            }
            const size_t num   = large_size+1;     // final number of words
            const size_t space = num << word_exp2; // in bytes
            natural      ans(space,as_capacity); assert( check(ans,"zero@add") );

            
            word_type   *sum   = ans.word;
            core_type    carry = 0;

            //--------------------------------------------------------------
            // add for common part
            //--------------------------------------------------------------
            for(size_t i=0;i<small_size;++i)
            {
                carry += core_type(small_data[i])+core_type(large_data[i]);
                sum[i] = word_type(carry);
                carry >>= word_bits;
                assert(carry<core_type(word_maxi));
            }

            //--------------------------------------------------------------
            // then propagate carry
            //--------------------------------------------------------------
            for(size_t i=small_size;i<large_size;++i)
            {
                carry += core_type(large_data[i]);
                sum[i] = word_type(carry);
                carry >>= word_bits;
                assert(carry<core_type(word_maxi));
            }

            //__________________________________________________________________
            //
            // register carry
            //__________________________________________________________________
            sum[large_size] = word_type(carry);

            ans.bytes = space;
            ans.update();

            assert( check(ans,"result@add") );
            return ans;
        }

        natural natural:: operator+() const
        {
            return natural(*this);
        }

        natural & natural:: operator++()
        {
            static word_type one = 1;
            natural          tmp = add(&one,1,word,words);
            xch(tmp);
            return *this;
        }

        natural  natural:: operator++(int)
        {
            static word_type one = 1;
            const  natural   ans(*this);
            {
                natural          tmp = add(&one,1,word,words);
                xch(tmp);
            }
            return ans;
        }

    }
}



