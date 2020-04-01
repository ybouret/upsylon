
#include "y/jargon/axiom/wildcard/repeat.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {
        
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
            Y_JAXIOM(std::cerr << "+[Repeat>=" << minimalCount <<"] <" << label << ">" << std::endl);
        }
        
        bool Repeat:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            size_t           count = 0;
            XNode           *ltree = xcache.query(*this);
            const Axiom     &axiom = *canon;
            try
            {
                while(axiom.accept(ltree,lexer,source,xcache,accepted))
                {
                    ++count;
                }
                
                if(count>=minimalCount)
                {
                    Y_JAXIOM(std::cerr << "|_[Repeat>=" << minimalCount << "]=" << count << "*<" << axiom.label << ">" << std::endl);
                    XNode::Advance(xtree,ltree);
                    return true;
                }
                else
                {
                    XNode::Restore(ltree,lexer,xcache);
                    return false;
                }
            }
            catch(...)
            {
                XNode::Release(ltree,xcache);
                throw;
            }
            return false;
        }
    }
    
}
