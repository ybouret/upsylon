
//! \file

#ifndef Y_GFX_COLOR_MAP_INCLUDED
#define Y_GFX_COLOR_MAP_INCLUDED 1

#include "y/gfx/color/ramp.hpp"


namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! create a color mapping vmin to vmax from a color ramp
        //
        //______________________________________________________________________
        class color_map : public type_to_rgba
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~color_map() throw();

            //! setup
            explicit color_map(const color_ramp &ramp_,
                               const float       vmin_,
                               const float       vmax_) throw();

            color_ramp  ramp; //!< internal ramp
            const float vmin; //!< minimal value
            const float vmax; //!< maximal value
            const float scal; //!< scaling


            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            float          v2u(const float v) const throw();        //!< value to unit float
            virtual unit_t depth() const throw();                   //!< sizeof(float)
            virtual rgba   operator()(const void *) const throw();  //!< rescaled get
        };




    }

}

#endif

