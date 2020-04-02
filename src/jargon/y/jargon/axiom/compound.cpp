
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
        
        Member:: Member(const Axiom &a) throw() :
        Object(), inode<Member>(), axiom(a)
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
    }
    
}


