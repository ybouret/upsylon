
//!\file

#ifndef Y_COUNTING_SYMM_INDX_INCLUDED
#define Y_COUNTING_SYMM_INDX_INCLUDED 1

#include "y/core/isqrt.hpp"

namespace upsylon {

    //! 1 <= k <= n(n+1)/2, return 1<=j<=i<=n indices
    template <typename T> inline
    void symm_indx(T &i, T &j, const T k) throw()
    {
        assert(k>0);
        const T s_arg = ((k-1)<<3)+1;
        const T i_tmp = (i = (isqrt::of(s_arg)+1)>>1);
        const T i_off = ( i_tmp*(i_tmp-1) )>>1;
        j             = k-i_off;
    }

    //! 0 < k << n(n+1)/2, return 0<=j<=i<n indices
    template <typename T> inline
    void symm_indx0(T &i, T &j, const T k) throw()
    {
        assert(k>0);
        const T s_arg = (k<<3)+1;
        const T i_tmp = (i = (isqrt::of(s_arg)-1)>>1);
        const T i_off = ( i_tmp*(i_tmp+1) )>>1;
        j = k-i_off;
    }


}


#endif
