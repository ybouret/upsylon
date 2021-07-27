
//!\file

#ifndef Y_COUNTING_SYMM_INDX_INCLUDED
#define Y_COUNTING_SYMM_INDX_INCLUDED 1

#include "y/core/isqrt.hpp"
#include <iostream>

namespace upsylon {

    //__________________________________________________________________________
    //
    //
    //! computing indices for symmetric addressing
    //
    //__________________________________________________________________________
    struct symm_indx
    {
        //! 1 <= k <= n(n+1)/2, return 1<=j<=i<=n indices
        template <typename T> static inline
        void get_v1(T &i, T &j, const T k) throw()
        {
            assert(k>0);
            const T s_arg = ((k-1)<<3)+1;
            const T i_tmp = (i = (mkl::sqrt_of(s_arg)+1)>>1);
            const T i_off = ( i_tmp*(i_tmp-1) )>>1;
            j             = k-i_off;
        }


        //! 1 <= k <= n(n+1)/2, return 1<=j<=i<=n indices
        template <typename T> static inline
        void get_v2(T &i, T &j, const T k) throw()
        {
            assert(k>0);
            T i_tmp = 1;
            {
                T sum=1;
                while(sum<k)
                    sum += ++i_tmp;
            }
            j=k-( (i_tmp*(i_tmp-1))>>1 );
            i=i_tmp;
        }

        //! 1 <= k <= n(n+1)/2, return 1<=j<=i<=n indices
        template <typename T> static inline
        void get_v3(T &i, T &j, const T k) throw()
        {
            assert(k>0);
            const T km = k-1;
            if(km<sizeof(pair)/2)
            {
                const uint8_t *p = &pair[km][0];
                i = p[0]+1;
                j = p[1]+1;
            }
            else
            {
                return get_v1(i,j,k);
            }
        }

        


        //! 0 < k << n(n+1)/2, return 0<=j<=i<n indices
        template <typename T> static inline
        void getC_v1(T &i, T &j, const T k) throw()
        {
            const T s_arg = (k<<3)+1;
            const T i_tmp = (i = (mkl::sqrt_of(s_arg)-1)>>1);
            const T i_off = ( i_tmp*(i_tmp+1) )>>1;
            j = k-i_off;
        }

        //! 0 < k << n(n+1)/2, return 0<=j<=i<n indices
        template <typename T> static inline
        void getC_v2(T &i, T &j, const T k) throw()
        {
            if(k<sizeof(pair)/2)
            {
                const uint8_t *p = &pair[k][0];
                i = p[0];
                j = p[1];
            }
            else
            {
                getC_v1(i,j,k);
            }
        }


        static const uint8_t pair[32768][2];
    };


}


#endif
