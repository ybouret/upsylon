

#include "y/jargon/axiom/compound/alternate.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const char Alternate:: CLID[] = "Alternate";

        
        Alternate:: ~Alternate() throw()
        {
        }
        
        bool Alternate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            for(const Member *member=head;member;member=member->next)
            {
                if( member->axiom.Y_JARGON_AXIOM_ACCEPT(xtree) )
                {
                    //Y_JAXIOM(std::cerr << "|_[" << CLID << "] <" << label << ">" << std::endl);
                    return true;
                }
            }
            return false;
        }
        
        static inline string * MakeLabel(unsigned &indx)
        {
            const string id   = vformat("%calt%u", Compound::VanishingMark, indx);
            string      *temp = Tags::Make(id);
            ++indx;
            return temp;
        }
        
        Alternate:: Alternate(unsigned &indx) :
        Compound(MakeLabel(indx),UUID)
        {
            setup();
        }
        
        void Alternate:: setup()
        {
            self = static_cast<Alternate *>(this);
            Y_JAXIOM(std::cerr << "+[" << CLID << "]<" << label << ">" << std::endl);
        }
        
        ios::ostream & Alternate:: display(ios::ostream &fp) const
        {
            fp("|_[%s<%s>]\n", CLID, **label);
            displayMembers(fp);
            return fp;
        }
        
        
        const char * Alternate:: vizual() const throw()
        {
            return "shape=egg,style=bold";
        }
    }
    
}

