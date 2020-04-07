
#include "y/jargon/axiom/all.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        bool Axiom:: isTerminal() const throw()
        {
            return Terminal::UUID == uuid;
        }
        
        bool Axiom:: isCompound() const throw()
        {
            switch(uuid)
            {
                case Aggregate::UUID:
                case Alternate::UUID: return true;
                default: break;
            }
            return false;
        }
        
        bool Axiom:: isWildcard() const throw()
        {
            switch(uuid)
            {
                case Option::     UUID:
                case ZeroOrMore:: UUID:
                case OneOrMore::  UUID: return true;
                default: break;
            }
            return false;
        }
        
        bool Axiom:: isApparent() const throw()
        {
            
            switch(uuid)
            {
                case Aggregate::UUID: return as<Aggregate>().feature != Aggregate::Vanishing;
                default: break;
            }
            
            return false;
        }
        
    }
    
}

#include "y/jargon/axiom/db.hpp"
namespace upsylon {
    
    namespace Jargon {
        
        

    }
}

