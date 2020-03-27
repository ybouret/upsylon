#include "y/jargon/xcache.hpp"

namespace upsylon {
    
    namespace Jargon {

        
        XCache:: ~XCache() throw()
        {
        }
        
        XCache:: XCache(size_t n) :
        XList(),
        inactive( new Inactive() )
        {
            while(n-- > 0)
            {
                push_back( XNode::Inactive(inactive) );
            }
        }
        
        void  XCache:: store(XNode *xnode) throw()
        {
            assert(xnode);
            XNode::Release(xnode,*this);
            assert(xnode->isInactive());
        }

        
        XNode *XCache:: query(const Dogma &d, Lexeme *l)
        {
            if(size)
            {
                XNode *xnode = pop_back(); assert(xnode->isInactive());
                xnode->activate(d,l);
                return xnode;
            }
            else
            {
                return XNode::Create(d,l);
            }
        }
        
        XNode *XCache:: query()
        {
            if(size)
            {
                XNode *xnode = pop_back(); assert(xnode->isInactive());
                return xnode;
            }
            else
            {
                return XNode::Inactive(inactive);
            }
        }
        
    }
    
}

