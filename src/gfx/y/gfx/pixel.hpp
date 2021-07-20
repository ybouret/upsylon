//! \file


#ifndef Y_GFX_PIXEL_INCLUDED
#define Y_GFX_PIXEL_INCLUDED 1

#include "y/gfx/color/convert.hpp"
#include "y/sort/network/sort9.hpp"
#include "y/comparison.hpp"

namespace upsylon
{
    namespace graphic
    {

        //! pixel-wise operations
        struct pixel
        {
            //! dedicated 9 objects sorting
            typedef network_sort<9> sorting;

            //! median of 9 items
            template <typename T>
            static inline T median9(T *arr) throw()
            {
                sorting::on(arr, comparison::increasing<T>);
                return arr[4];
            }

            //! check if zero
            template <typename T> static bool is_zero(const T&) throw();

            //! sub
            template <typename T> static T    sub(const T &lhs, const T &rhs) throw();
            

            //! check if not zero
            template <typename T> static inline
            bool is_not_zero(const T &value) throw() { return !is_zero(value); }

            //! return a zeroed pixel
            template <typename T> static
            T zero() throw();

            //! return an opaque pixel
            template <typename T> static
            T opaque() throw();


            //! return a saturated pixel
            template <typename T> static
            T saturated(const T &) throw();
            

            //! average of 9 items
            template <typename T> static T average9(T *arr) throw();

            //! minimum of 9 items
            template <typename T>
            static inline T min9(const T *arr) throw()
            {
                T ans = arr[0];
                for(unsigned i=1;i<9;++i) { const T tmp = arr[i]; if(tmp<ans) ans = tmp; }
                return ans;
            }

            //! maximum of 9 items
            template <typename T>
            static inline T max9(const T arr[9]) throw()
            {
                T ans = arr[0];
                for(unsigned i=1;i<9;++i) { const T tmp = arr[i]; if(tmp>ans) ans = tmp; }
                return ans;
            }

        };

        //! take median by component
        template <> inline rgb pixel::median9<rgb>(rgb *arr) throw()
        {
            unsigned r[9] = { 0 };
            unsigned g[9] = { 0 };
            unsigned b[9] = { 0 };
            for(size_t i=0;i<9;++i)
            {
                const rgb tmp = arr[i];
                r[i] = tmp.r;
                g[i] = tmp.g;
                b[i] = tmp.b;
            }
            return rgb(uint8_t(median9(r)),
                       uint8_t(median9(g)),
                       uint8_t(median9(b))
                       );
        }

        //! take median by component
        template <> inline rgba pixel::median9<rgba>(rgba *arr) throw()
        {
            unsigned r[9] = { 0 };
            unsigned g[9] = { 0 };
            unsigned b[9] = { 0 };
            unsigned a[9] = { 0 };

            for(size_t i=0;i<9;++i)
            {
                const rgba tmp = arr[i];
                r[i] = tmp.r;
                g[i] = tmp.g;
                b[i] = tmp.b;
                a[i] = tmp.a;
            }
            return rgba(uint8_t(median9(r)),
                        uint8_t(median9(g)),
                        uint8_t(median9(b)),
                        uint8_t(median9(a))
                        );
        }

        //! minimum by intensity
        template <> inline rgb pixel::min9<rgb>(const rgb *arr) throw()
        {
            rgb     ans = arr[0];
            uint8_t score  = convert<uint8_t,rgb>::from(ans);
            for(unsigned i=1;i<9;++i)
            {
                const rgb     tmp   = arr[i];
                const uint8_t value = convert<uint8_t,rgb>::from(tmp);
                if(value<score)
                {
                    ans   = tmp;
                    score = value;
                }
            }
            return ans;
        }

        //! maximum by intensity
        template <> inline rgb pixel::max9<rgb>(const rgb *arr) throw()
        {
            rgb     ans = arr[0];
            uint8_t score  = convert<uint8_t,rgb>::from(ans);
            for(unsigned i=1;i<9;++i)
            {
                const rgb     tmp   = arr[i];
                const uint8_t value = convert<uint8_t,rgb>::from(tmp);
                if(value>score)
                {
                    ans   = tmp;
                    score = value;
                }
            }
            return ans;
        }



    }

}

#endif
