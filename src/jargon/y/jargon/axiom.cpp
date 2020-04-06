
#include "y/jargon/axiom.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
     
 
        TermNode:: ~TermNode() throw() {}
        
        TermNode::  TermNode(const Terminal & t) throw() :
        Object(),
        inode<TermNode>(),
        term(t)
        {}
        
        AlphaNode:: AlphaNode(const Axiom &args) throw() :
        axiom(args), terms()
        {}
        
        AlphaNode:: ~AlphaNode() throw()
        {
        }
        
        
        Axiom::~Axiom() throw()
        {
        }
        
        bool Axiom::Verbose = false;
        
#if 0
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
#endif
        
    }
}
