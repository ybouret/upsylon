
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
        }
        
    }
    
}

