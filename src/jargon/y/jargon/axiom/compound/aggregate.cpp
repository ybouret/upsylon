
#include "y/jargon/axiom/compound/aggregate.hpp"

namespace upsylon {
    
    namespace Jargon {

        Aggregate:: ~Aggregate() throw()
        {
        }
        
        bool Aggregate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            return false;
        }
        
        void Aggregate:: setup()
        {
            self = static_cast<Aggregate *>(this);
            Y_JAXIOM(std::cerr << "+[Aggregate] <" << label << ">" << std::endl);
        }

    }
    
}

