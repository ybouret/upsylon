#ifndef Y_SORTED_SUM_INCLUDED
#define Y_SORTED_SUM_INCLUDED 1

#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    //! careful sum of POSITIVE types to minimize underflow
    template <typename T>
    inline T sorted_sum( addressable<T> &arr )
    {
        T ans(0);
        hsort(arr,comparison::decreasing<T>);
        for(size_t i=arr.size();i>0;--i)
        {
            ans += arr[i];
        }
        return ans;
    }

    //! careful sum of SIGNED types to minimize underflow
    template <typename T>
    inline T sorted_sum_abs( addressable<T> &arr )
    {
        T ans(0);
        hsort(arr,comparison::decreasing_abs<T>);
        for(size_t i=arr.size();i>0;--i)
        {
            ans += arr[i];
        }
        return ans;
    }
}

#endif

