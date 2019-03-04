
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node * Node:: AST(Node *node) throw()
            {
                assert(node);
                if(node->terminal)
                {
                    assert(node->rule.derived);
                    const Terminal *term = static_cast<const Terminal *>(node->rule.derived);
                    return node;
                }
                else
                {
                    return node;
                }

            }

        }
    }
}
