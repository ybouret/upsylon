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
                const Axiom  *which    = NULL;
                {
                    const Observer::Scope scope(obs,NULL);
                    for(const Reference *ref=head;ref;ref=ref->next,++number)
                    {
                        Node *node  = NULL;
                        which  = & **ref;
                        if( which->accept(node,source,lexer,obs) )
                        {
                            accepted = true;
                            if(node)
                            {
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
                // at this point, no node...
                if(accepted) throw exception("Language found invalid alternate <%s>!", **name);
                return false;
                
            ALTERNATE_SUCCESS:
                assert(which);
                return true;
            }
            
            
        }
        
    }
}


