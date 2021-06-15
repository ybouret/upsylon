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
                size_t        number   =  1;
                bool          accepted = false;
                {
                    Observer::Increase   outer(obs);
                    for(const Reference *ref=head;ref;ref=ref->next,++number)
                    {
                        Node                *node  = NULL;
                        const Axiom         &axiom = **ref;
                        Observer::Increase   inner(obs);
                        if( axiom.accept(node,source,lexer,obs) )
                        {
                            accepted = true;
                            if(node)
                            {
                                Node::Grow(tree,node);
                                Y_LANG_PRINTLN( obs.indent() << "alt<" << name << "> [" << Accepted << "<" << axiom.name << "> ]" );
                                return true;
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
                if(accepted)
                    throw exception("Language found invalid alternate <%s>!", **name);
                return false;
            }

            
        }

    }
}


