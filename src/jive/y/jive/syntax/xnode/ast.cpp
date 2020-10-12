

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
                switch(node->kind)
                {
                    case IsInternal: node = AST_Internal(node); break;
                    case IsTerminal: node = AST_Terminal(node); break;
                }
                return node;
            }

            Node * Node:: AST_Internal(Node *node) throw()
            {
                assert(node);
                assert(node->kind==IsInternal);

                Node::List &src = node->_List();
                Node::List  tgt;
                while(src.size)
                {
                    Node *sub = AST( src.pop_front() );

                    switch(sub->kind)
                    {
                        case IsTerminal: {
                            const Axiom &axiom = sub->axiom;
                            if( axiom.isTerminal() && (axiom.as<Terminal>().type==Terminal::Division) )
                            {
                                Release(sub);
                            }
                            else
                            {
                                tgt.push_back(sub);
                            }
                        } break;

                        case IsInternal:
                            tgt.push_back(sub);
                            break;
                    }


                }
                tgt.swap_with(src);
                return node;
            }

            Node * Node:: AST_Terminal(Node *node)  throw()
            {
                assert(node);
                const Axiom &axiom = node->axiom;
                if( axiom.isTerminal() && (axiom.as<Terminal>().type == Terminal::Univocal) )
                {
                    node->_Lptr()->release();
                }
                return node;
            }

        }
        
    }
    
}


