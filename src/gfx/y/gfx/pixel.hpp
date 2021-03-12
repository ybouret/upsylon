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

        struct pixel
        {
            typedef network_sort<9> sorting;

            template <typename T>
            static inline T median(T arr[9]) throw()
            {
                sorting::on(arr, comparison::increasing<T>);
                return arr[4];
            }


            
            template <typename T> static T average(T arr[9]) throw();

            template <typename T>
            static inline T minimum(const T arr[9]) throw()
            {
                T ans = arr[0];
                for(unsigned i=1;i<9;++i) { const T tmp = arr[i]; if(tmp<ans) ans = tmp; }
                return ans;
            }

            template <typename T>
            static inline T maximum(const T arr[9]) throw()
            {
                T ans = arr[0];
                for(unsigned i=1;i<9;++i) { const T tmp = arr[i]; if(tmp>ans) ans = tmp; }
                return ans;
            }

        };


        template <> inline rgb pixel::median<rgb>(rgb arr[9]) throw()
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
            return rgb(uint8_t(median(r)),
                       uint8_t(median(g)),
                       uint8_t(median(b))
                       );
        }

        template <> inline rgba pixel::median<rgba>(rgba arr[9]) throw()
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
            return rgba(uint8_t(median(r)),
                        uint8_t(median(g)),
                        uint8_t(median(b)),
                        uint8_t(median(a))
                        );
        }

        template <> inline rgb pixel::minimum<rgb>(const rgb arr[9]) throw()
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

        template <> inline rgb pixel::maximum<rgb>(const rgb arr[9]) throw()
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
