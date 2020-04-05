
#include "y/jargon/axiom.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
     
#if 0
        Member:: ~Member() throw()
        {
        }
        
        Member:: Member(const Axiom &a) throw() :
        Object(), inode<Member>(), axiom(a)
        {
        }
#endif
        
        Axiom::~Axiom() throw()
        {
        }
        
        bool Axiom::Verbose = false;
        
        void Axiom:: checkLabel(const char *which) const
        {
            assert(which);
            const string id = **label;
            if(id.size()>0)
            {
                switch(id[0])
                {
                    case '+':
                    case '?':
                    case '*':
                    case '#':
                        throw exception("Jargon::%s(invalid label <%s>)",which,*id);
                        
                    default:
                        break;
                }
            }
        }

    }
}
