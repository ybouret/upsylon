#include "y/jive/language/axiom/compound/alternate.hpp"


namespace upsylon
{

    namespace Jive
    {

        namespace Language
        {
            Alternate:: ~Alternate() throw()
            {
            }

            Y_LANG_AXIOM_IMPL(Alternate)
            {
                Y_LANG_PRINTLN( obs.indent() << "alt<" << name << ">");
                size_t        number    = 1;
                bool          accepted  = false;
                const  Axiom *which     = NULL;
                {
                    Observer::Increase   outer(obs);
                    for(const Reference *ref=head;ref;ref=ref->next,++number)
                    {
                        Node                *node  = NULL;
                        const Axiom         &axiom = **ref;
                        Y_LANG_PRINTLN( obs.indent() << "|_alt<" << name << ">@" << number << "/" << size << " : <" << axiom.name << ">");
                        Observer::Increase   inner(obs);
                        if( axiom.accept(node,source,lexer,obs) )
                        {
                            accepted = true;
                            if(node)
                            {
                                which = & axiom;
                                Node::Grow(tree,node);
                                goto ALTERNATE_SUCCESS;
                            }
                            // else keep a chance to accept something...
                        }
                        else
                        {
                            // sanity check
                            assert(NULL==node);
                        }
                    }
                }
                Y_LANG_PRINTLN( obs.indent() << "alt<" << name << "> [" << Status(accepted) << "]" );
                return accepted;

            ALTERNATE_SUCCESS:
                Y_LANG_PRINTLN( obs.indent() << "alt<" << name << "> [" << Accepted << "<" << which->name << "> ]" );
                return true;
            }

            
        }

    }
}


