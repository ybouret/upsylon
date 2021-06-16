
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

            bool Aggregate:: isGrouping() const throw()
            {
                return Grouping == type;
            }


            bool Aggregate:: isApparent() const throw()
            {
                return Grouping != type;
            }


            Y_LANG_AXIOM_IMPL(Aggregate)
            {
                XTree        branch( Node::Acquire(*this) );
                Node::List  &leaves = branch->leaves();
                size_t       number = 1;

                //--------------------------------------------------------------
                //
                // main loop over different components
                //
                //--------------------------------------------------------------
                {
                    if(isApparent())
                    {
                        Y_LANG_PRINTLN(obs.indent() << "|_" << name << " " << enumerate(' '));
                    }
                    const Observer::Scope scope(obs,isApparent() ? this : NULL);
                    for(const Reference  *ref=head;ref;ref=ref->next,++number)
                    {
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

                //--------------------------------------------------------------
                //
                // success
                //
                //--------------------------------------------------------------
                {
                    const size_t accepted = leaves.size;
                    if(accepted<=0) throw exception("Language found invalid aggregate <%s>!", **name);
                    Node::Grow(tree,branch.yield());
                    if(isApparent())
                    {
                        Y_LANG_PRINTLN(obs.indent() << "|_" << name << " [" << Accepted << " #" << accepted << "/" << size << "]");
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
                    Node::ReturnTo(lexer,branch.yield());
                    if(isApparent())
                    {
                        Y_LANG_PRINTLN(obs.indent() << "|_" << name << " [" << Rejected << "]");
                    }
                    return false;
                }
            }

            
            
        }

    }
}


