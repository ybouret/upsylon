#include "y/mpl/natural.hpp"
#include "y/type/cswap.hpp"



namespace upsylon
{
    namespace mpl
    {
        natural natural:: __add(const uint8_t *l,
                                const size_t   nl,
                                const uint8_t *r,
                                const size_t   nr)
        {
            typedef unsigned long carry_type;
            
            const uint8_t *small_data = l;
            size_t         small_size = nl;
            const uint8_t *large_data = r;
            size_t         large_size = nr;
            if(large_size<small_size)
            {
                cswap(small_data,large_data);
                cswap(small_size,large_size);
            }
            const size_t  ns    = large_size+1;
            natural       ans(ns,as_capacity);
            uint8_t      *sum   = ans.byte;
            carry_type    carry = 0;
            
            //__________________________________________________________________
            //
            // common part
            //__________________________________________________________________
            for(size_t i=0;i<small_size;++i)
            {
                carry += carry_type(small_data[i]) + carry_type(large_data[i]);
                sum[i] = uint8_t(carry);
                carry >>= 8;
                assert(carry<256);
            }
            
            //__________________________________________________________________
            //
            // propagate carry
            //__________________________________________________________________
            for(size_t i=small_size;i<large_size;++i)
            {
                carry += carry_type(large_data[i]);
                sum[i] = uint8_t(carry);
                carry >>= 8;
                assert(carry<256);
            }
            
            //__________________________________________________________________
            //
            // register carry
            //__________________________________________________________________
            sum[large_size] = uint8_t(carry);
            
            ans.bytes = ns;
            ans.update();
            
            return ans;
        }
    }
}
