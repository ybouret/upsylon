
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

                XTree        branch( Node::Acquire(*this) );
                Node::List  &leaves = branch->leaves();
                size_t       number = 1;

                //--------------------------------------------------------------
                //
                // main loop over different components
                //
                //--------------------------------------------------------------
                {
                    Observer::Increase   outer(obs);
                    bool                 found = false;
                    for(const Reference *ref=head;ref;ref=ref->next,++number)
                    {
                        Y_LANG_PRINTLN( obs.indent() << "|_agg<" << name << ">@" << number << "/" << size);
                        Observer::Increase   inner(obs);
                        Node                *node = NULL;
                        if( (**ref).accept(node,source,lexer,obs) )
                        {
                            if(node)
                            {
                                leaves.push(node);
                            }
                            if(!found)
                            {
                                if(Grouping!=type)
                                {
                                    obs.into = this; // partially accepted
                                    Y_LANG_PRINTLN( obs.indent() << "[<== into <" << name << ">]");
                                }
                                found    = true;
                            }
                        }
                        else
                        {
                            assert(NULL==node);
                            goto AGGREGATE_FAILURE;
                        }
                    }
                }

                //--------------------------------------------------------------
                //
                // success
                //
                //--------------------------------------------------------------
                {
                    const size_t accepted = leaves.size;
                    Node::Grow(tree,branch.yield());
                    Y_LANG_PRINTLN( obs.indent() << "agg<" << name << "> [" << Accepted << " #" << accepted << "/" << size << "]" );
                    if(Grouping!=type)
                    {
                        obs.host = this;
                        obs.unit = obs.curr;
                    }
                    return true;
                }

                //--------------------------------------------------------------
                //
                // failure
                //
                //--------------------------------------------------------------
                {
                AGGREGATE_FAILURE:
                    Y_LANG_PRINTLN( obs.indent() << "agg<" << name << "> [" << Rejected << " @" << number << "/" << size << "]" );
                    Node::ReturnTo(lexer,branch.yield());
                    return false;
                }
            }

        }

    }
}


