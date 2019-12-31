
//! \file

#ifndef Y_GRAPHIC_PIXEL_INCLUDED
#define Y_GRAPHIC_PIXEL_INCLUDED 1

#include "y/graphic/types.hpp"
#include "y/sort/nwsrt.hpp"

namespace upsylon {

    namespace Graphic {

        //! Pixel operations
        struct Pixel
        {
            static const Point delta[8]; //!< 4+4 delta directions


            //! set to zero
            template <typename T> static void Zero( T & ) throw();

            //! test against zero
            template <typename T> static bool IsZero( const T & ) throw();


            //! invert color
            template <typename T> static T Invert( const T & ) throw();

            //! median value of local array
            template <typename T> static inline
            T Median9__(  T arr[] ) throw()
            {
                nwsrt::on9(arr);
                return arr[4];
            }

            //! median by channels
            template <typename TYPE, typename T, size_t N> static inline
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

            //! generic median
            template <typename T> static
            T Median9( T arr[] ) throw();

            //! average of float
            static float   Average9__( float   arr[] ) throw();

            //! average of bytes
            static uint8_t Average9__( uint8_t arr[] ) throw();

            //! average by channels
            template <typename TYPE, typename T, size_t N> static inline
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

            //! generic average
            template <typename T> static T Average9( T arr[] ) throw();

            //! generic minimal value
            template <typename T> static T MinOf( const T &lhs, const T &rhs ) throw();

            //! minimal value of a local array
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

            //! generic maximal value
            template <typename T> static T MaxOf( const T &lhs, const T &rhs ) throw();

            //! maximal value of an array
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

