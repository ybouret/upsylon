


#include "y/jive/syntax/axiom/wildcard/repeat.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Repeat:: ~Repeat() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(Repeat)
            {
                Y_JIVE_PRINTLN("Repeat <" << name << "> >= " << atLeast);
                Node::Pointer  rep( Node::Acquire(*this) );
                size_t         num = 0;
                Node::List    &tgt = rep->leaves();
                {
                    Node      *sub = 0;
                    while( axiom.accept(sub,lexer,source) )
                    {
                        ++num;
                        tgt.push_back(sub);
                        sub=0;
                    }
                }

                if(num>=atLeast)
                {
                    if(tree)
                    {
                        // merge leaves
                        assert(tree->kind==tree->IsInternal);
                        tree->leaves().merge_back(tgt);
                    }
                    else
                    {
                        // bad grammar design
                        tree = rep.yield();
                    }
                    return true;
                }
                else
                {
                    Node::ReturnTo(lexer,rep.yield());
                    return false;
                }

            }


            
        }

    }

}

