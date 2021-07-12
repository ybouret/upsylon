//! \file

#ifndef Y_GFX_COLOR_CHART_INCLUDED
#define Y_GFX_COLOR_CHART_INCLUDED 1

#include "y/gfx/color/ramp.hpp"


namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! create a color chart from bytes
        //
        //______________________________________________________________________
        class color_chart : public type_to_rgba
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! cleanup
            virtual ~color_chart() throw();

            //! setup
            explicit color_chart(const color_ramp &ramp_);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual unit_t depth() const throw();                   //!< sizeof(uint8_t)
            virtual rgba   operator()(const void *) const throw();  //!< convert

            //__________________________________________________________________
            //
            //members
            //__________________________________________________________________
            color_ramp  ramp; //!< internal ramp
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_chart);
        };




    }

}

#endif

