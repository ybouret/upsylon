
#include "y/ink/color/named-colors.hpp"

namespace upsylon
{
    namespace Ink
    {
        const NamedColor NamedColor::Tableau[] =
        {
#include "named-colors.inc"
        };
        
        const size_t NamedColor::Count = sizeof(Tableau)/sizeof(Tableau[0]);
        
    }
    
}

