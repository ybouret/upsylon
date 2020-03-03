
#include "y/graphic/pixel.hpp"
#include "y/graphic/color/rgb.hpp"

namespace upsylon {

    namespace Graphic {

        const Point Pixel::delta[8] =
        {
            Point(-1,0),
            Point(1,0),
            Point(0,-1),
            Point(0,1),

            Point(1,1),
            Point(1,-1),
            Point(-1,-1),
            Point(-1,1)

        };

        template <>
        void Pixel:: Zero<uint8_t>( uint8_t &x ) throw()
        {
            x=0;
        }

        template <>
        void Pixel:: Zero<float>( float &x ) throw()
        {
            x=0;
        }

        template <>
        void Pixel:: Zero<rgb>( rgb &C ) throw()
        {
            C.r = C.g = C.b = 0;
        }


        template <>
        void Pixel:: Zero<rgba>( rgba &C ) throw()
        {
            C.r = C.g = C.b = 0;
        }

        


    }

}

namespace upsylon {

    namespace Graphic {


        template <>
        bool Pixel:: IsZero<uint8_t>( const uint8_t &x ) throw()
        {
            return x<=0;
        }


        template <>
        bool Pixel:: IsZero<float>( const float &x ) throw()
        {
            return x<=0.0f;
        }

        template <>
        bool Pixel:: IsZero<rgb>( const rgb &C ) throw()
        {
            return (C.r+C.g+C.b)<=0;
        }

        template <>
        bool Pixel:: IsZero<rgba>( const rgba &C ) throw()
        {
            return (C.r+C.g+C.b)<=0;
        }


    }

}


namespace upsylon {

    namespace Graphic {


        template <>
        uint8_t Pixel:: Invert<uint8_t>( const uint8_t &x ) throw()
        {
            return 0xff-x;
        }

        template <>
        float Pixel:: Invert<float>( const float &x ) throw()
        {
            return 1.0f-x;
        }

        template <>
        rgb Pixel:: Invert<rgb>( const rgb &C ) throw()
        {
            return rgb(255-C.r,255-C.g,255-C.b);
        }

        template <>
        rgba Pixel:: Invert<rgba>( const rgba &C ) throw()
        {
            return rgba(255-C.r,255-C.g,255-C.b);
        }


    }

}

namespace upsylon {

    namespace Graphic {

        template < >
        uint8_t Pixel:: Median9<uint8_t>( uint8_t arr[] ) throw()
        {
            return Median9__(arr);
        }

        template < >
        float Pixel:: Median9<float>( float arr[] ) throw()
        {
            return Median9__(arr);
        }

        template <>
        rgb Pixel:: Median9( rgb arr[] ) throw()
        {
            return Median9_<rgb,uint8_t,3>( arr );
        }

        template <>
        rgba Pixel:: Median9( rgba arr[] ) throw()
        {
            return Median9_<rgba,uint8_t,4>( arr );
        }



    }
}

namespace upsylon {

    namespace Graphic {

        float Pixel:: Average9__( float   arr[] ) throw()
        {
            nwsrt::on(arr,comparison::increasing<float>);
            float sum = arr[0];
            sum += arr[1];
            sum += arr[2];
            sum += arr[3];
            sum += arr[4];
            sum += arr[5];
            sum += arr[6];
            sum += arr[7];
            sum += arr[8];
            return sum/9.0f;
        }

        uint8_t Pixel:: Average9__( uint8_t arr[] ) throw()
        {
            unsigned sum = arr[0];
            sum += arr[1];
            sum += arr[2];
            sum += arr[3];
            sum += arr[4];
            sum += arr[5];
            sum += arr[6];
            sum += arr[7];
            sum += arr[8];
            return uint8_t(sum/9);
        }

        template < >
        float Pixel::Average9<float>( float arr[] ) throw()
        {
            return Average9__(arr);
        }

        template < >
        uint8_t Pixel::Average9<uint8_t>( uint8_t arr[] ) throw()
        {
            return Average9__(arr);
        }

        template <>
        rgb Pixel:: Average9( rgb arr[] ) throw()
        {
            return Average9_<rgb,uint8_t,3>( arr );
        }

        template <>
        rgba Pixel:: Average9( rgba arr[] ) throw()
        {
            return Average9_<rgba,uint8_t,4>( arr );
        }


    }

}

#include "y/graphic/convert.hpp"

namespace upsylon {

    namespace Graphic {

        template <>
        float Pixel:: MinOf<float>( const float &lhs, const float &rhs ) throw()
        {
            return (lhs<rhs) ? lhs : rhs;
        }

        template <>
        uint8_t Pixel:: MinOf<uint8_t>( const uint8_t &lhs, const uint8_t &rhs ) throw()
        {
            return (lhs<rhs) ? lhs : rhs;
        }

        template <>
        rgb Pixel:: MinOf<rgb>( const rgb &lhs, const rgb &rhs ) throw()
        {
            return (Convert::GreyScale(lhs)<Convert::GreyScale(rhs)) ? lhs : rhs;
        }

        template <>
        rgba Pixel:: MinOf<rgba>( const rgba &lhs, const rgba &rhs ) throw()
        {
            return (Convert::GreyScale(lhs)<Convert::GreyScale(rhs)) ? lhs : rhs;
        }


    }

}

namespace upsylon {

    namespace Graphic {

        template <>
        float Pixel:: MaxOf<float>( const float &lhs, const float &rhs ) throw()
        {
            return (lhs<rhs) ? rhs : lhs;
        }

        template <>
        uint8_t Pixel:: MaxOf<uint8_t>( const uint8_t &lhs, const uint8_t &rhs ) throw()
        {
            return (lhs<rhs) ? rhs : lhs;
        }

        template <>
        rgb Pixel:: MaxOf<rgb>( const rgb &lhs, const rgb &rhs ) throw()
        {
            return (Convert::GreyScale(lhs)<Convert::GreyScale(rhs)) ? rhs : lhs;
        }

        template <>
        rgba Pixel:: MaxOf<rgba>( const rgba &lhs, const rgba &rhs ) throw()
        {
            return (Convert::GreyScale(lhs)<Convert::GreyScale(rhs)) ? rhs : lhs;
        }


    }

}

namespace upsylon {

    namespace Graphic {

        template <>
        uint8_t Pixel:: Blend<uint8_t>(const uint8_t &fg,
                                       const unsigned wf,
                                       const uint8_t &bg,
                                       const unsigned wb ) throw()
        {
            const unsigned F = fg;
            const unsigned B = bg;
            return uint8_t( (F*wf+B*wb)/255 );
        }

        template <>
        float Pixel:: Blend<float>(const float   &fg,
                                   const unsigned wf,
                                   const float   &bg,
                                   const unsigned wb ) throw()
        {
            return (fg*wf+bg*wb)/255.0f;
        }


        template <>
        uint8_t Pixel:: Blend<uint8_t>( const uint8_t &fg, const uint8_t &bg, const uint8_t alpha ) throw()
        {
            return BlendType<uint8_t, uint8_t, 1>(fg, bg, alpha);
        }

        template <>
        rgb Pixel:: Blend<rgb>( const rgb &fg, const rgb &bg, const uint8_t alpha ) throw()
        {
            return BlendType<rgb, uint8_t,3>(fg, bg, alpha);
        }

        template <>
        rgba Pixel:: Blend<rgba>( const rgba &fg, const rgba &bg, const uint8_t alpha ) throw()
        {
            return BlendType<rgba, uint8_t,3>(fg, bg, alpha);
        }

        template <>
        float Pixel:: Blend<float>( const float &fg, const float &bg, const uint8_t alpha ) throw()
        {
            return BlendType<float,float, 1>(fg, bg, alpha);
        }

    }
}



