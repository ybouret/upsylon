#include "y/jargon/grammar.hpp"
#include "y/type/aliasing.hpp"
namespace upsylon {
    
    namespace Jargon {

        XNode *Grammar:: AST( XNode *xnode ) const throw()
        {
            assert(xnode);
            
            switch(xnode->genre)
            {
                case XNode::IsInactive: break;
                case XNode::IsTerminal: return onTerminal(xnode);
                case XNode::IsInternal: return onInternal(xnode);
            }
            return xnode;
        }
        
        XNode * Grammar:: onTerminal(XNode *xnode) const throw()
        {
            assert(xnode);
            assert(xnode->isTerminal());
            
            if( xnode->dogma->as<Terminal>().isDefinite() )
            {
                aliasing::_(xnode->lexeme()).erase();
            }
            
            return xnode;
        }

        XNode *Grammar:: onInternal(XNode *xnode) const throw()
        {
            XList &children = xnode->children();
            
            // first pass: propagate and remove divisions
            {
                XList temp;
                while(children.size)
                {
                    XNode *chld = AST(children.pop_front());
                    if(chld->isTerminal() && chld->dogma->as<Terminal>().isDivision())
                    {
                        xcache.store(chld);
                    }
                    else
                    {
                        temp.push_back(chld);
                    }
                }
                children.swap_with(temp);
            }
            
            // second pass: study
            const Axiom   &axiom = *(xnode->dogma);
            switch( axiom.uuid )
            {
                default:
                    std::cerr << "unhandled [" << fourcc_(axiom.uuid) << "]" << std::endl;
                    break;
            }
            return xnode;
        }

        
    }
    
}
