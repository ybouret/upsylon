//!\file
#ifndef Y_LANG_SYNTAX_NODE_INCLUDED
#define Y_LANG_SYNTAX_NODE_INCLUDED

#include "y/lang/lexical/unit.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Node : public Object
            {
            public:

                typedef core::list_of_cpp<Node> List;

                Node      *next;
                Node      *prev;
                const bool terminal;
                const bool internal;

                inline virtual ~Node() throw() {}


            private:
                Y_DISABLE_ASSIGN(Node);
                char data[sizeof(List)];
            };

        }
    }
}

#endif
