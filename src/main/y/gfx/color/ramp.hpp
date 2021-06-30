//! \file

#ifndef Y_GFX_COLOR_RAMP_INCLUDED
#define Y_GFX_COLOR_RAMP_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"
#include "y/sequence/accessible.hpp"
#include "y/sequence/array.hpp"

namespace upsylon
{

    namespace graphic
    {

        class color_mix : public type_to_rgba
        {
        public:
            typedef accessible<rgba>  repository_type;
            typedef accessible<float> stop_point_type;

            static rgba build(const float u, const repository_type  &arr) throw();
            static rgba build(const float u, const accessible<float> &pos, const repository_type &arr) throw();

            virtual ~color_mix() throw();
            virtual unit_t                  depth() const throw();
            virtual const repository_type & repository() const throw() = 0;

            rgba get(const float u) const throw();


        protected:
            explicit color_mix() throw();


        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_mix);
        };

        

        



    }

}

#endif

