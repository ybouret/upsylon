
#include "y/jargon/axiom/compound.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Compound:: ~Compound() throw()
        {
        }
        
      
        
        void Compound:: add(Axiom &axiom)
        {
            push_back( new Member(axiom) );
        }

        
        Compound & Compound:: operator<<( Axiom &axiom )
        {
            add(axiom);
            return *this;
        }
        
        void Compound:: displayMembers(ios::ostream &fp) const
        {
            for(const Member *m=head;m;m=m->next)
            {
                fp(" |_<%s>\n", **(m->axiom.label) );
            }
        }

    }
    
}


