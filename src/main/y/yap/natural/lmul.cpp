
#include "y/yap/natural.hpp"
#include <iostream>

namespace upsylon
{
    namespace yap
    {

        natural natural::lmul(const word_type *lhs, const size_t lnw,
                              const word_type *rhs, const size_t rnw)
        {
            if(lnw>0&&rnw>0)
            {
                const size_t pnw = lnw+rnw;          // required words for product
                const size_t psz = pnw << word_exp2; // required bytes for product
                natural      res(psz,as_capacity);   // product

                {
                    word_type   *ptr = res.word;
                    word_type   *tgt = ptr+rnw;
                    for(size_t i=0;i<lnw;++i)
                    {
                        const core_type L     = lhs[i];
                        core_type       carry = 0;
                        word_type      *sub   = ptr+i;
                        for(size_t j=0;j<rnw;++j)
                        {
                            word_type        &here = sub[j];
                            const core_type   temp = core_type(here) + carry + L * core_type(rhs[j]);
                            here  = word_type(temp&word_maximum);
                            carry = temp >> word_bits;
                        }
                        tgt[i] = word_type(carry);
                    }
                    
                }

                res.words = pnw;
                res.bytes = psz;
                res.update();
                return res;
            }
            else
            {
                return natural();
            }
        }
    }

}
