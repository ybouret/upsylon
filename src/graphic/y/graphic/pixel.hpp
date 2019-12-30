
//! \file

#ifndef Y_GRAPHIC_PIXEL_INCLUDED
#define Y_GRAPHIC_PIXEL_INCLUDED 1

#include "y/graphic/types.hpp"
#include "y/sort/nwsrt.hpp"

namespace upsylon {

    namespace Graphic {

        struct Pixel
        {
            static const Point delta[8];


            template <typename T>
            static void Zero( T & ) throw();

            template <typename T>
            static bool IsZero( const T & ) throw();


            template <typename T>
            static T Invert( const T & ) throw();

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
                        *( ((const T*)( &arr[1] ))+n),
                        *( ((const T*)( &arr[2] ))+n),
                        *( ((const T*)( &arr[3] ))+n),
                        *( ((const T*)( &arr[4] ))+n),
                        *( ((const T*)( &arr[5] ))+n),
                        *( ((const T*)( &arr[6] ))+n),
                        *( ((const T*)( &arr[7] ))+n),
                        *( ((const T*)( &arr[8] ))+n),
                    };
                    res[n] = Median9__(tmp);
                }
                return ans;
            }


            template <typename T> static
            T Median9( T arr[] ) throw();

            static float   Average9__( float   arr[] ) throw();
            static uint8_t Average9__( uint8_t arr[] ) throw();

            template <typename TYPE, typename T, size_t N>
            static inline
            TYPE Average9_( const TYPE arr[] )
            {
                TYPE ans;
                T   *res = (T*)&ans;
                for(size_t n=0;n<N;++n)
                {
                    T tmp[9] =
                    {
                        *( ((const T*)( &arr[0] ))+n),
                        *( ((const T*)( &arr[1] ))+n),
                        *( ((const T*)( &arr[2] ))+n),
                        *( ((const T*)( &arr[3] ))+n),
                        *( ((const T*)( &arr[4] ))+n),
                        *( ((const T*)( &arr[5] ))+n),
                        *( ((const T*)( &arr[6] ))+n),
                        *( ((const T*)( &arr[7] ))+n),
                        *( ((const T*)( &arr[8] ))+n),
                    };
                    res[n] = Average9__(tmp);
                }
                return ans;
            }


            template <typename T> static
            T Average9( T arr[] ) throw();
            
            template <typename T> static
            T MinOf( const T &lhs, const T &rhs ) throw();

            template <typename T> static
            T MinOf9(const T arr[]) throw()
            {
                T ans = arr[0];
                for(size_t i=1;i<9;++i)
                {
                    ans = MinOf(ans,arr[i]);
                }
                return ans;
            }

            template <typename T> static
            T MaxOf( const T &lhs, const T &rhs ) throw();

            template <typename T> static
            T MaxOf9(const T arr[]) throw()
            {
                T ans = arr[0];
                for(size_t i=1;i<9;++i)
                {
                    ans = MaxOf(ans,arr[i]);
                }
                return ans;
            }


        };
    }
}

#endif

