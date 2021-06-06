#include "y/jive/language/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            
            
            static inline void xAppend(Node::List &output, Node *child) throw()
            {
                output.push(child);
            }
            
            static inline void xFusion(Node::List &output, Node *child) throw()
            {
                output.merge_back(child->leaves());
                Node::Release(child);
            }
            
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
                    } break;
                        
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
                            // recursive application
                            Node *child = AST(leaves.pop_front());
                            
                            // post-process
                            switch(child->state)
                            {
                                    //------------------------------------------
                                    //
                                    // process terminal child
                                    //
                                    //------------------------------------------
                                case IsTerminal:
                                    switch(child->axiom.as<Terminal>().type)
                                    {
                                        case Terminal::Standard:
                                        case Terminal::Univocal:
                                            xAppend(output,child);
                                            break;
                                        
                                        case Terminal::Division:
                                            Node::Release(child);
                                            break;
                                    }
                                    break;
                                    
                                    //------------------------------------------
                                    //
                                    // process internal child
                                    //
                                    //------------------------------------------
                                case IsInternal:
                                    switch(child->axiom.uuid)
                                    {
                                        case Repeat::UUID:
                                            xFusion(output,child);
                                            break;
                                            
                                        case Aggregate::UUID:
                                            switch(child->axiom.as<Aggregate>().type)
                                            {
                                                case Aggregate::Standard:
                                                    output.push(child);
                                                    break;
                                                    
                                                case Aggregate::Variadic:
                                                    if(child->leaves().size==1)
                                                    {
                                                        xFusion(output,child);
                                                    }
                                                    else
                                                    {
                                                        xAppend(output,child);
                                                    }
                                                    break;
                                                    
                                                case Aggregate::Grouping:
                                                    xFusion(output,child);
                                                    break;
                                            }
                                            break;
                                            
                                        default:
                                            output.push(child);
                                            break;
                                    }
                                    break;
                            }
                        }
                        output.swap_with(leaves);
                    } break;
                }
                return node;
            }
            
            
        }
    }
}

