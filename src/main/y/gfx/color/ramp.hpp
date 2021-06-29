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

        class color_ramp : public color_mix
        {
        public:
            virtual ~color_ramp() throw();

            virtual rgba operator()(const void*) const throw();

        protected:
            explicit color_ramp() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_ramp);
        };

        class color_map : public color_mix
        {
        public:


        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_map);
        };

        class colors_proxy
        {
        public:
            virtual ~colors_proxy() throw();
            explicit colors_proxy(const rgba *col, const size_t num) throw() : repo( (rgba*)col,num)
            {
            }

        protected:
            const lightweight_array<rgba> repo;
            Y_DISABLE_COPY_AND_ASSIGN(colors_proxy);
        };

        template <typename HOST>
        class fixed_color_ramp : public color_ramp, public colors_proxy
        {
        public:
            inline explicit fixed_color_ramp() throw() :
            color_ramp(),
            colors_proxy(HOST::data,
                         HOST::size)
            {
            }

            inline virtual ~fixed_color_ramp() throw()
            {
                
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(fixed_color_ramp);
        };

        



    }

}

#endif

