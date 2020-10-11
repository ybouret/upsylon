

#include "y/jive/syntax/axiom/all.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            

            Node * Node:: AST(Node *node)  throw()
            {
                assert(node);
                const Axiom    &axiom = node->axiom;
                const uint32_t  uuid  = axiom.uuid;
                
                switch(node->kind)
                {
                    case IsInternal: {
                        Node::List &src = node->_List();
                        Node::List  tgt;
                        while(src.size)
                        {
                            Node *sub = AST( src.pop_front() );
                            tgt.push_back(sub);
                        }
                        tgt.swap_with(src);
                    } break;
                        
                    case IsTerminal:
                        if(Terminal::UUID==uuid&&axiom.as<Terminal>().type==Terminal::Univocal)
                        {
                            node->_Lptr()->release();
                        }
                        break;
                }
                return node;
            }

        }
        
    }
    
}


