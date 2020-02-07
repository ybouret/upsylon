//!\file
#ifndef Y_STRING_ESMA_INCLUDED
#define Y_STRING_ESMA_INCLUDED 1

#include "y/string/basic.hpp"

namespace upsylon {

    namespace core {

        struct esma
        {
            //! build offset table
            /**
             k[m], x[m], m>=0
             */
            template <typename T> static inline
            void build( ptrdiff_t *k, const T x[], const ptrdiff_t m) throw()
            {
                assert(m>=0);
                assert(!(0==x&&m>0));
                assert(!(0==k&&m>0));
                ptrdiff_t  i = 0, j = k[0] = -1;
                while(i<m)
                {
                    while( (j> -1) && (x[i] != x[j]) )
                    {
                        j = k[j];
                    }
                    ++i; ++j;
                    if (x[i] == x[j])
                        k[i] = k[j];
                    else
                        k[i] = j;
                }
            }

            //! find all offset of patter x[m] in string y[n]
            /**
             use the precomputed table k[m] for x[n]
             bool proc(index)
             */
            template <typename T, typename FUNC> static inline
            size_t find(const T         y[], const ptrdiff_t n,
                        const T         x[], const ptrdiff_t m,
                        const ptrdiff_t k[],
                        FUNC           &proc,
                        const size_t    countMax=0)
            {
                const bool      check = (countMax>0);
                size_t          count = 0;
                ptrdiff_t       i     = 0;
                ptrdiff_t       j     = 0;

                while (j<n)
                {
                    while( (i > -1) && (x[i] != y[j]) ) i = k[i];
                    ++i; ++j;
                    if(i>=m)
                    {
                        const size_t res = (j-i);
                        if( !proc(res) ) return count; //!< early return
                        ++count;
                        //std::cerr << "@" << res << std::endl;
                        // if( ! on_find( res, args ) )return;
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

