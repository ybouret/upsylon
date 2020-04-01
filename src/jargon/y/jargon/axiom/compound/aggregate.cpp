
#include "y/jargon/axiom/compound/aggregate.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {

        Aggregate:: ~Aggregate() throw()
        {
        }
        
        bool Aggregate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            XNode *ltree = xcache.query(*this);
            
            
            return false;
        }
        
        void Aggregate:: setup()
        {
            self = static_cast<Aggregate *>(this);
            Y_JAXIOM(std::cerr << "+[Aggregate] <" << label << ">" << std::endl);
        }

    }
    
}

