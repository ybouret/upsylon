
#include "y/jargon/axiom/compound/aggregate.hpp"

namespace upsylon {
    
    namespace Jargon {

        const char                  Aggregate:: CLID[]   = "Aggregate";
        const Aggregate::ActingType Aggregate:: AsActing = {};

        
        Aggregate:: ~Aggregate() throw()
        {
        }
        
        bool Aggregate:: accept(Y_JARGON_AXIOM_ARGS) const
        {
            XNode *ltree = XNode::Create(*this);
            try
            {
                for(const Member *member=head;member;member=member->next)
                {                    
                    if( ! member->axiom.Y_JARGON_AXIOM_ACCEPT(ltree) )
                    {
                        XNode::Restore(ltree, lexer);
                        return false;
                    }
                }
                XNode::Advance(xtree,ltree);
                Y_JAXIOM( if(feature!=Design) std::cerr << "|_[" << CLID << "] <" << label << ">" << std::endl );
                return true;
            }
            catch(...)
            {
                XNode::Release(ltree);
                throw;
            }
        }
        
        static inline string * MakeLabel(unsigned &indx)
        {
            const string id   = vformat("%cagg%u", Compound::VanishingMark, indx);
            string      *temp = Tags::Make(id);
            ++indx;
            return temp;
        }
        
        Aggregate:: Aggregate(unsigned &indx) :
        Compound( MakeLabel(indx), UUID ),
        feature(Design)
        {
            setup();
        }

        void Aggregate:: setup()
        {
            self = static_cast<Aggregate *>(this);
            Y_JAXIOM(std::cerr << "+[" << CLID << "] <" << label << ">" << std::endl);
        }

        ios::ostream & Aggregate:: display(ios::ostream &fp) const
        {
            fp("|_[%s<%s>]\n", CLID, **label);
            displayMembers(fp);
            return fp;
        }
        
        
        
        const char * Aggregate:: vizual() const throw()
        {
            switch(feature)
            {
                case Acting: return "shape=house,style=\"bold,filled,rounded\"";
                case Design: return "shape=house,style=\"bold,dashed\"";
                default:
                    break;
            }
            return "shape=house,style=\"bold,filled\"";
        }
    }
    
}

