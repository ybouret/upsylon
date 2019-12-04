
//! \file

#ifndef Y_MATH_STAT_MEDIAN_INCLUDED
#define Y_MATH_STAT_MEDIAN_INCLUDED 1

#include "y/math/types.hpp"
#include "y/comparison.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"

namespace upsylon {
    
    namespace math {

        namespace kernel {

            //! compute with destruction of data
            template <typename T> inline
            T median( addressable<T> &data, T *adev)
            {
                if(adev) *adev=0;
                const size_t n = data.size();
                if(n>0)
                {
                    hsort(data,comparison::increasing<T>);
                    const size_t nh = (n>>1);
                    const T      m  = ( (n&1) != 0 ) ? data[nh+1] : (data[nh]+data[nh+1])/T(2);
                    if(adev)
                    {
                        for(size_t i=n;i>0;--i)
                        {
                            data[i] = abs_of(data[i]-m);
                        }
                        hsort(data,comparison::decreasing<T>);
                        T a = 0;
                        for(size_t i=n;i>0;--i)
                        {
                            a += data[i];
                        }
                        *adev = a/n;
                    }
                    return m;
                }
                else
                {
                    return 0;
                }
            }

            //! median on a range
            template <typename T, typename ITERATOR> inline
            T median( ITERATOR it, const size_t n, T *adev )
            {
                vector<T> data(n,as_capacity);
                for(size_t i=n;i>0;--i,++it)
                {
                    data.push_back_(*it);
                }
                return median(data,adev);
            }

        }



        //! median computation for sequence with iterator
        template <typename SEQ> inline
        typename SEQ::type median_for( const SEQ &seq, typename SEQ::type *adev=0)
        {
            return kernel::median(seq.begin(), seq.size(), adev);
        }

        //! median for accessible
        template <typename T> inline
        T median_of( const accessible<T> &arr, T *adev=0)
        {
            size_t    n = arr.size();
            vector<T> tmp(n,as_capacity);
            while(n>0)
            {
                tmp.push_back_(arr[n]);
                --n;
            }
            return kernel::median(tmp,adev);
        }

    }
}


#endif

