//! \file

#ifndef Y_MATH_STAT_AVERAGE_INCLUDED
#define Y_MATH_STAT_AVERAGE_INCLUDED 1

#include "y/mkl/types.hpp"
#include "y/type/point2d.hpp"
#include "y/sequence/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/sort/heap.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace math
    {

        namespace kernel {

            //! compute with destruction of data
            template <typename T>
            T average( addressable<T> &data, T *sdev)
            {
                const size_t n    = data.size();
                if(sdev)    *sdev = 0;
                if(n>0)
                {
                    hsort(data,comparison::decreasing_abs<T>);
                    T ave = 0;
                    for(size_t i=n;i>0;--i)
                    {
                        ave += data[i];
                    }
                    ave /= n;
                    if(sdev&&n>1)
                    {
                        for(size_t i=n;i>0;--i)
                        {
                            data[i] = square_of( data[i] - ave );
                        }
                        hsort(data,comparison::decreasing<T>);
                        T sig = 0;
                        for(size_t i=n;i>0;--i)
                        {
                            sig += data[i];
                        }
                        *sdev = sqrt_of(sig)/(n-1);
                    }
                    return ave;
                }
                else
                {
                    return 0;
                }
            }

            //! compute for a range
            //! low level average computation
            template <typename T, typename ITERATOR>
            T average( ITERATOR it, size_t n, T *sdev)
            {

                vector<T> tmp(n,as_capacity);
                while( n-- > 0 )
                {
                    tmp.push_back_( *it );
                    ++it;
                }
                return kernel::average(tmp,sdev);
            }
        }





        //! average for sequence with iterator
        template <typename SEQ> inline
        typename SEQ::type average_for( const SEQ &seq, typename SEQ::type *sdev=0)
        {
            return kernel::average(seq.begin(), seq.size(), sdev);
        }

        //! average for accessible type
        template <typename T> inline
        T average_of( const accessible<T> &arr, T *sdev=0)
        {
            size_t    n = arr.size();
            vector<T> tmp(n,as_capacity);
            while(n>0)
            {
                tmp.push_back_(arr[n]);
                --n;
            }
            return kernel::average(tmp,sdev);
        }




      
    }
}

#endif

