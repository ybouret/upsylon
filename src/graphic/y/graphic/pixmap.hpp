//! \file

#ifndef Y_GRAPHIC_PIXMAP_INCLUDED
#define Y_GRAPHIC_PIXMAP_INCLUDED 1

#include "y/graphic/surface.hpp"
#include "y/type/args.hpp"

namespace upsylon {

    namespace Graphic {

        //! ABI for AnonymousRow
        template <typename T>
        class Row
        {
        public:
            Y_DECL_ARGS(T,type);      //!< aliases
        private:
            type              *addr;  //!< typed address
        public:
            const Bitmap      &bitmap; //!< source bitmap

            //! inline const access
            inline const_type & operator[]( const unit_t i ) const throw()
            {
                assert(addr);
                assert(i>=0);
                assert(i<bitmap.w);
                return addr[i];
            }

            //! inline access
            inline type & operator[](const unit_t i) throw()
            {
                assert(addr);
                assert(i>=0);
                assert(i<bitmap.w);
                assert(bitmap.mode==Memory::ReadWrite);
                return addr[i];
            }

            //! inline zero flux const access
            inline const_type & operator()(const unit_t i) const throw()
            {
                assert( addr );
                return addr[ bitmap.zfw(i) ];
            }

            //! inline zero flux access
            inline type & operator()(const unit_t i) throw()
            {
                assert( addr );
                assert(bitmap.mode==Memory::ReadWrite);
                return addr[ bitmap.zfw(i) ];
            }

        private:
            Row(); ~Row() throw();
            Y_DISABLE_COPY_AND_ASSIGN(Row);
        };

        namespace Kernel {
            //! tools for Pixmaps
            struct Pixmap
            {
                //! check compatible depths
                static inline Bitmap *CheckBitmap( Bitmap *bmp, const size_t depth );
            };
        }


        //! a bitmap of type
        template <typename T>
        class Pixmap : public Surface
        {
        public:
            Y_DECL_ARGS(T,type);                           //!< aliases
            static const size_t BytesPerPixel = sizeof(T); //!< alias
            typedef Row<T> RowType;                        //!< alias

            //! create a new pixmap
            inline explicit Pixmap(const size_t W, const size_t H) :
            Surface( Bitmap::Create(W,H,sizeof(T)) )
            {
            }

            //! create a pixmap from a new bitmap
            inline explicit Pixmap( Bitmap *bmp ) :
            Surface( Kernel::Pixmap::CheckBitmap(bmp,sizeof(T)) )
            {

            }

            //! direct copy
            inline void copy( const Pixmap &pxm ) throw()
            {
                assert( sameSurfaceThan(pxm) );
                const Bitmap &self = **this;
                const unit_t bytes = self.scanline;
                unit_t       j     = self.h;
                for(--j;j>=0;--j)
                {
                    memcpy( self.stdRow(j)->addr, pxm->stdRow(j)->addr, bytes );
                }
            }

            //! cleanup
            inline virtual ~Pixmap() throw()
            {
            }

            //! const row access
            inline const Row<T> & operator[](const unit_t j) const throw()
            {
                const  Bitmap           &self = **this;
                return *(const Row<T> *)(self.stdRow(j));
            }

            //! row access
            inline Row<T> & operator[](const unit_t j) throw()
            {
                const Bitmap &self = **this;
                assert( self.mode == Memory::ReadWrite );
                return *(Row<T> *)(self.stdRow(j));
            }

            //! const zero flux row
            inline const Row<T> & operator()(const unit_t j) const throw()
            {
                const  Bitmap &self = **this;
                return *(const Row<T> *)(self.zfxRow(j));
            }

            //! zero flux row
            inline  Row<T> & operator()(const unit_t j) throw()
            {
                const  Bitmap &self = **this;
                assert( self.mode == Memory::ReadWrite );
                return *(Row<T> *)(self.zfxRow(j));
            }


        private:
            Y_DISABLE_COPY_AND_ASSIGN(Pixmap);
        };

    }
    
}

#endif

