
#include "y/gfx/color/named.hpp"
#include "y/randomized/bits.hpp"
#include "y/hashing/hash64.hpp"
namespace upsylon
{
    namespace graphic
    {

        const rgba named_color:: table[Y_GFX_NAMED_COLORS] =
        {
#include "named-tab.hxx"
        };

        static const size_t named_color_span = Y_GFX_NAMED_COLORS-1;

        rgba named_color:: rand(randomized::bits &ran) throw()
        {
            return table[ ran.range<size_t>(1,named_color_span) ];
        }

        rgba named_color:: get(const size_t tag, const size_t off) throw()
        {
            if(tag<=0)
            {
                return table[Y_BLACK_INDEX];
            }
            else
            {
                return table[ 1 + (off+hashing::hash64::keyIBJ(tag))%named_color_span ];
            }
        }


    }
}
