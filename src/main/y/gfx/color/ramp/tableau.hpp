//! \file

#ifndef Y_GFX_COLOR_RAMP_TABLEAU_INCLUDED
#define Y_GFX_COLOR_RAMP_TABLEAU_INCLUDED 1

#include "y/gfx/color/ramp.hpp"

namespace upsylon
{

    namespace graphic
    {
        //______________________________________________________________________
        //
        //
        //! proxy to convert user data into a repository
        //
        //______________________________________________________________________
        class fixed_color_ramp : public color_ramp_
        {
        public:
            virtual ~fixed_color_ramp() throw();                         //!< cleanup
            virtual const color_repository & repository() const throw(); //!< internal repository

        protected:
            explicit fixed_color_ramp(const rgba *data, const size_t size) throw(); //!< setup

        private:
            const lightweight_array<rgba> repo;
            Y_DISABLE_COPY_AND_ASSIGN(fixed_color_ramp);
        };

        //______________________________________________________________________
        //
        //
        //! use static data from a HOST class to make a color ramp
        //
        //______________________________________________________________________
        template <typename HOST>
        class color_tableau : public fixed_color_ramp
        {
        public:
            //! setup from HOST::data and HOST::size
            inline explicit color_tableau()              throw() : fixed_color_ramp( HOST::data, HOST::size ) {}

            //! setup from HOST.data and HOST.size
            inline explicit color_tableau(const HOST &h) throw() : fixed_color_ramp( h.data, h.size) {}

            //! cleanup
            inline virtual ~color_tableau()              throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_tableau);
        };



    }
}

#endif

