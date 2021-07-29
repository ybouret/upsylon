
#include "y/gfx/pixel.hpp"

namespace upsylon
{
    namespace graphic
    {

        template <>   bool pixel:: is_zero<float>(const float &f) throw()
        {
            return f <= 0.0f;
        }

        template <>   bool pixel:: is_zero<uint8_t>(const uint8_t &u) throw()
        {
            return u<=0;
        }


        template <>   bool pixel:: is_zero<rgb>(const rgb &c) throw()
        {
            return unsigned(c.r) + unsigned(c.g) + unsigned(c.b) <= 0;
        }

        template <>   bool pixel:: is_zero<rgba>(const rgba &c) throw()
        {
            return unsigned(c.r) + unsigned(c.g) + unsigned(c.b) <= 0;
        }


    }

}

namespace upsylon
{
    namespace graphic
    {
        template <> float pixel:: zero<float>()   throw() { return 0.0f; }
        template <> float pixel:: opaque<float>() throw() { return 1.0f; }

        template <> uint8_t pixel:: zero<uint8_t>()   throw() { return 0x00; }
        template <> uint8_t pixel:: opaque<uint8_t>() throw() { return 0xff; }

        template <> double pixel:: zero<double>()   throw() { return 0.0; }
        template <> double pixel:: opaque<double>() throw() { return 1.0; }

        template <> rgb    pixel:: zero<rgb>()   throw() { return rgb(0,0,0); }
        template <> rgb    pixel:: opaque<rgb>() throw() { return rgb(0xff,0xff,0xff); }

        template <> rgba    pixel:: zero<rgba>()   throw() { return rgba(0,0,0,0xff); }
        template <> rgba    pixel:: opaque<rgba>() throw() { return rgba(0xff,0xff,0xff,0xff); }

    }

}

 


#include <cstdlib>

namespace upsylon
{
    namespace graphic
    {

        template <> float pixel:: sub<float>(const float &lhs, const float &rhs) throw()
        {
            return fabsf(lhs-rhs);
        }

        template <> double pixel:: sub<double>(const double &lhs, const double &rhs) throw()
        {
            return fabs(lhs-rhs);
        }

        template <> uint8_t pixel:: sub<uint8_t>(const uint8_t &lhs, const uint8_t &rhs) throw()
        {
            return uint8_t( abs(int(lhs) - int(rhs)) );
        }

        static inline uint8_t ave_of(const float I, const uint8_t x, const uint8_t y) throw()
        {
            const unsigned X = x;
            const unsigned Y = y;
            const float    A  = ( (X+Y)>> 1 );
            return floorf( A * I + 0.5f );
        }


        template <> rgb pixel:: sub<rgb>(const rgb &lhs, const rgb &rhs) throw()
        {
            const float L = convert<float,rgb>::from(lhs);
            const float R = convert<float,rgb>::from(rhs);
            const float I = fabs(L-R);
            return rgb(ave_of(I,lhs.r,rhs.r),
                       ave_of(I,lhs.g,rhs.g),
                       ave_of(I,lhs.b,rhs.b));
        }

    }

}


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
