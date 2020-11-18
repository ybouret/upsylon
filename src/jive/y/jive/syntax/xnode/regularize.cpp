#include "y/jive/syntax/axiom.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace  Syntax
        {

            void Node:: regularize(size_t &stamp, size_t &length) throw()
            {
                switch(kind)
                {
                    case IsTerminal: {
                        const Lexeme *l = lexeme();
                        aliasing::_(l->stamp) = ++stamp;
                        length = max_of(length,l->size);
                    } break;

                    case IsInternal:
                        for(Node *node=leaves().head;node;node=node->next)
                        node->regularize(stamp,length);
                        break;
                }
            }

        }

    }
}

