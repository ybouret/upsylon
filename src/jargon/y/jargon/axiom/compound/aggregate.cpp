
#include "y/jargon/axiom/compound/aggregate.hpp"
#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {

        const char Aggregate:: CLID[] = "Aggregate";
        
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
                    if( ! member->axiom.accept(ltree,lexer,source,xcache) )
                    {
                        XNode::Restore(ltree, lexer, xcache);
                        return false;
                    }
                }
                XNode::Advance(xtree,ltree);
                Y_JAXIOM(std::cerr << "+" << CLID << "<" << label << ">" << std::endl);
                return true;
            }
            catch(...)
            {
                XNode::Release(ltree,xcache);
                throw;
            }
        }
        
        static inline string * MakeLabel(unsigned &indx)
        {
            const string id   = vformat("%cagg%u", Compound::DesignMark, indx);
            string      *temp = Tags::Make(id);
            ++indx;
            return temp;
        }
        
        Aggregate:: Aggregate(unsigned &indx) :
        Compound( MakeLabel(indx), UUID )
        {
            setup();
        }

        
        void Aggregate:: setup()
        {
            self = static_cast<Aggregate *>(this);
            Y_JAXIOM(std::cerr << "+" << CLID << " <" << label << ">" << std::endl);
        }

        ios::ostream & Aggregate:: display(ios::ostream &fp) const
        {
            fp("|_[%s<%s>]\n", CLID, **label);
            displayMembers(fp);
            return fp;
        }
    }
    
}

