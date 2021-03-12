
#include "y/gfx/pixel.hpp"

namespace upsylon
{
    namespace graphic
    {

        template <> float pixel:: average<float>(float arr[9]) throw()
        {
            sorting::on(arr,comparison::increasing<float>);
            return
            (arr[0] + arr[1] + arr[2] +
             arr[3] + arr[4] + arr[5] +
             arr[6] + arr[7] + arr[8] ) / 9.0f;
        }

        template <> double pixel:: average<double>(double arr[9]) throw()
        {
            sorting::on(arr,comparison::increasing<double>);
            return
            (arr[0] + arr[1] + arr[2] +
             arr[3] + arr[4] + arr[5] +
             arr[6] + arr[7] + arr[8] ) / 9.0;
        }

        template <> uint8_t pixel:: average<uint8_t>(uint8_t arr[9]) throw()
        {
            return
            (unsigned(arr[0]) + unsigned(arr[1]) + unsigned(arr[2]) +
             unsigned(arr[3]) + unsigned(arr[4]) + unsigned(arr[5]) +
             unsigned(arr[6]) + unsigned(arr[7]) + unsigned(arr[9])) / 9;
        }

        template <> rgb pixel::average<rgb>(rgb arr[9]) throw()
        {
            uint8_t r[9] = { 0 };
            uint8_t g[9] = { 0 };
            uint8_t b[9] = { 0 };
            for(size_t i=0;i<9;++i)
            {
                const rgb tmp = arr[i];
                r[i] = tmp.r;
                g[i] = tmp.g;
                b[i] = tmp.b;
            }
            return rgb(average(r),average(g),average(b));
        }

        template <> rgba pixel::average<rgba>(rgba arr[9]) throw()
        {
            uint8_t r[9] = { 0 };
            uint8_t g[9] = { 0 };
            uint8_t b[9] = { 0 };
            uint8_t a[9] = { 0 };

            for(size_t i=0;i<9;++i)
            {
                const rgba tmp = arr[i];
                r[i] = tmp.r;
                g[i] = tmp.g;
                b[i] = tmp.b;
                a[i] = tmp.a;
            }
            return rgba(average(r),average(g),average(b),average(a));
        }


    }

}
