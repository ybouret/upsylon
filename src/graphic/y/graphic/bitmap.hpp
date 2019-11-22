//! \file

#ifndef Y_GRAPHIC_BITMAP_INCLUDED
#define Y_GRAPHIC_BITMAP_INCLUDED 1

#include "y/graphic/rectangle.hpp"
#include "y/ptr/arc.hpp"
#include "y/hashing/function.hpp"

namespace upsylon {

    namespace Graphic {

        class Bitmap;
        
        class AnonymousRow
        {
        public:
            explicit AnonymousRow(void              *address,
                                  const Bitmap      &parent) throw();
            void              *addr;
            const Bitmap      &bitmap;
            const void        *pixel(const unit_t i) const throw();

        private:
            ~AnonymousRow() throw();
            Y_DISABLE_COPY_AND_ASSIGN(AnonymousRow);
        };

        //! smart pointer for bytes
        class Bitmap : public Rectangle
        {
        public:
            typedef arc_ptr<Bitmap> Pointer;
            
            virtual ~Bitmap() throw();

            const size_t       depth;     //!< bytes per pixel
            const size_t       scanline;  //!< w * depth
            const size_t       stride;    //!< stride >= scanline
            const Memory::Kind kind;      //!< Global|Static
            const Memory::Mode mode;      //!< ReadWrite|ReadOnly
            const ZeroFlux     zfw;       //!< zero flux for width
            const ZeroFlux     zfh;       //!< zero flux for height
            
            static Bitmap *Create(const size_t W, const size_t H, const size_t BPP);

            static Bitmap *Share(const Bitmap &bitmap, const Rectangle &rectangle);
            static Bitmap *Share(const Bitmap &bitamp);

            static Bitmap *Clone(const Bitmap &bitmap, const Rectangle &rectangle);
            static Bitmap *Clone(const Bitmap &bitmap);



            const AnonymousRow *row(const unit_t j) const throw(); //! zero flux
            const void         *get(const unit_t i, const unit_t j) const throw(); //! zero flux
            const void         *get(const Point &p) const throw(); //! zero flux

            const AnonymousRow *row__(const unit_t j) const throw(); //!< j in 0..h-1


            void run( hashing::function & ) const throw();

        private:
            void   *entry; //!< address of first pixel
        public:
            const size_t  bytes; //!< number of accessible bytes
        private:
            size_t *nref;  //!< shared refs

            AnonymousRow *rows;
            size_t        rlen;

            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);

            explicit Bitmap(const Rectangle   &rectangle,
                            const void        *data,
                            const size_t       size,
                            const size_t       bytesPerPixel,
                            const size_t       dataStride,
                            const Memory::Kind memoryKind,
                            const Memory::Mode memoryMode);

            void setupRows(void *origin);

            explicit Bitmap(const Bitmap    &bitmap,
                            const Rectangle &rectangle);

        };

    }

}

#endif


