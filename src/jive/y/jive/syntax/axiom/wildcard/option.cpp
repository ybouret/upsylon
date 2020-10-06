

#include "y/jive/syntax/axiom/wildcard/option.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Option:: ~Option() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(Option)
            {
                Y_JIVE_PRINTLN("Option <" << name << ">");
                Node *subTree = 0;
                if( axiom.accept(subTree,lexer,source) )
                {
                    if(subTree)
                    {
                        Grow(tree,subTree);
                    }
                }
                else
                {
                    assert(NULL==subTree);
                }
                return true;
            }

            

        }

    }

}

