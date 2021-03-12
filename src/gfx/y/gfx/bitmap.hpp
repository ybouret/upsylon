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

        //______________________________________________________________________
        //
        // global definitions
        //______________________________________________________________________
        typedef arc_ptr<const crux::pixels> shared_pixels; //!< alias
        typedef arc_ptr<const bitrows>      shared_rows;   //!< alias

        //______________________________________________________________________
        //
        //! bitmap
        //______________________________________________________________________
        class bitmap : public entity, public area
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! bitmap WxHxD
            explicit bitmap(const unit_t W, const unit_t H, const unit_t D);
            bitmap(const bitmap & ) throw();  //!< no throw copy
            virtual ~bitmap()       throw();  //!< cleanup



            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            friend std::ostream & operator<<(std::ostream &, const bitmap &); //!< display info

            void       *at(const unit_t x, const unit_t y) throw();       //!< address (x,y)
            void       *at(const coord p)                  throw();       //!< address (p.x,p.y)
            const void *at(const unit_t x, const unit_t y) const throw(); //!< address (x,y)
            const void *at(const coord p)                  const throw(); //!< address (p.x,p.y)

            void       *row_address(const unit_t y)       throw(); //!< address of row[y]
            const void *row_address(const unit_t y) const throw(); //!< address of row[y], const
            void       *get_line(const unit_t y)          throw(); //!< address of row[y][0]
            const void *get_line(const unit_t y)    const throw(); //!< address of row[y][0], const

            void        ldz() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const unit_t        depth;    //!< bytes per pixel
            const unit_t        scanline; //!< bytes per line
            const unit_t        stride;   //!< stride >= scanline
            const zflux         zfh;      //!< zero flux/h
            const shared_pixels pixels;   //!< raw memory
            const shared_rows   a_rows;   //!< anonymous rows

        private:
            Y_DISABLE_ASSIGN(bitmap);
        };

    }
    
}

#endif
