
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
            if( axiom.accept(ltree,lexer,source,xcache))
            {
                assert(ltree!=NULL);
                XNode::Advance(xtree,ltree);
                // accepted is node, directly
                Y_JAXIOM(std::cerr << "|_[Option] <" << label << ">" << std::endl);
                return true;
            }
            else
            {
                assert(NULL==ltree);
                return true;
            }
        }
     
        
        ios::ostream & Option:: display(ios::ostream &fp) const
        {
            return fp("|_[Option <%s>]\n", **axiom.label);
        }
    }
    
}

