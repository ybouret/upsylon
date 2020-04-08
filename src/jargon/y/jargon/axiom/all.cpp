
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
                case Aggregate::UUID: return as<Aggregate>().feature != Aggregate::Design;
                default: break;
            }
            
            return false;
        }
        
    }
    
}

#include "y/container/task.hpp"
#include "y/sequence/vector.hpp"
#include "y/memory/pooled.hpp"

namespace upsylon {
    
    namespace Jargon {

        void Axiom:: reveal()
        {
            DB all;
            collect(all);
            parents.swap_with(all);
        }

        void Axiom:: collect( DB &all ) const
        {
            for(DB::const_iterator i = parents.begin(); i != parents.end(); ++i )
            {
                const Axiom &parent = *(*i).as<Axiom>();
                if(all.search(parent)) continue;
                
                switch(parent.uuid)
                {
                    case Aggregate::UUID:
                        if(parent.as<Aggregate>().feature!=Aggregate::Design)
                        {
                            all.secure(parent);
                        }
                        else
                        {
                            //forward
                            parent.collect(all);
                        }
                        break;
                        
                    case Option::     UUID:
                    case ZeroOrMore:: UUID:
                    case OneOrMore::  UUID:
                    case Alternate::  UUID: parent.collect(all); break;
                        
                    default:
                        throw exception("<%s> should be a parent for <%s>", **(parent.label), **label);
                }
            }
            
        }

        
        
    }
}

