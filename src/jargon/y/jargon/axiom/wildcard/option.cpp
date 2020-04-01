
#include "y/jargon/axiom/wildcard/option.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Option:: ~Option() throw()
        {
        }
                
        Option:: Option( Axiom &axiom ) :
        Wildcard( MakeTag(Mark,*axiom.label), UUID, axiom )
        {
            self = static_cast<Option *>(this);
            Y_JAXIOM(std::cerr << "+[Option] <" << label << ">" << std::endl);
        }
        
        bool Option:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            XNode       *ltree = NULL;
            const Axiom &axiom = *canon;
            if( axiom.accept(ltree,lexer,source,xcache,accepted))
            {
                assert(ltree!=NULL);
                XNode::Advance(xtree,ltree);
                // accepted is node, directly
                return true;
            }
            else
            {
                assert(NULL==ltree);
                return false;
            }
        }
        
    }
    
}

