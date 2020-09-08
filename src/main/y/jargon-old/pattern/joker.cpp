
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
            assert(motif->strong());
        }
        
        Joker:: Joker( const Joker &J ) :
        object(),
        Pattern(J),
        motif(J.motif->clone())
        {
            assert(motif->strong());
        }
        
        const char Joker::feeble_pattern[] = "feeble pattern";
        
        void Joker:: adjoin(FirstChars &fc)      const
        {
            motif->adjoin(fc);
        }

    }
    
}
