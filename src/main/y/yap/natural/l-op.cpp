

#include "y/yap/natural.hpp"

namespace upsylon
{
    namespace yap
    {
        natural natural:: logical(l_op op,
                                  const word_type *lhs, const size_t lnw,
                                  const word_type *rhs, const size_t rnw)
        {
            assert(op);
            assert(lhs);
            assert(rhs);
            
            //------------------------------------------------------------------
            // organize data
            //------------------------------------------------------------------
            const word_type *small_data = lhs;
            size_t           small_size = lnw;
            const word_type *large_data = rhs;
            size_t           large_size = rnw;
            if(large_size<small_size)
            {
                cswap(small_data,large_data);
                cswap(small_size,large_size);
            }
            
            
            //------------------------------------------------------------------
            // create result
            //------------------------------------------------------------------
            const size_t n = large_size << word_exp2;
            natural      ans(n,as_capacity);
            
            //------------------------------------------------------------------
            // common loop
            //------------------------------------------------------------------
            word_type *w = ans.word;
            for(size_t i=0;i<small_size;++i)
            {
                w[i] = op(small_data[i],large_data[i]);
            }
            
            //------------------------------------------------------------------
            // remaining loop
            //------------------------------------------------------------------
            for(size_t i=small_size;i<large_size;++i)
            {
                w[i] = op(0,large_data[i]);
            }
            
            
            //------------------------------------------------------------------
            // finalize
            //------------------------------------------------------------------
            ans.bytes = n;
            ans.update();
            return ans;
        }
        
        
    }
    
}
