
//! \file

#ifndef Y_GRAPHIC_BITMAP_INCLUDED
#define Y_GRAPHIC_BITMAP_INCLUDED 1

#include "y/graphic/area.hpp"

namespace upsylon {

    namespace Graphic {

        //! smart pointer for bytes
        class Bitmap : public Area
        {
        public:
            virtual ~Bitmap() throw();

            const size_t       depth;     //!< bytes per pixel
            const size_t       scanline;  //!< w * depth
            const size_t       stride;    //!< stride >= scanline
            const Memory::Kind kind;      //!< Global|Static
            const Memory::Mode mode;      //!< ReadWrite|ReadOnly


            static Bitmap *Create(const size_t W,
                                  const size_t H,
                                  const size_t BPP);
            


        private:
            void   *entry; //!< address of first pixel
            size_t  bytes; //!< number of accessible bytes
            size_t *nref;

            Y_DISABLE_COPY_AND_ASSIGN(Bitmap);

            explicit Bitmap(const Area        &area,
                            const void        *data,
                            const size_t       size,
                            const size_t       bytesPerPixel,
                            const size_t       dataStride,
                            const Memory::Kind memoryKind,
                            const Memory::Mode memoryMode);

        };

    }

}

#endif


