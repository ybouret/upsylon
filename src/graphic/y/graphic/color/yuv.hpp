//! \file


#ifndef Y_GRAPHIC_COLOR_YUV_INCLUDED
#define Y_GRAPHIC_COLOR_YUV_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon {

    namespace Graphic {

        //! YUV pixel
        class YUV
        {
        public:
            static const float Umin; //!< -0.436f
            static const float Umax; //!<  0.436f
            static const float Vmin; //!< -0.615f
            static const float Vmax; //!<  0.615f

            float y; //!< 0..1
            float u; //!< Umin..Umax
            float v; //!< Vmin..Vmax

            YUV(const float Y=0,const float U=0,const float V=0) throw(); //!< setup
            YUV(const YUV &) throw();                                     //!< copy
            YUV&operator=(const YUV &) throw();                           //!< assign
            ~YUV() throw();                                               //!< cleanup

            static float toY(const float) throw(); //!< check Y
            static float toU(const float) throw(); //!< check U
            static float toV(const float) throw(); //!< check V

        };

    }

}

#endif

