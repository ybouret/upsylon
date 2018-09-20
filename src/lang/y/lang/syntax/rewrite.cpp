
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

                    Node::List &src = node->children;
                    Node::List  tgt;

                    while(src.size)
                    {
                        auto_ptr<Node> child  = Rewrite(src.pop_front());
                        const Rule     &rule  = child->rule;
                        if(Terminal::UUID == rule.uuid && static_cast<const Terminal *>(rule.data)->isOperator )
                        {
                            std::cerr << "should make operator from " << child->lexeme << std::endl;
                            auto_ptr<Node> parent = Node::Create(rule); // a funky terminal ID with content!
                            
                        }
                        else
                        {

                        }

                        tgt << child.yield();
                    }

                    src.swap_with(tgt);

                    return node;
                }
            }

        }

    }

}

