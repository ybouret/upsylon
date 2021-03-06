
//! \file

#ifndef Y_GRAPHIC_PIXEL_INCLUDED
#define Y_GRAPHIC_PIXEL_INCLUDED 1

#include "y/graphic/types.hpp"
#include "y/sort/network/sort9.hpp"
#include "y/comparison.hpp"

namespace upsylon {

    namespace Graphic {

        //! Pixel operations
        struct Pixel
        {
            typedef network_sort<9> nwsrt;  //!< for sorting 9 points
            static const Point delta[8];    //!< 4+4 delta directions

            //------------------------------------------------------------------
            //
            // simple operations
            //
            //------------------------------------------------------------------

            //! set to zero
            template <typename T> static void Zero( T & ) throw();

            //! test against zero
            template <typename T> static bool IsZero( const T & ) throw();


            //! invert color
            template <typename T> static T Invert( const T & ) throw();

            //------------------------------------------------------------------
            //
            // medians
            //
            //------------------------------------------------------------------

            //! median value of local array
            template <typename T> static inline
            T Median9__(  T arr[] ) throw()
            {
                nwsrt::on(arr,comparison::increasing<T>);
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


            //------------------------------------------------------------------
            //
            // averages
            //
            //------------------------------------------------------------------

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

            //------------------------------------------------------------------
            //
            // minima
            //
            //------------------------------------------------------------------

            //! generic minimal value
            template <typename T> static T MinOf( const T &lhs, const T &rhs ) throw();

            //! minimal value of a local array
            template <typename T> static inline
            T MinOf9(const T arr[]) throw()
            {
                T ans = arr[0];
                for(size_t i=1;i<9;++i)
                {
                    ans = MinOf(ans,arr[i]);
                }
                return ans;
            }

            //------------------------------------------------------------------
            //
            // maxima
            //
            //------------------------------------------------------------------

            //! generic maximal value
            template <typename T> static T MaxOf( const T &lhs, const T &rhs ) throw();

            //! maximal value of an array
            template <typename T> static inline
            T MaxOf9(const T arr[]) throw()
            {
                T ans = arr[0];
                for(size_t i=1;i<9;++i)
                {
                    ans = MaxOf(ans,arr[i]);
                }
                return ans;
            }


            //------------------------------------------------------------------
            //
            // blending
            //
            //------------------------------------------------------------------

            //! generic one channel blend
            template <typename T> static
            T Blend(const T &fg, const unsigned wf,
                    const T &bg, const unsigned wb ) throw();

            //! generic multiple channels blend
            template <typename TYPE, typename T, const size_t DIM> static inline
            TYPE BlendType( const TYPE &FG, const TYPE &BG, const uint8_t alpha) throw()
            {
                const unsigned wf = alpha;
                const unsigned wb = 255-alpha;
                const T       *fg = (const T *) &FG;
                const T       *bg = (const T *) &BG;
                TYPE           ans;
                T             *tgt = (T *)&ans;
                for(size_t dim=0;dim<DIM;++dim)
                {
                    tgt[dim] = Blend<T>(fg[dim],wf,bg[dim],wb);
                }
                return ans;
            }


            //! generic color blending
            template <typename T> static
            T Blend( const T &fg, const T &bg, const uint8_t alpha ) throw();

        };
    }
}

#endif

