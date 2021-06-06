
#include "y/jive/language/axiom/compound/aggregate.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            Aggregate:: ~Aggregate() throw()
            {
            }


            Y_LANG_AXIOM_IMPL(Aggregate)
            {
                Y_LANG_PRINTLN( obs.indent() << "agg<" << name << ">");

                xTree        branch( Node::Acquire(*this) );
                Node::List  &leaves = branch->leaves();
                size_t num=1;

                {
                    Observer::Increase   outer(obs);
                    for(const Reference *ref=head;ref;ref=ref->next,++num)
                    {
                        Y_LANG_PRINTLN( obs.indent() << "|_agg<" << name << ">@" << num << "/" << size);
                        Observer::Increase   inner(obs);
                        Node                *node = NULL;
                        if( (**ref).accept(node,source,lexer,obs) )
                        {
                            if(node)
                            {
                                leaves.push(node);
                            }
                        }
                        else
                        {
                            assert(NULL==node);
                            goto AGGREGATE_FAILURE;
                        }
                    }
                }
                
                {
                    // success
                    const size_t accepted = leaves.size;
                    Node::Grow(tree,branch.yield());
                    Y_LANG_PRINTLN( obs.indent() << "agg<" << name << "> [" << Accepted << " #" << accepted << "/" << size << "]" );
                    return true;
                }

                {
                    // failure
            AGGREGATE_FAILURE:
                Y_LANG_PRINTLN( obs.indent() << "agg<" << name << "> [" << Rejected << " @" << num << "/" << size << "]" );
                Node::ReturnTo(lexer,branch.yield());
                return false;
                }
            }

        }

    }
}


