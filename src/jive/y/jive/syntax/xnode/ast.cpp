

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

                //const Axiom &top = node->axiom;
                Node::List  &src = node->_List();
                Node::List   tgt;
                while(src.size)
                {
                    Node::Pointer child( AST( src.pop_front() ) );
                    const Axiom  &axiom  = child->axiom;
                    switch(child->kind)
                    {
                            //--------------------------------------------------
                            // a terminal in the leaves
                            //--------------------------------------------------
                        case IsTerminal: {
                            if( axiom.isTerminal() && (axiom.as<Terminal>().type==Terminal::Division) )
                            {
                                // do nothing
                            }
                            else
                            {
                                tgt.push_back(child.yield());
                            }
                        } break;

                            //--------------------------------------------------
                            // an internal in the leaves
                            //--------------------------------------------------
                        case IsInternal:
                            switch(axiom.uuid)
                            {
                                case Repeat::UUID:
                                    tgt.merge_back( child->leaves() );
                                    break;

                                case Aggregate::UUID: {
                                    assert(child->kind==IsInternal);
                                    const Aggregate::Type &t = axiom.as<Aggregate>().type;
                                    switch(t)
                                    {
                                        case Aggregate::Standard:
                                            tgt.push_back(child.yield());
                                            break;

                                        case Aggregate::Grouping:
                                            tgt.merge_back( child->leaves() );
                                            break;

                                        case Aggregate::Variadic:
                                            switch(child->leaves().size)
                                            {
                                                case  0: break;
                                                case  1: tgt.push_back( child->leaves().pop_front() ); break;
                                                default: tgt.push_back(child.yield()); break;
                                            }
                                            break;
                                    }

                                } break;

                                default:
                                    tgt.push_back(child.yield());
                            }
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


