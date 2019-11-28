//! \file

#ifndef Y_GRAPHIC_BITMAP_INCLUDED
#define Y_GRAPHIC_BITMAP_INCLUDED 1

#include "y/graphic/rectangle.hpp"
#include "y/ptr/arc.hpp"
#include "y/hashing/function.hpp"

namespace upsylon {

    namespace Graphic {

        class Bitmap; //!< forward declaration

        //! anonymous row, to map typed Row later
        class AnonymousRow
        {
        public:
            //! setup
            explicit AnonymousRow(void              *address,
                                  const Bitmap      &parent) throw();
            void              *addr;   //!< entry address
            const Bitmap      &bitmap; //!< owner
            const void        *zfx(const unit_t i) const throw(); //!< zero flux index
            const void        *std(const unit_t i) const throw(); //!< standard index

        private:
            ~AnonymousRow() throw();
            Y_DISABLE_COPY_AND_ASSIGN(AnonymousRow);
        };

        //! smart pointer for bytes
        class Bitmap : public Rectangle
        {
        public:
            typedef arc_ptr<Bitmap> Pointer; //!< alias

            virtual ~Bitmap() throw();    //!< cleaunp
            const size_t       depth;     //!< bytes per pixel
            const size_t       scanline;  //!< w * depth
            const size_t       stride;    //!< stride >= scanline
            const Memory::Kind kind;      //!< Global|Static
            const Memory::Mode mode;      //!< ReadWrite|ReadOnly
            const ZeroFlux     zfw;       //!< zero flux for width
            const ZeroFlux     zfh;       //!< zero flux for height

            //! create a new bitmap
            static Bitmap *Create(const size_t W, const size_t H, const size_t BPP);

            //! share a portion of a bitmap
            static Bitmap *Share(const Bitmap &bitmap, const Rectangle &rectangle);

            //! share full bitmap
            static Bitmap *Share(const Bitmap &bitamp);

            //! clone a portion of a bitmap
            static Bitmap *Clone(const Bitmap &bitmap, const Rectangle &rectangle);
            //! clone a full bitmap
            static Bitmap *Clone(const Bitmap &bitmap);


            const AnonymousRow *stdRow(const unit_t j) const throw();                 //!< j in 0..h-1
            const AnonymousRow *zfxRow(const unit_t j) const throw();                 //!< zero flux
            const void         *stdGet(const unit_t i, const unit_t j) const throw(); //!< standard
            const void         *stdGet(const Point &p) const throw();                 //!< standard
            const void         *zfxGet(const unit_t i, const unit_t j) const throw(); //!< zeroFlux
            const void         *zfxGet(const Point &p) const throw();                 //!< zero flux

            void run( hashing::function & ) const throw(); //!< run a hashing function on all data
            void checkWritable(const char *context) const;                    //!< check writable

            void clear(); //!< sequential clear

        private:
            void   *entry;       //!< address of first pixel
        public:
            const size_t  bytes; //!< number of accessible bytes
        private:
            size_t *nref;        //!< shared references for memory

            AnonymousRow *rows;  //!< the rows
            size_t        rlen;  //!< bytes for rows

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


