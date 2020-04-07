
#include "y/jargon/axiom.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Axiom::DB :: DB() : DB_Type()
        {
        }
        
        Axiom:: DB:: ~DB() throw()
        {
        }
        
        bool Axiom::DB:: insert(const Axiom &axiom)
        {
            const Axiom::Address addr = axiom;
            return insert_by(addr,addr);
        }
        
        bool Axiom:: DB:: search(const Axiom &axiom) const throw()
        {
            const Axiom::Address addr = axiom;
            return search_by(addr) != NULL;
        }



    }
    
}
