
#include "y/jargon/axiom/db.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        AxiomDB :: AxiomDB() :
        suffix_tree<Axiom::Address>()
        {
        }
        
        AxiomDB:: ~AxiomDB() throw()
        {
        }
        
        bool AxiomDB:: insert(const Axiom &axiom)
        {
            const Axiom::Address addr = axiom;
            return insert_by(addr,addr);
        }
        
        bool AxiomDB:: search(const Axiom &axiom) const throw()
        {
            const Axiom::Address addr = axiom;
            return search_by(addr) != NULL;
        }



    }
    
}
