
#include "y/jargon/axiom/wildcard/repeat.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const char Repeat::CLID[] = "Repeat";
        
        Repeat:: ~Repeat() throw()
        {
            
        }
        
        Repeat:: Repeat(Axiom         &axiom,
                        const size_t   nmin,
                        const uint32_t user,
                        const char     mark) :
        Wildcard( MakeTag(mark,*axiom.label), user, axiom ),
        minimalCount(nmin)
        {
            self = static_cast<Repeat *>(this);
            Y_JAXIOM(std::cerr << "|_[" << CLID << "] >=" << minimalCount <<"*<" << label << ">" << std::endl);
        }
        
        bool Repeat:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            size_t           count = 0;
            XNode           *ltree = XNode::Create(*this);
            try
            {
                while(axiom.Y_JARGON_AXIOM_ACCEPT(ltree))
                {
                    ++count;
                }
                
                if(count>=minimalCount)
                {
                    XNode::Combine(xtree,ltree);
                    return true;
                }
                else
                {
                    XNode::Restore(ltree,lexer);
                    return false;
                }
            }
            catch(...)
            {
                delete ltree;
                throw;
            }
            return false;
        }
        
        ios::ostream & Repeat:: display(ios::ostream &fp) const
        {
            return fp("|_[%s>=%u*<%s>]\n", CLID, unsigned(minimalCount), **axiom.label);
        }

        const char * Repeat:: vizual() const throw()
        {
            return "shape=octagon,style=bold";
        }
        
    }
    
}
