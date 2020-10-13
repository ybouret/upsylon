

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
                Y_JIVE_PRINTLN("");
                Node      *subTree  = 0;
                if( axiom.accept(subTree,lexer,source,guess,depth+1) )
                {
                    if(subTree)
                    {
                        Y_JIVE_PRINTLN("accepted <" << subTree->axiom.name << ">");
                        Grow(tree,subTree);
                        return true;
                    }
                    else
                    {
                        Y_JIVE_PRINTLN("accepted [empty]");
                        return true;
                    }
                }
                else
                {
                    assert(NULL==subTree);
                    Y_JIVE_PRINTLN("rejected");
                    return false;
                }
            }

            

        }

    }

}

