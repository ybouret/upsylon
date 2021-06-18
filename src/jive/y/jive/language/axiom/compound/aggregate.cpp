
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

            const Aggregate * Aggregate:: asApparent() const throw()
            {
                switch (type) {
                    case Standard:
                    case Variadic:
                        return this;

                    case Grouping:
                        return NULL;
                }
                return NULL;
            }


            Y_LANG_AXIOM_IMPL(Aggregate)
            {
                XTree        branch( Node::Acquire(*this) );
                Node::List  &leaves = branch->leaves();

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
                    const Observer::Scope scope(obs,asApparent());
                    for(const Reference  *ref=head;ref;ref=ref->next)
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
                    if(accepted<=0) throw exception("%s found invalid aggregate %s!", **(obs.gname), **name);
                    {
                        Node *node = leaves.tail; assert(NULL!=node);
                        switch(node->state)
                        {
                            case Node::IsInternal: break;
                            case Node::IsTerminal: node->lexeme()->usage = Lexeme::Done;  break;
                        }
                    }
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


