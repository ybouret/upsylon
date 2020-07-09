
#include "y/spade/layout/fragment.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Spade
    {
        
        namespace Kernel {
          
            void Fragment:: TooManyGhosts(const Coord1D       ng,
                                         const unsigned      level,
                                         const GhostLocation where)
            {
                assert(ng>0);
                const char *text = "";
                switch(where)
                {
                    case InnerGhost: text="inner"; break;
                    case OuterGhost: text="outer"; break;
                }
                throw exception("Spade::Fragment(too many <%s> ghosts (%u) for probe level=%u)", text, int(ng), level);
            }
            
#define Y_SPADE_FRAG_IMPL(ID) const char Fragment:: ID[] = #ID
            
            Y_SPADE_FRAG_IMPL(autoExchange);
            Y_SPADE_FRAG_IMPL(asyncTwoWays);
            Y_SPADE_FRAG_IMPL(asyncForward);
            Y_SPADE_FRAG_IMPL(asyncReverse);

        }
        
    }
    
}

