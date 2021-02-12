
#include "y/gfx/color/convert.hpp"

namespace upsylon
{
    namespace GFX
    {
        
        template <> template <>
        uint8_t Convert<rgb>:: To<uint8_t>(const rgb &c) throw()
        {
            return Kernel::Conv::Table[ unsigned(c.r) + unsigned(c.g) + unsigned(c.b) ];
        }
        
        template <> template <>
        float Convert<rgb>:: To<float>(const rgb &c) throw()
        {
            return Kernel::Conv::UnitFloat[ To<uint8_t>(c) ];
        }
        
        template <> template <>
        rgba Convert<rgb>:: To<rgba>(const rgb &c) throw()
        {
            return c;
        }

    }
    
}


