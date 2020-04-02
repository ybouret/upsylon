#include "y/jargon/axiom/visitor.hpp"

namespace upsylon {
    
    namespace Jargon {
       
        Visitor:: Visitor() : AxiomAddresses()
        {
        }
        
        Visitor:: ~Visitor() throw()
        {
        }
        
        bool Visitor:: wasVisited(const Axiom &axiom) const throw()
        {
            const AxiomAddress addr = &axiom;
            return search_by(addr) != NULL;
        }
        
        bool Visitor:: greenLight( const Axiom &axiom )
        {
            const AxiomAddress addr = &axiom;
            return true == insert_by(addr,addr);
        }

        
    }
    
}

