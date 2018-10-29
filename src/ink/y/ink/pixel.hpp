//! \file
#ifndef Y_INK_PIXEL_INCLUDED
#define Y_INK_PIXEL_INCLUDED 1

#include "y/ink/types.hpp"
#include "y/sort/heap.hpp"

namespace upsylon
{
    namespace Ink
    {

        //! data for color types
        template <typename T>
        struct Pixel
        {
            static const  T    Zero;                                     //!< the zero value
            static const  T    Opaque;                                   //!< the opaque value
            static inline T    Copy( const T &C ) { return C; }          //!< copy function
            static        T    Inverse(const T &C);                      //!< inverse function
            static        bool IsZero(const T &C);                       //!< test if is zero pixel
            static        T    Average(T *arr, const size_t num);        //!< average of pixels
            static        int  Compare(const T &lhs, const T &rhs);      //!< comparison
            static        T    Blend(const T &fg, const T &bg, const uint8_t alpha); //! blend fg over bg with alpha
            
            //! median value
            static inline T    Median(T *arr,const size_t num)
            {
                switch(num)
                {
                    case 0: return Pixel<T>::Zero;
                    case 1: return arr[0];
                    default:
                        break;
                }
                assert(num>=2);
                {
                    lightweight_array<T> ra(arr,num);
                    hsort(ra,Pixel<T>::Compare);
                }
                if( (num&1) != 0 )
                {
                    // odd
                    return arr[num>>1];
                }
                else
                {
                    // even
                    return Pixel<T>::Average(arr+((num>>1)-1),2);
                }
            }

            //! minimum value of items
            static inline T Erode(T *arr,const size_t num)
            {
                assert(num>0);
                T ans = arr[0];
                for(size_t i=1;i<num;++i)
                {
                    const T tmp = arr[i];
                    if( Compare(tmp,ans) < 0 )
                    {
                        ans = tmp;
                    }
                }
                return ans;
            }

            //! maximum value of items
            static inline T Dilate(T *arr,const size_t num)
            {
                assert(num>0);
                T ans = arr[0];
                for(size_t i=1;i<num;++i)
                {
                    const T tmp = arr[i];
                    if( Compare(tmp,ans) > 0 )
                    {
                        ans = tmp;
                    }
                }
                return ans;
            }
        };

    }
}

#if !defined(Y_INK_PIXEL_IMPL)

//! for external template field
#define _Y_PIXEL_DECL(TYPE,FIELD) extern template const TYPE upsylon::Ink::Pixel<TYPE>::FIELD

//! declare all external template fields
#define Y_PIXEL_DECL(T)  \
_Y_PIXEL_DECL(T,Zero);   \
_Y_PIXEL_DECL(T,Opaque)

Y_PIXEL_DECL(float);     //!< for float pixel
Y_PIXEL_DECL(uint8_t);   //!< for byte pixel
Y_PIXEL_DECL(double);    //!< for double pixel
Y_PIXEL_DECL(uint16_t);  //!< for word pixel
Y_PIXEL_DECL(uint32_t);  //!< for dword pixel
Y_PIXEL_DECL(upsylon::Ink::cplx); //!< for complex pixel

#endif

#endif

