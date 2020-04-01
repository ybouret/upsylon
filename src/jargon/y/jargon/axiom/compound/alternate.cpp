

#include "y/jargon/axiom/compound/alternate.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Alternate:: ~Alternate() throw()
        {
        }
        
        bool Alternate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            for(const Member *member=head;member;member=member->next)
            {
                if( (**member).accept(xtree, lexer, source, xcache, accepted) )
                {
                    return true;
                }
            }
            return false;
        }
        
        
        void Alternate:: setup()
        {
            self = static_cast<Alternate *>(this);
            Y_JAXIOM(std::cerr << "+[Alternate] <" << label << ">" << std::endl);
        }
    }
    
}

