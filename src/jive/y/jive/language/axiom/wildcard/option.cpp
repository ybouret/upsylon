


#include "y/jive/language/axiom/wildcard/option.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            Option:: ~Option() throw()
            {
            }

            Y_LANG_AXIOM_IMPL(Option)
            {
                Node *node = NULL;
                ++obs.depth;
                if(axiom.accept(node, source, lexer, obs))
                {
                    if(node) Node::Grow(tree,node);
                    --obs.depth;
                }
                else
                {
                    assert(NULL==node);
                    --obs.depth;
                }

                return true;
            }

        }

    }

}

