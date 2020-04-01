

#include "y/jargon/axiom/compound/alternate.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Alternate:: ~Alternate() throw()
        {
        }
        
        bool Alternate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            for(const Member *member=head;member;member=member->next)
            {
                if( (**member).accept(xtree, lexer, source, xcache, accepted) )
                {
                    Y_JAXIOM(std::cerr << "|_[Alternate] <" << label << ">" << std::endl);
                    return true;
                }
            }
            return false;
        }
        
        static inline string * MakeLabel(unsigned &indx)
        {
            const string id   = vformat("%c%u", Alternate::Mark, indx);
            string      *temp = Tags::Make(id);
            ++indx;
            return temp;
        }
        
        Alternate:: Alternate(unsigned &indx) :
        Compound(MakeLabel(indx),UUID)
        {
            self = static_cast<Alternate *>(this);
            Y_JAXIOM(std::cerr << "+[Alternate] <" << label << ">" << std::endl);
        }
    }
    
}

