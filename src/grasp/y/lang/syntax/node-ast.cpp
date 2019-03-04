
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            const Terminal & Node:: term() const throw()
            {
                assert(terminal);
                assert(rule.derived);
                return *static_cast<const Terminal *>(rule.derived);
            }


            Node * Node:: AST(Node *node) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    
                    if(node->term().isUnivocal())
                    {
                        node->lexeme().clear();
                    }

                    return node;
                }
                else
                {
                    Node::List &self = node->children();
                    Node::List  temp;
                    while(self.size)
                    {
                        Node *sub = AST(self.pop_front());

                        if(sub->terminal)
                        {
                            assert(sub->rule.derived);
                            if(static_cast<const Terminal *>(sub->rule.derived)->isSemantic())
                            {
                                delete sub;
                            }
                            else
                            {
                                temp.push_back(sub);
                            }
                        }
                        else
                        {
                            temp.push_back(sub);
                        }
                    }
                    self.swap_with(temp);
                    return node;
                }

            }

        }
    }
}
