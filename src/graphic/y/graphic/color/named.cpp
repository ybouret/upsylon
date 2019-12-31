

#include "y/graphic/color/named.hpp"
#include "y/code/primality.hpp"

namespace upsylon {

    namespace Graphic {

        NamedColor:: NamedColor(const char *n,const rgb c) throw() :
        name(n),
        code(c)
        {
            assert(name!=NULL);
        }

        NamedColor:: ~NamedColor() throw()
        {
        }

        NamedColor:: NamedColor(const NamedColor &other) throw() :
        name( other.name ),
        code( other.code )
        {
        }
        

        const NamedColor NamedColors::List[Y_NAMED_COLORS] =
        {
#include "named.cxx"
        };

        rgb   NamedColors:: GetRGB( const size_t index ) throw()
        {
            return List[ index % Y_NAMED_COLORS ].code;
        }

        rgba   NamedColors:: GetRGBA( const size_t index ) throw()
        {
            return List[ index % Y_NAMED_COLORS ].code;
        }

        IndexToRGBA:: IndexToRGBA(const size_t callIndex,
                                  const size_t jumpValue) throw() :
        call( callIndex ),
        jump( (jumpValue<=1) ? 1 : primality::next(jumpValue) )
        {
        }

        IndexToRGBA:: ~IndexToRGBA() throw()
        {
        }

        rgba IndexToRGBA:: get(const void *addr) throw()
        {
            assert(addr);
            const size_t i = *static_cast<const size_t *>(addr);
            if(i<=0)
            {
                return Y_BLACK;
            }
            else
            {
                return NamedColors::GetRGBA( (i-1)*jump + call );
            }
        }


    }
}

