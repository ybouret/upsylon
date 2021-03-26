//! \file

#ifndef Y_GFX_COLOR_RAMP_INCLUDED
#define Y_GFX_COLOR_RAMP_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"

namespace upsylon
{

    namespace graphic
    {

        class color_ramp
        {
        public:
            virtual ~color_ramp() throw();

            virtual size_t      dimensions() const throw() = 0;
            virtual const rgba *repository() const throw() = 0;

            //! interpolate rgba with 0<=u<=1
            rgba get(const float u) const throw();


        protected:
            explicit color_ramp() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_ramp);

        public:
            class mapping : public type_to_rgba
            {
            public:
                virtual ~mapping() throw();
                explicit mapping(const color_ramp &ramp_,
                                 const float       vmin_=0.0f,
                                 const float       vmax_=1.0f) throw();

                virtual unit_t depth()                  const throw(); //!< sizeof(float)
                virtual rgba   operator()(const void *) const throw(); //!< using ramp..


                const float       vmin;
                const float       vmax;
                const float       scal;
                const color_ramp &ramp;
            private:
                Y_DISABLE_COPY_AND_ASSIGN(mapping);
            };
        };



    }

}

#endif

