
#include "y/jargon/axiom/compound/aggregate.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {

        Aggregate:: ~Aggregate() throw()
        {
        }
        
        bool Aggregate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            XNode *ltree = xcache.query(*this);
            try
            {
                for(const Member *member=head;member;member=member->next)
                {                    
                    if( ! (**member).accept(ltree,lexer,source,xcache,accepted) )
                    {
                        XNode::Restore(ltree, lexer, xcache);
                        return false;
                    }
                }
                XNode::Advance(xtree,ltree);
                accepted = this;
                Y_JAXIOM(std::cerr << "|_[Aggregate] <" << label << ">" << std::endl);
                return true;
            }
            catch(...)
            {
                XNode::Release(ltree,xcache);
                throw;
            }
        }
        
        void Aggregate:: setup()
        {
            self = static_cast<Aggregate *>(this);
            Y_JAXIOM(std::cerr << "+[Aggregate] <" << label << ">" << std::endl);
        }

    }
    
}

