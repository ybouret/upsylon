
#include "y/jargon/axiom/compound.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Compound:: ~Compound() throw()
        {
        }
        
        Member:: ~Member() throw()
        {
        }
        
        Member:: Member(const Dogma &dogma) throw() :
        Object(), inode<Member>(), Dogma(dogma)
        {
        }
        
        
        Compound & Compound:: operator<<( Axiom &axiom )
        {
            static const char fn[] = "Jargon::Compound";
            static const char ax[] = "Axiom";
            
            // must be a dynamic object
            if(axiom.refcount()<=0)
                throw exception("%s<%s> << (static %s<%s>)",fn,**label,ax,**(axiom.label));
            
            const Dogma dogma = &axiom;
            push_back( new Member(dogma) );
            return *this;
        }
    }
    
}


