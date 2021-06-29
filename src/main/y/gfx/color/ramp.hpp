//! \file

#ifndef Y_GFX_COLOR_RAMP_INCLUDED
#define Y_GFX_COLOR_RAMP_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"

namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! interface for color ramps
        //
        //______________________________________________________________________
        class color_ramp
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~color_ramp() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual size_t      dimensions() const throw() = 0; //!< registered colors
            virtual const rgba *repository() const throw() = 0; //!< first color address

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            rgba get(const float u) const throw(); //!< interpolate rgba with 0<=u<=1

        protected:
            explicit color_ramp() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_ramp);
            rgba closest(const float u, const rgba *, const size_t) const throw();
            rgba blended(const float u, const rgba *, const size_t) const throw();
            
#if 0
        public:

            //__________________________________________________________________
            //
            //! mapping floats to a color ramp
            //__________________________________________________________________
            class mapping : public type_to_rgba
            {
            public:
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~mapping() throw();                             //!< cleanup
                explicit mapping(const color_ramp &ramp_,
                                 const float       vmin_=0.0f,
                                 const float       vmax_=1.0f) throw(); //!< setup

                //__________________________________________________________________
                //
                // interface
                //__________________________________________________________________
                virtual unit_t depth()                  const throw(); //!< sizeof(float)
                virtual rgba   operator()(const void *) const throw(); //!< using ramp..

            private:
                Y_DISABLE_COPY_AND_ASSIGN(mapping);
                const float       vmin;
                const float       vmax;
                const float       scal;
                const color_ramp &ramp;
            };
#endif
        };



    }

}

#endif

