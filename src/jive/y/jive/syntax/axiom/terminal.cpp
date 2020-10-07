
#include "y/jive/syntax/axiom/terminal.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Terminal:: ~Terminal() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(Terminal)
            {
                Y_JIVE_PRINTLN("Terminal <"<<name<<">");
                
                //--------------------------------------------------------------
                // get lexeme
                //--------------------------------------------------------------
                Lexeme::Pointer lexeme( lexer.get(source) );
                if(lexeme.is_valid())
                {
                    //----------------------------------------------------------
                    // check label
                    //----------------------------------------------------------
                    if( *(lexeme->label) == *name )
                    {
                        // matching !
                        Y_JIVE_PRINTLN("|_accept '"<<*lexeme<<"'");
                        Grow(tree,Node::Acquire(*this,lexeme.yield()));
                        return true;
                    }
                    else
                    {
                        //! different!
                        Y_JIVE_PRINTLN("|_reject " << lexeme->label);
                        lexer.unget(lexeme.yield());
                        return false;
                    }
                }
                else
                {
                    Y_JIVE_PRINTLN("|_no lexeme");
                    return false;
                }

            }

            void Terminal:: attach(Axiom::Registry &db) const
            {
                (void) new_in(db);
            }

            
        }

    }

}

