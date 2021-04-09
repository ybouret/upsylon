
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
                    for(size_t i=0;i<lnw;++i)
                    {
                        const core_type L     = lhs[i];
                        core_type       carry = 0;
                        for(size_t j=0;j<rnw;++j)
                        {
                            word_type        &p    = ptr[i+j];
                            const core_type   temp = core_type(p) + carry + L * core_type(rhs[j]);
                            p      = word_type(temp%word_radix);
                            carry  = temp/word_radix;
                        }
                        ptr[rnw+i] = word_type(carry);
                    }

#if 0
                    std::cerr << '(';
                    for(size_t i=0;i<pnw;++i)
                    {
                        std::cerr << ' ' << ptr[i];
                    }
                    std::cerr << ' ' << ')' << std::endl;
#endif
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
