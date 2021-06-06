#include "y/jive/language/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
#if 0
            Node * Node:: AST_Terminal(Node *node) throw()
            {
                assert(node);
                assert(node->state==IsTerminal);
                assert(Terminal::UUID==node->axiom.uuid);
                
                switch(node->axiom.as<Terminal>().type)
                {
                    case Terminal::Univocal:
                        node->lexeme()->release();
                        break;
                    default:
                        break;
                }
                return node;
            }
            
            Node * Node:: AST_Internal(Node *node) throw()
            {
                assert(node);
                assert(node->state==IsInternal);
                List &leaves = node->leaves();
                List  output;
                while(leaves.size)
                {
                    Node *child = AST(leaves.pop_front());
                    
                    output.push_back(child);
                }
                leaves.swap_with(output);
                return node;
            }
#endif
            
            Node * Node:: AST(Node *node) throw()
            {
                assert(node);
                
                switch(node->state)
                {
                        //------------------------------------------------------
                        //
                        // process terminal
                        //
                        //------------------------------------------------------
                    case IsTerminal:
                    {
                        switch(node->axiom.as<Terminal>().type)
                        {
                            case Terminal::Univocal:
                                node->lexeme()->release();
                                break;
                            default:
                                break;
                        }
                        return node;
                    }
                        
                        //------------------------------------------------------
                        //
                        // process internal
                        //
                        //------------------------------------------------------
                    case IsInternal:
                    {
                        List &leaves = node->leaves();
                        List  output;
                        while(leaves.size)
                        {
                            Node *child = AST(leaves.pop_front());
                            output.push_back(child);
                        }
                        
                        output.swap_with(leaves);
                        return node;
                    }
                }
            }
        }
    }
}

