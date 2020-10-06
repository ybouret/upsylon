


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
                //--------------------------------------------------------------
                // prepare local count
                //--------------------------------------------------------------
                Node::Pointer  rep( Node::Acquire(*this) ); // internal node
                size_t         num = 0;                     // count
                Node::List    &tgt = rep->leaves();         // internal leaves
                {
                    Node      *sub = 0; // termporary sub-tree
                    while( axiom.accept(sub,lexer,source) )
                    {
                        ++num;
                        tgt.push_back(sub); assert(tgt.size==num);
                        Y_JIVE_PRINTLN("\tgot " << num);
                        sub=0;
                    }
                }

                if(num>=atLeast)
                {
                    Y_JIVE_PRINTLN("|_accept " << num << ">=" << atLeast);
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
                    Y_JIVE_PRINTLN("|_reject " << num << "<" << atLeast);
                    Node::ReturnTo(lexer,rep.yield());
                    return false;
                }

            }


            
        }

    }

}

