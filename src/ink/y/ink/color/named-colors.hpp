//! \file
#ifndef Y_INK_NAMED_COLORS_INCLUDED
#define Y_INK_NAMED_COLORS_INCLUDED 1

#include "y/ink/color/named-colors.hxx"
#include "y/ink/color/data2rgba.hpp"

namespace upsylon
{
    namespace Ink
    {
        struct NamedColor
        {
            const char   *name;
            uint8_t r;
            uint8_t g;
            uint8_t b;
            
            static const NamedColor Tableau[];
            static const size_t     Count;
            
            static inline
            RGB Fetch( const size_t indx ) throw()
            {
                const NamedColor &C =  Tableau[(indx+Y_BLACK_INDEX)%Count];
                return RGB(C.r,C.g,C.b);
            }
        };
        
        class index_to_rgba : public data2rgba
        {
        public:
            inline index_to_rgba(const size_t delta=0) throw() :
            shift(delta)
            {
            }
            
            inline virtual ~index_to_rgba() throw()
            {
            }
            
            const size_t shift;
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(index_to_rgba);
             virtual RGBA get(const void *addr) throw()
            {
                assert(addr);
                const size_t indx = *(const size_t *)addr;
                return NamedColor::Fetch(indx);
            }
        };
    }
}

#endif

