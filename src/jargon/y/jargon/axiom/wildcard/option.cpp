
#include "y/jargon/axiom/wildcard/option.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const char Option:: CLID[] = "Option";
        
        Option:: ~Option() throw()
        {
         
        }
                
        Option:: Option( Axiom &axiom ) :
        Wildcard( MakeTag(Mark,*axiom.label), UUID, axiom )
        {
            self = static_cast<Option *>(this);
            Y_JAXIOM(std::cerr << "+" << CLID << "<" << label << ">" << std::endl);
        }
        
        bool Option:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            XNode       *ltree = NULL;
            if( axiom.Y_JARGON_AXIOM_ACCEPT(ltree))
            {
                assert(ltree!=NULL);
                XNode::Advance(xtree,ltree);
                // accepted is node, directly
                Y_JAXIOM(std::cerr << "|_[" << CLID << "]<" << label << ">" << std::endl);
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
            return fp("|_[%s<%s>]\n", CLID, **axiom.label);
        }
        
        const char *Option:: vizual() const throw()
        {
            return "shape=diamond,style=bold";
        }

    }
    
}

