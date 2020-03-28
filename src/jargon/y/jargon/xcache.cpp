#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {

        
        XCache:: ~XCache() throw()
        {
        }
        
        
        void XCache:: reserve(size_t n)
        {
            while(n-- > 0)
            {
                push_back( XNode::Create(*dull) );
            }
        }
        
        XNode *XCache:: query()
        {
            return (size>0) ? pop_back() : XNode::Create(*dull);
        }

        XNode *XCache:: query(const Internal &axiom)
        {
            if(size)
            {
                return pop_back()->activate(axiom);
            }
            else
            {
                return XNode::Create(axiom);
            }
        }
        
        XNode *XCache:: query(const Terminal &axiom, Lexeme *lexeme)
        {
            if(size)
            {
                return pop_back()->activate(axiom,lexeme);
            }
            else
            {
                return XNode::Create(axiom,lexeme);
            }
        }

        
        void  XCache:: store(XNode *xnode) throw()
        {
            assert(xnode);
            XNode::Release(xnode,*this);
            assert(xnode->isInactive());
        }
        
    }
    
}

