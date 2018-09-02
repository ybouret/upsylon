//! \file
#ifndef Y_INK_NAMED_COLORS_INCLUDED
#define Y_INK_NAMED_COLORS_INCLUDED 1

#include "y/ink/color/named-colors.hxx"
#include "y/ink/color/data2rgba.hpp"
#include "y/code/primality.hpp"

namespace upsylon
{
    namespace Ink
    {
        //! a named color
        struct NamedColor
        {
            const char *name; //!< its name
            uint8_t     r;    //!< red
            uint8_t     g;    //!< green
            uint8_t     b;    //!< blue
            
            static const NamedColor Tableau[]; //!< compiled data
            static const size_t     Count;     //!< number of colors

            //! fetch by index, 0 is black
            static inline
            RGB Fetch( const size_t indx ) throw()
            {
                const NamedColor &C =  Tableau[(indx+Y_BLACK_INDEX)%Count];
                return RGB(C.r,C.g,C.b);
            }
        };

        //! convert index to RGBA, see Blobs for usage
        class index_to_rgba : public data2rgba
        {
        public:
            //! constructor
            inline index_to_rgba(const size_t delta=0) throw() :
            shift(primality::next(delta))
            {
            }
            //! desctructor
            inline virtual ~index_to_rgba() throw()
            {
            }
            
            const size_t shift; //!< prime multiplier for color scan
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(index_to_rgba);
             virtual RGBA get(const void *addr) throw()
            {
                assert(addr);
                const size_t indx = *(const size_t *)addr;
                return NamedColor::Fetch(indx*shift);
            }
        };
    }
}

#endif

