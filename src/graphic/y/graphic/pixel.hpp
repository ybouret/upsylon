
//! \file

#ifndef Y_GRAPHIC_PIXEL_INCLUDED
#define Y_GRAPHIC_PIXEL_INCLUDED 1

#include "y/graphic/types.hpp"
#include "y/sort/nwsrt.hpp"

namespace upsylon {

    namespace Graphic {

        struct Pixel
        {
            template <typename T> static inline
            T Median9__(  T arr[] ) throw()
            {
                nwsrt::on9(arr);
                return arr[4];
            }

            template <typename TYPE, typename T, size_t N>
            static inline
            TYPE Median9_( const TYPE arr[] )
            {
                TYPE ans;
                T   *res = (T*)&ans;
                for(size_t n=0;n<N;++n)
                {
                    T tmp[9] =
                    {
                        *( ((const T*)( &arr[0] ))+n),
                    };
                    res[n] = Median9__(tmp);
                }
                return ans;
            }

            template <typename T> static
            T Median9( T arr[] ) throw();


            template <typename T>
            static void Zero( T & ) throw();

            template <typename T>
            static bool IsZero( const T & ) throw();


            template <typename T>
            static T Invert( const T & ) throw();

        };
    }
}

#endif

