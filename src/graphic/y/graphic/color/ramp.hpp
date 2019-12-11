
//! \file
#ifndef Y_GRAPHIC_COLOR_RAMP_INCLUDED
#define Y_GRAPHIC_COLOR_RAMP_INCLUDED 1

#include "y/graphic/color/data2rgba.hpp"

namespace upsylon {

    namespace Graphic {

        //! color ramp interface
        class Ramp  
        {
        public:
            virtual ~Ramp() throw(); //!< cleanup

            //! get the matching color
            rgb operator()(const float v, const float vmin=0.0f, const float vmax=1.0f);

        protected:
            explicit Ramp() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ramp);
            virtual const rgb *_256colors() const throw() = 0;
        };


    }

}


#endif


