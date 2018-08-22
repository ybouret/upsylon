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
            //! different kind of bitmap
            enum memory_model
            {
                memory_is_global, //!< fully allocated
                memory_is_shared, //!< shared bitmap
                memory_from_user  //!< memory from user
            };
            
            void        *entry;    //!< address of first pixel
            void        *rows;     //!< address of first row
            const unit_t w;        //!< width
            const unit_t h;        //!< height
            const unit_t depth;    //!< bytes per pixel > 0
            const unit_t scanline; //!< w*depth
            const unit_t stride;   //!< stride>=scanline
            const unit_t bytes;    //!< pixels*depth
            
            //! memory_is_global bitmap
            explicit bitmap(const unit_t D,const unit_t W,const unit_t H);
            
            //! memory_is_shared bitmap, same layouts
            explicit bitmap( bitmap *bmp );
            
            //! copy with same memory
            bitmap( const bitmap &bmp );
            
            //! release private memory
            virtual ~bitmap() throw();

            //! get line address
            inline void *get_line(const unit_t j) throw()
            {
                assert(j>=0);assert(j<w);
                return (static_cast<row_layout *>(rows)+j)->p;
            }

            //! get line address, const
            inline const void *get_line(const unit_t j) const throw()
            {
                assert(j>=0);assert(j<w);
                return (static_cast<row_layout *>(rows)+j)->p;
            }

            inline void *get(const unit_t i, const unit_t j) throw()
            {
                assert(i>=0);assert(i<w);
                assert(j>=0);assert(j<h);
                return 0;
            }

            inline const void *get(const unit_t i, const unit_t j) const throw()
            {
                assert(i>=0);assert(i<w);
                assert(j>=0);assert(j<h);
                return 0;
            }

            
        private:
            Y_DISABLE_ASSIGN(bitmap);
            //! layout of a pixmap row
            struct row_layout { uint8_t *p; unit_t w; };
            
            bitmap *shared;
            void   *private_memory;
            size_t  private_length;
            
            void allocate();
            void allocate_rows_only();
            void link_rows() throw();
            
        public:
            const memory_model model; //!< memory model for this bitmap
        };
    }
}

#endif

