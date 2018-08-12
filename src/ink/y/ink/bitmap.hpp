//! \file
#ifndef Y_INK_BITMAP_INCLUDED
#define Y_INK_BITMAP_INCLUDED 1

#include "y/ink/area.hpp"
#include "y/ptr/counted.hpp"

namespace upsylon
{
    namespace ink
    {
     
        //! type agnostic bitmap
        class bitmap : public area, public counted
        {
        public:
            enum memory_model
            {
                memory_is_global,
                memory_is_shared,
                memory_from_user
            };
            
            void        *entry;    //!< address of first pixel
            void        *rows;     //!< address of first row
            const unit_t w;        //!< width
            const unit_t h;        //!< height
            const unit_t depth;    //!< bytes per pixel > 0
            const unit_t scanline; //!< w*depth
            const unit_t stride;   //!< stride>=scanline
            const unit_t pixels;   //!< w*h
            const unit_t bytes;    //!< pixels*depth
            
            //! memory_is_global bitmap
            explicit bitmap(const unit_t W,const unit_t H,const unit_t D);
            
            //! memory_is_shared bitmap, same layouts
            explicit bitmap( bitmap *bmp );
            
            //! copy with same memory
            bitmap( const bitmap &bmp );
            
            //! release private memory
            virtual ~bitmap() throw();
            
            
        private:
            Y_DISABLE_ASSIGN(bitmap);
            //! layout of a pixmap row
            struct row_layout { void *p; unit_t w; };
            
            bitmap *shared;
            void   *private_memory;
            size_t  private_length;
            
            void allocate();
            void allocate_rows_only();
            void link_rows() throw();
            
        public:
            const memory_model model;
        };
    }
}

#endif

