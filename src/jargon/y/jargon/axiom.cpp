
#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
     
        std::ostream & Axiom:: display( std::ostream &os, const char *id) const
        {
            assert(id);
            os << id << "<" << label << "> [" << fourcc_(uuid) <<"] @" << refcount();
            return os;
        }

        
        Axiom::~Axiom() throw()
        {
            Y_JAXIOM( display(std::cerr,"~Axiom") << std::endl);
        }
        
        bool Axiom::Verbose = false;
        
    }
}
