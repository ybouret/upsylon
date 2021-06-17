
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
                size_t       i = 1;
                const size_t n = size;

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
                    const Observer::Scope scope(obs, (isApparent() ? this : NULL) );
                    for(const Reference  *ref=head;ref;ref=ref->next,++i)
                    {
                        Node                *node = NULL;
                        if( (**ref).accept(node,source,lexer,obs) )
                        {
                            if(node)
                            {
                                leaves.push(node);
                                switch(node->state)
                                {
                                    case Node::IsInternal: break;
                                    case Node::IsTerminal:
                                        if(i>=n)
                                        {
                                            aliasing::_(node->lexeme()->usage) = Lexeme::Done;
                                        }
                                        break;
                                }
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
                    if(accepted<=0) throw exception("%s found invalid aggregate <%s>!", *(obs.gname), **name);
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


