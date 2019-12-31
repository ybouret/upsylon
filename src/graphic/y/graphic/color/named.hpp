//! \file

#ifndef Y_GRAPHIC_NAMED_COLORS_INCLUDED
#define Y_GRAPHIC_NAMED_COLORS_INCLUDED 1


#include "y/graphic/color/data2rgba.hpp"
#include "y/graphic/color/named.hxx"

namespace upsylon {

    namespace Graphic {

        class NamedColor
        {
        public:
            const char *name;
            const rgb   code;
            NamedColor(const char *n, const rgb c) throw();
            ~NamedColor() throw();
            NamedColor(const NamedColor &other) throw();
            
        private:
            Y_DISABLE_ASSIGN(NamedColor);
        };

        struct NamedColors
        {
            static const NamedColor List[Y_NAMED_COLORS];
            static rgb   GetRGB(  const size_t index ) throw();
            static rgba  GetRGBA( const size_t index ) throw();
        };

        class IndexToRGBA : public Data2RGBA
        {
        public:
            explicit IndexToRGBA(const size_t callIndex=Y_RED_INDEX,
                                 const size_t jumpValue=1) throw();
            virtual ~IndexToRGBA() throw();

            const size_t call;
            const size_t jump;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(IndexToRGBA);
            virtual rgba get(const void *addr) throw();

        };

    }
}

#endif

