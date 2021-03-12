
#include "y/gfx/pixel.hpp"

namespace upsylon
{
    namespace graphic
    {

        template <> float pixel:: average9<float>(float *arr) throw()
        {
            sorting::on(arr,comparison::increasing<float>);
            return
            (arr[0] + arr[1] + arr[2] +
             arr[3] + arr[4] + arr[5] +
             arr[6] + arr[7] + arr[8] ) / 9.0f;
        }

        template <> double pixel:: average9<double>(double *arr) throw()
        {
            sorting::on(arr,comparison::increasing<double>);
            return
            (arr[0] + arr[1] + arr[2] +
             arr[3] + arr[4] + arr[5] +
             arr[6] + arr[7] + arr[8] ) / 9.0;
        }

        template <> uint8_t pixel:: average9<uint8_t>(uint8_t *arr) throw()
        {
            return
            (unsigned(arr[0]) + unsigned(arr[1]) + unsigned(arr[2]) +
             unsigned(arr[3]) + unsigned(arr[4]) + unsigned(arr[5]) +
             unsigned(arr[6]) + unsigned(arr[7]) + unsigned(arr[8])) / 9;
        }

        template <> rgb pixel::average9<rgb>(rgb *arr) throw()
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
            return rgb(average9(r),average9(g),average9(b));
        }

        template <> rgba pixel::average9<rgba>(rgba *arr) throw()
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
            return rgba(average9(r),average9(g),average9(b),average9(a));
        }


    }

}
