//! \file

#ifndef Y_GFX_BITMAP_INCLUDED
#define Y_GFX_BITMAP_INCLUDED 1

#include "y/gfx/bitrows.hpp"
#include "y/gfx/area.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon
{
    namespace graphic
    {
        typedef arc_ptr<const crux::pixels> shared_pixels; //!< aluas
        typedef arc_ptr<const bitrows>      shared_rows;   //!< alis

        class bitmap : public entity, public area
        {
        public:
            explicit bitmap(const unit_t W, const unit_t H, const unit_t D);
            bitmap(const bitmap &other) throw();
            virtual ~bitmap() throw();

            const size_t        depth;  //!< bytes per pixel
            const size_t        stride; //!< stride
            const zflux         zfh;    //!< zero flux/h
            const zflux         zfw;    //!< zero flux/w
            const shared_pixels pixels; //!< raw memory
            const shared_rows   a_rows; //!< anonymous rows
            
            friend std::ostream & operator<<(std::ostream &, const bitmap &);


        private:
            Y_DISABLE_ASSIGN(bitmap);
        };

    }
    
}

#endif
