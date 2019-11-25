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
            Y_DECL_ARGS(T,type);

        private:
            type              *addr;

        public:
            const Bitmap      &bitmap;

            inline const_type & operator[]( const unit_t i ) const throw()
            {
                assert(addr);
                assert(i>=0);
                assert(i<bitmap.w);
                return addr[i];
            }

            inline type & operator[](const unit_t i) throw()
            {
                assert(addr);
                assert(i>=0);
                assert(i<bitmap.w);
                assert(bitmap.mode==Memory::ReadWrite);
                return addr[i];
            }

            inline const_type & operator()(const unit_t i) const throw()
            {
                assert( addr );
                return addr[ bitmap.zfw(i) ];
            }

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
            struct Pixmap
            {
                static inline Bitmap *CheckBitmap( Bitmap *bmp, const size_t depth );
            };
        }


        template <typename T>
        class Pixmap : public Surface
        {
        public:
            Y_DECL_ARGS(T,type);

            inline explicit Pixmap(const size_t W, const size_t H) :
            Surface( Bitmap::Create(W,H,sizeof(T)) )
            {
            }

            inline explicit Pixmap( Bitmap *bmp ) :
            Surface( Kernel::Pixmap::CheckBitmap(bmp,sizeof(T)) )
            {

            }

            inline virtual ~Pixmap() throw()
            {
            }

            inline const Row<T> & operator[](const unit_t j) const throw()
            {
                const  Bitmap           &self = **this;
                return *(const Row<T> *)(self.stdRow(j));
            }

            inline Row<T> & operator[](const unit_t j) throw()
            {
                const Bitmap &self = **this;
                assert( self.mode == Memory::ReadWrite );
                return *(Row<T> *)(self.stdRow(j));
            }


            inline const Row<T> & operator()(const unit_t j) const throw()
            {
                const  Bitmap &self = **this;
                return *(const Row<T> *)(self.zfxRow(j));
            }

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

