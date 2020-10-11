


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
                Y_JIVE_PRINTLN("repeat <" << axiom.name << "> >= " << atLeast);
                //--------------------------------------------------------------
                // prepare local count
                //--------------------------------------------------------------
                Node::Pointer  rep( Node::Acquire(*this) ); // internal node
                size_t         num = 0;                     // count
                Node::List    &tgt = rep->leaves();         // internal leaves
               
                //--------------------------------------------------------------
                // run local count
                //--------------------------------------------------------------
                ++depth;
                {
                    Node      *sub = 0; // temporary sub-tree
                    while( axiom.accept(sub,lexer,source,mind,depth) )
                    {
                        ++num;
                        tgt.push_back(sub); assert(tgt.size==num);
                        sub=0;
                    }
                }
                --depth;

                //--------------------------------------------------------------
                // check result
                //--------------------------------------------------------------
                if(num>=atLeast)
                {
                    Y_JIVE_PRINTLN("accept <" << axiom.name << ">" << num << ">=" << atLeast);
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
                    Y_JIVE_PRINTLN("reject <" << axiom.name << ">" << num << "<" << atLeast);
                    Node::ReturnTo(lexer,rep.yield());
                    return false;
                }

            }


            
        }

    }

}

