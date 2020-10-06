


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
               
                //--------------------------------------------------------------
                // run local count
                //--------------------------------------------------------------
                {
                    Node      *sub = 0; // temporary sub-tree
                    while( axiom.accept(sub,lexer,source) )
                    {
                        ++num;
                        tgt.push_back(sub); assert(tgt.size==num);
                        sub=0;
                    }
                }

                //--------------------------------------------------------------
                // check result
                //--------------------------------------------------------------
                if(num>=atLeast)
                {
                    Y_JIVE_PRINTLN("|_accept " << num << ">=" << atLeast);
                    if(num>0)
                    {
                        if(tree)
                        {
                            // merge leaves
                            assert(tree->kind==tree->IsInternal);
                            tree->leaves().merge_back(tgt);
                        }
                        else
                        {
                            // bad grammar design, but whatever..
                            tree = rep.yield();
                        }
                    }
                    // else leave untouched
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

