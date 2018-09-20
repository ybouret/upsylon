
#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/compound.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {



            Node *Node:: Rewrite(Node *node)
            {
                assert(node);
                if( node->terminal )
                {
                    return node;
                }
                else
                {

                    Node::List &source = node->children;
                    Node::List  target;

                    try
                    {

                        while(source.size)
                        {
                            auto_ptr<Node> child  = Rewrite(source.pop_front());
                            const Rule     &rule  = child->rule;
                            if(Terminal::UUID == rule.uuid && static_cast<const Terminal *>(rule.data)->isOperator )
                            {
                                std::cerr << "should make operator from " << child->lexeme << std::endl;
                                auto_ptr<Node> parent = Node::Create(rule); // a funky terminal ID with content!

                                // get LHS operator
                                if(target.tail)
                                {
                                    parent->children.push_front(target.pop_back());
                                }

                                // get RHS operator
                                if(source.head)
                                {
                                    parent->children.push_back( Rewrite(source.pop_front()) );
                                }

                                // and replace
                                target << parent.yield();
                            }
                            else
                            {
                                target << child.yield();
                            }

                        }

                        source.swap_with(target);
                    }
                    catch(...)
                    {
                        while(target.size) delete target.pop_back();
                        throw;
                    }

                    return node;
                }
            }

        }

    }

}

