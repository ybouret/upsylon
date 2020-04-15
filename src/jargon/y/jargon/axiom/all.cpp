
#include "y/jargon/axiom/all.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
#define Y_JTEXT_FOR(CLASS) \
const char *TextFor::CLASS = CLASS::CLID
       
        Y_JTEXT_FOR(Terminal);
        Y_JTEXT_FOR(Operator);
        Y_JTEXT_FOR(Aggregate);
        Y_JTEXT_FOR(Alternate);
        Y_JTEXT_FOR(OneOrMore);
        Y_JTEXT_FOR(ZeroOrMore);
        
    }
    
}


namespace upsylon {
    
    namespace Jargon {

        void Axiom:: revealParents()
        {
            DB all;
            collectParents(all);
            parents.swap_with(all);
        }

        void Axiom:: collectParents( DB &all ) const
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
                            parent.collectParents(all);
                        }
                        break;
                        
                    case Option::     UUID:
                    case ZeroOrMore:: UUID:
                    case OneOrMore::  UUID:
                    case Alternate::  UUID: parent.collectParents(all); break;
                        
                    default:
                        throw exception("<%s> should be a parent for <%s>", **(parent.label), **label);
                }
            }
            
        }

    }
}

namespace upsylon {
    
    namespace Jargon {
        
       
     
    }
}


