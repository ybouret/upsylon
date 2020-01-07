//! \file

#ifndef Y_GRAPHIC_NAMED_COLORS_INCLUDED
#define Y_GRAPHIC_NAMED_COLORS_INCLUDED 1


#include "y/graphic/color/data2rgba.hpp"
#include "y/graphic/color/named.hxx"

namespace upsylon {

    namespace Graphic {

        //! a name and a color
        class NamedColor
        {
        public:
            const char *name; //!< legacy name
            const rgb   code; //!< r,g,b
            NamedColor(const char *n, const rgb c) throw(); //!< setup
            ~NamedColor() throw();                          //!< cleanup
            NamedColor(const NamedColor &) throw();         //!< copy
            
        private:
            Y_DISABLE_ASSIGN(NamedColor);
        };

        //! manage some named colors
        struct NamedColors
        {
            static const NamedColor List[Y_NAMED_COLORS];       //!< built-in colors
            static rgb   GetRGB(  const size_t index ) throw(); //!< RGB  by index, safe
            static rgba  GetRGBA( const size_t index ) throw(); //!< RGBA by index, safe
        };

        //! convert size_t to a color
        class IndexToRGBA : public Data2RGBA
        {
        public:

            explicit IndexToRGBA(const size_t callIndex=Y_RED_INDEX,
                                 const size_t jumpValue=1) throw(); //!< setup
            virtual ~IndexToRGBA() throw();                         //!< cleanup

            const size_t call; //!< for i=1
            const size_t jump; //!< index=(i-1)*jump + call
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(IndexToRGBA);
            virtual rgba get(const void *addr) throw();

        };

    }
}

#endif

