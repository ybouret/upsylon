
#include "y/jargon/axiom/wildcard.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Wildcard:: ~Wildcard() throw()
        {
        }
        
        Axiom * Wildcard:: checkShared(Axiom &axiom) const
        {
            if( axiom.refcount() <=0 )
                throw exception("Jargon::Wildcard<%s>(static Axiom<%s>", **label, ** axiom.label);
            return &axiom;
        }

        const Axiom & Wildcard:: operator*() const throw()
        {
            return *canon;
        }

        string * Wildcard::MakeTag( const char mark, const string &name )
        {
            const string id = mark + name;
            return Tags::Make(id);
        }
        
        
        
    }
    
}

