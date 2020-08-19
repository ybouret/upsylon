
#include "y/yap/natural.hpp"

namespace upsylon {

    namespace yap {

        bool natural:: eq(const word_type *lhs, const size_t lnw,
                          const word_type *rhs, const size_t rnw) throw()
        {
            assert(lhs);
            assert(rhs);
            //std::cerr << "eq #lhs=" << lnw << " | #rhs=" << rnw << std::endl;
            if(lnw==rnw)
            {
                for(size_t i=lnw;i>0;--i,++lhs,++rhs)
                {
                    if(*lhs!=*rhs)
                    {
                        return false;
                    }
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        bool natural:: neq(const word_type *lhs, const size_t lnw,
                           const word_type *rhs, const size_t rnw) throw()
        {
            assert(lhs);
            assert(rhs);
            if(lnw==rnw)
            {
                for(size_t i=lnw;i>0;--i,++lhs,++rhs)
                {
                    if(*lhs!=*rhs)
                    {
                        return true;
                    }
                }
                return false;
            }
            else
            {
                return true;
            }
        }


        int  natural:: cmp(const word_type *lhs, const size_t lnw,
                           const word_type *rhs, const size_t rnw) throw()
        {
            assert(lhs);
            assert(rhs);
            if(lnw<rnw)
            {
                return -1;
            }
            else if(rnw<lnw)
            {
                return 1;
            }
            else
            {
                for(size_t i=lnw;i>0;)
                {
                    --i;
                    const word_type L = lhs[i];
                    const word_type R = rhs[i];
                    if(L<R)      return -1;
                    else if(R<L) return  1;
                    else         continue;
                }
                return 0;
            }
        }

        sign_type  natural:: scmp(const word_type *lhs, const size_t lnw,
                                  const word_type *rhs, const size_t rnw) throw()
        {
            assert(lhs);
            assert(rhs);
            if(lnw<rnw)
            {
                return __negative;
            }
            else if(rnw<lnw)
            {
                return __positive;
            }
            else
            {
                for(size_t i=lnw;i>0;)
                {
                    --i;
                    const word_type L = lhs[i];
                    const word_type R = rhs[i];
                    if(L<R)      return __negative;
                    else if(R<L) return __positive;
                    else         continue;
                }
                return __zero;
            }
        }


    }

}
