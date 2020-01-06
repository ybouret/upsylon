
#include "y/graphic/color/yuv.hpp"
#include "y/type/utils.hpp"

namespace upsylon {

    namespace Graphic {

        const float YUV:: Umax =  0.436f;
        const float YUV:: Umin = -0.436f;

        const float YUV:: Vmax =  0.615f;
        const float YUV:: Vmin = -0.615f;

        float YUV:: toY(const float Y) throw()
        {
            return clamp<float>(0.0f,Y,1.0f);
        }

        float YUV:: toU(const float U) throw()
        {
            return clamp<float>(Umin,U,Umax);
        }

        float YUV:: toV(const float V) throw()
        {
            return clamp<float>(Vmin,V,Vmax);
        }

        YUV:: ~YUV() throw()
        {
        }

        YUV:: YUV(const float Y,const float U,const float V) throw() :
        y( toY(Y) ),
        u( toU(U) ),
        v( toV(V) )
        {
        }

        YUV:: YUV( const YUV &other ) throw() :
        y( toY(other.y) ),
        u( toU(other.u) ),
        v( toV(other.v) )
        {
        }

        YUV & YUV:: operator=(const YUV &other) throw()
        {
            y = toY(other.y);
            u = toU(other.u);
            v = toV(other.v);
            return *this;
        }

    }

}


