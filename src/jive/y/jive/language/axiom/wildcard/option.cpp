


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
                Y_LANG_PRINTLN( obs.indent() << "Option<" << name << ">");
                Node *node = NULL;
                ++obs.depth;
                if(axiom.accept(node, source, lexer, obs))
                {
                    if(node) Node::Grow(tree,node);
                    --obs.depth;
                    Y_LANG_PRINTLN( obs.indent() << "Option<" << name << "> [" << Accepted << "]");
                }
                else
                {
                    assert(NULL==node);
                    --obs.depth;
                    Y_LANG_PRINTLN( obs.indent() << "Option<" << name << "> [" << Rejected << "]");
                }

                return true;
            }

        }

    }

}

