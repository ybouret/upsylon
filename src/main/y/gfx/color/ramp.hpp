//! \file

#ifndef Y_GFX_COLOR_RAMP_INCLUDED
#define Y_GFX_COLOR_RAMP_INCLUDED 1

#include "y/gfx/color/type-to-rgba.hpp"
#include "y/sequence/accessible.hpp"
#include "y/sequence/array.hpp"
#include "y/ptr/arc.hpp"
#include "y/object.hpp"
#include "y/ptr/counted.hpp"



namespace upsylon
{

    namespace graphic
    {

        typedef accessible<rgba> color_repository;

        class color_ramp_ : public object, public counted
        {
        public:

            virtual ~color_ramp_() throw();
            virtual const color_repository & repository() const throw() = 0;

            rgba get(const float u) const throw();

        protected:
            explicit color_ramp_() throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_ramp_);
        };


        class fixed_color_ramp : public color_ramp_
        {
        public:
            explicit fixed_color_ramp(const rgba *data, const size_t size) throw();
            virtual ~fixed_color_ramp() throw();
            virtual const color_repository & repository() const throw();

        private:
            const lightweight_array<rgba> repo;
            Y_DISABLE_COPY_AND_ASSIGN(fixed_color_ramp);
        };

        template <typename HOST>
        class color_ramp_of : public fixed_color_ramp
        {
        public:
            inline explicit color_ramp_of()              throw() : fixed_color_ramp( HOST::data, HOST::size ) {}
            inline explicit color_ramp_of(const HOST &h) throw() : fixed_color_ramp( h.data, h.size) {}
            inline virtual ~color_ramp_of()               throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(color_ramp_of);
        };

        class monochrome
        {
        public:
            static const size_t size = 2;
            rgba                data[size];

            monochrome(const rgba &C) throw() : data()
            {
                aliasing::_(data[0]) = rgba(0,0,0);
                aliasing::_(data[1]) = C;
            }

            ~monochrome() throw()
            {
            }

            monochrome(const monochrome &other) throw() : data()
            {
                memcpy( (void*) data, other.data, sizeof(data) );
            }

        private:
            Y_DISABLE_ASSIGN(monochrome);
        };

        struct cold_to_hot
        {
            static const size_t size;
            static const rgba   data[];
        };


        
        class color_ramp : public arc_ptr<color_ramp_>, public type_to_rgba
        {
        public:
            virtual ~color_ramp()    throw();
            color_ramp(const color_ramp&)  throw();
            color_ramp(color_ramp_*) throw();
            color_ramp & operator=(const color_ramp &) throw();

            virtual unit_t depth() const throw();
            virtual rgba   operator()(const void *) const throw();
        };

        


    }

}

#endif

