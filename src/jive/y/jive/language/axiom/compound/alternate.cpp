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
                //--------------------------------------------------------------
                //
                // initialize
                //
                //--------------------------------------------------------------
                size_t        number   =  1;
                bool          accepted = false;
                const Axiom  *which    = NULL;

                //--------------------------------------------------------------
                //
                // search
                //
                //--------------------------------------------------------------
                {
                    Y_LANG_PRINTLN(obs.indent() << "|_" << name << " " << enumerate('|'));
                    const Observer::Scope scope(obs);
                    for(const Reference *ref=head;ref;ref=ref->next,++number)
                    {
                        Node *node  = NULL;
                        which       = & **ref;
                        if( which->accept(node,source,lexer,obs) )
                        {
                            accepted = true;
                            if(node)
                            {
                                Node::Grow(tree,node);
                                goto ALTERNATE_SUCCESS; // leave scope...
                            }
                            // else keep a chance to accept something not empty
                        }
                        else
                        {
                            // sanity check
                            assert(NULL==node);
                        }
                    }
                }

                //--------------------------------------------------------------
                //
                // at this point, no valid node was created
                //
                //--------------------------------------------------------------
                if(accepted) throw exception("%s found invalid alternate %s!", **(obs.gname), **name);
                Y_LANG_PRINTLN(obs.indent() << "|_" << name << " [" << Rejected << "]" );
                return false;
                
            ALTERNATE_SUCCESS:
                assert(which);
                Y_LANG_PRINTLN(obs.indent() << "|_" << name << " [" << Accepted << " " << which->name << "] <- " << obs.inside() );
                return true;
            }
            
            
        }
        
    }
}


