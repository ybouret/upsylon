//! \file


#ifndef Y_GRAPHIC_COLOR_YUV_INCLUDED
#define Y_GRAPHIC_COLOR_YUV_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace Graphic {

        class YUV
        {
        public:
            static const float Umin;
            static const float Umax;
            static const float Vmin;
            static const float Vmax;

            float y; //!< 0..1
            float u; //!< Umin..Umax
            float v; //!< Vmin..Vmax

            YUV(const float Y=0,const float U=0,const float V=0) throw();
            YUV(const YUV &) throw();
            YUV&operator=(const YUV &) throw();
            
            ~YUV() throw();

            static float toY(const float) throw();
            static float toU(const float) throw();
            static float toV(const float) throw();

        };

    }

}

#endif

