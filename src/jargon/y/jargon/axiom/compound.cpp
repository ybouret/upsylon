
#include "y/jargon/axiom/compound.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Compound:: ~Compound() throw()
        {
            Y_JAXIOM( display(std::cerr,"~Compound") << "$" << size << std::endl);
        }
        
        Member:: ~Member() throw()
        {
        }
        
        Member:: Member(const Canon &canon) throw() :
        Object(), inode<Member>(), Canon(canon)
        {
        }
        
        void Compound:: add(Axiom &axiom)
        {
            // must be a dynamic object
            if(axiom.refcount()<=0)
                throw exception("Jargon::Compound<%s>.add(static Axiom<%s>)",**label,**(axiom.label));
            
            const Canon canon( &axiom );
            push_back( new Member(canon) );
        }

        
        Compound & Compound:: operator<<( Axiom &axiom )
        {
            add(axiom);
            return *this;
        }
    }
    
}


