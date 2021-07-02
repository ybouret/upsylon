//! \file

#ifndef Y_GFX_COLOR_RAMP_INCLUDED
#define Y_GFX_COLOR_RAMP_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"
#include "y/sequence/accessible.hpp"
#include "y/sequence/array.hpp"
#include "y/ptr/arc.hpp"
#include "y/ptr/counted.hpp"
#include <cstring>


namespace upsylon
{

    namespace graphic
    {

        //______________________________________________________________________
        //
        //
        //! repository of colors
        //
        //______________________________________________________________________
        typedef accessible<rgba> color_repository;

        //______________________________________________________________________
        //
        //
        //! allocated color ramp interface
        //
        //______________________________________________________________________
        class color_ramp_ : public virtual counted_object
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~color_ramp_() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual const color_repository & repository() const throw() = 0; //!< access colors

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________
            //! interpolated color with 0<=u<=1
            rgba get(const float u) const throw();

        protected:
            explicit color_ramp_() throw(); //!< setup

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_ramp_);
        };

        //______________________________________________________________________
        //
        //
        //! color ramp interface
        //
        //______________________________________________________________________
        class color_ramp : public arc_ptr<color_ramp_>, public type_to_rgba
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~color_ramp()    throw();                    //!< leanup
            color_ramp(const color_ramp&)  throw();              //!< no-throw copy
            color_ramp(color_ramp_*) throw();                    //!< setup
            color_ramp & operator=(const color_ramp &) throw();  //!< no-throw assign

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            virtual unit_t depth() const throw();                   //!< sizeof(float)
            virtual rgba   operator()(const void *) const throw();  //!< get(float)
        };

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

