
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            
            namespace
            {
                static inline
                void AST_Terminal( Node *sub, Node::List &temp )
                {
                    if(sub->rule.as<Terminal>().attr==Semantic)
                    {
                        delete sub;
                    }
                    else
                    {
                        temp.push_back(sub);
                    }
                }

                static inline
                void AST_Internal( Node *sub, Node::List &temp )
                {
                    if(sub->rule.uuid == Aggregate::UUID )
                    {
                        switch( sub->rule.as<Compound>().behavior )
                        {
                            case SubGroup:
                                temp.push_back(sub);
                                break;

                            case MergeAll:
                                temp.merge_back(sub->children());
                                delete sub;
                                break;

                            case MergeOne:
                                if(1==sub->children().size)
                                {
                                    temp.merge_back(sub->children());
                                    delete sub;
                                }
                                else
                                {
                                    temp.push_back(sub);
                                }
                                break;
                        }
                    }
                    else
                    {
                        temp.push_back(sub);
                    }
                }
            }

            Node * Node:: AST(Node *node) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    if(node->rule.as<Terminal>().univocal)
                    {
                        // delete content
                        node->lexeme().clear();
                    }
                    return node;
                }
                else
                {
                    Node::List &self = node->children();
                    {
                        Node::List temp;
                        while(self.size)
                        {
                            Node *sub = Node::AST( self.pop_front() );
                            if(sub->terminal)
                            {
                                AST_Terminal(sub,temp);
                            }
                            else
                            {
                                AST_Internal(sub,temp);
                            }
                        }
                        temp.swap_with(self);
                    }

                    if(node->rule.uuid == Aggregate::UUID )
                    {
                        if( SubGroup == node->rule.as<Compound>().behavior )
                        {
                            return node;
                        }
                        else
                        {
                            // possible merging is only 1 child
                            if(1==node->children().size)
                            {
                                Node  *one = node->children().pop_front();
                                delete node;
                                return one;
                            }
                            else
                            {
                                return node;
                            }
                        }
                    }
                    else
                    {
                        return node;
                    }
                }

            }


        }
    }
}
