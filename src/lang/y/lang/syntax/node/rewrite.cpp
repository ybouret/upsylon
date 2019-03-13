
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Node * Node:: Rewrite( Node *node, const Grammar &G)
            {
                assert( node );
                assert( node->is_single() );
                
                if(node->terminal)
                {
                    //----------------------------------------------------------
                    // do nothing
                    //----------------------------------------------------------
                    return node;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    //----------------------------------------------------------
                    auto_ptr<Node> guard(node);

                    Node::List &self = node->children();

                    {
                        Node::List temp;
                        while(self.size>0)
                        {
                            Node *sub = Rewrite(self.pop_front(),G);

                            if(sub->terminal&&sub->rule.as<Terminal>().attr==Operator)
                            {
                                std::cerr << "detected operator <" << sub->rule.name << ">" << std::endl;
                                auto_ptr<Node> keep = sub;
                                // find rhs, mandatory
                                if(self.size<=0)
                                {
                                    throw exception("{%s} missing right argument for operator <%s>", **(G.name), *(sub->rule.name));
                                }
                                auto_ptr<Node> rhs = Rewrite( self.pop_front(), G);

                                // find lhs, optional=>unary operator
                                auto_ptr<Node> lhs = (temp.tail != NULL) ? temp.pop_back() : NULL;

                                // create
                                Node *op = NULL;
                                if(sub->rule.as<Terminal>().univocal)
                                {
                                    // univocal operator
                                    op = Node::Create(sub->rule);
                                }
                                else
                                {
                                    // ordinary (rx) operator
                                    const string s = sub->lexeme().to_string();
                                    op = Node::Create(sub->rule,s);
                                }
                                assert(op);
                                if(lhs.is_valid()) op->children().push_back( lhs.yield() );
                                op->children().push_back( rhs.yield() );
                                temp.push_back(op);
                            }
                            else
                            {
                                temp.push_back(sub);
                            }
                        }

                        temp.swap_with(self);
                    }

                    return guard.yield();
                }

            }
        }
    }
}

