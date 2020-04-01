

#include "y/jargon/axiom/compound/alternate.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Alternate:: ~Alternate() throw()
        {
        }
        
        bool Alternate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            return false;
        }
        
        
        void Alternate:: setup()
        {
            self = static_cast<Alternate *>(this);
            Y_JAXIOM(std::cerr << "+[Alternate] <" << label << ">" << std::endl);
        }
    }
    
}

