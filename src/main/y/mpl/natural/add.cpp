#include "y/mpl/natural.hpp"

#define Y_ADD_UNROLL 0

#if Y_ADD_UNROLL == 1
#include "y/core/loop.hpp"

#define Y_ADD_COMMON(I)                 \
carry += small_data[I] + large_data[I]; \
sum[I] = uint8_t(carry);                \
carry >>= 8; \
assert(carry<256)

#define Y_ADD_CARRY(I)   \
carry += large_data[I];  \
sum[I] = uint8_t(carry); \
carry >>= 8;\
assert(carry<256)

#endif

namespace upsylon
{
    namespace mpl
    {
        natural natural:: __add(const uint8_t *l,
                                const size_t   nl,
                                const uint8_t *r,
                                const size_t   nr)
        {
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
            unsigned long carry = 0;


#if Y_ADD_UNROLL == 1
            Y_LOOP_FUNC(small_size, Y_ADD_COMMON, 0);
            if(large_size>small_size)
            {
                Y_LOOP_FUNC_(large_size-small_size, Y_ADD_CARRY, small_size);
            }
#else
            //__________________________________________________________________
            //
            // common part
            //__________________________________________________________________
            for(size_t i=0;i<small_size;++i)
            {
                carry += small_data[i] + large_data[i];
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
                carry += large_data[i];
                sum[i] = uint8_t(carry);
                carry >>= 8;
                assert(carry<256);
            }
#endif


            sum[large_size] = uint8_t(carry);
            ans.bytes = ns;
            ans.update();
            
            return ans;
        }
    }
}
