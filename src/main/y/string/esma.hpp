//!\file
#ifndef Y_STRING_ESMA_INCLUDED
#define Y_STRING_ESMA_INCLUDED 1

#include "y/os/platform.hpp"
 namespace upsylon {

    namespace core {

        struct esma
        {
            //! build offset table
            /**
             k[m], x[m], m>=0
             */
            template <typename T> static inline
            void build( unit_t *k, const T x[], const unit_t m) throw()
            {
                assert(m>0);
                assert(k!=0);
                assert(x!=0);
                unit_t  i = 0, j = k[0] = -1;
                while(i<m)
                {
                    while( (j> -1) && (x[i] != x[j]) )
                    {
                        j = k[j];
                    }
                    ++i; assert(i<m);
                    ++j; assert(j<m);
                    if (x[i] == x[j])
                    {
                        k[i] = k[j];
                    }
                    else
                    {
                        k[i] = j;
                    }
                }
            }

            //! find all offset of pattern x[m] in string y[n]
            /**
             use the precomputed table k[m] for x[n]
             bool proc(index)
             */
            template <typename T, typename FUNC> static inline
            size_t find(const T         y[], const unit_t n,
                        const T         x[], const unit_t m,
                        const unit_t    k[],
                        FUNC           &proc,
                        const size_t    countMax=0)
            {
                const bool   check = (countMax>0);
                size_t       count = 0;
                unit_t       i     = 0;
                unit_t       j     = 0;

                while (j<n)
                {
                    while( (i > -1) && (x[i] != y[j]) ) i = k[i];
                    ++i; ++j;
                    if(i>=m)
                    {
                        const size_t res = (j-i);
                        if( !proc(res) ) return count;                    //-- early return
                        ++count;
                         if( check && (count >= countMax) ) return count; //-- global return
                        i = k[i];
                    }
                }
                return count;
            }

        };


        
    }

}

#endif

