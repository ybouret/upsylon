
#include "y/graphic/pixel.hpp"
#include "y/graphic/color/rgb.hpp"

namespace upsylon {

    namespace Graphic {


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

