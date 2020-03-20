
#include "y/jargon/pattern/joker.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        //! Logical operations based pattern
        Joker:: ~Joker() throw()
        {
        }
        
        Joker:: Joker(const uint32_t u, const Motif &m) throw():
        Pattern(u),
        motif(m)
        {
        }
        
        Joker:: Joker( const Joker &J ) :
        Pattern(J),
        motif(J.motif->clone())
        {
        }
        
        
    }
    
}

