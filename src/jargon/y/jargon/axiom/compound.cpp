
#include "y/jargon/axiom/compound.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Member:: ~Member() throw()
        {
        }
        
        Member:: Member(const Axiom &args) throw()  :
        axiom(args)
        {
        }
        
        Compound:: ~Compound() throw()
        {
        }
        
      
        
        void Compound:: add(Axiom &axiom)
        {
            push_back( new Member(axiom) );
            aliasing::_(tail->axiom.parents).secure(*this);
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


