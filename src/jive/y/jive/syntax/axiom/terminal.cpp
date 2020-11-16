
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
                
                //--------------------------------------------------------------
                //
                // get lexeme
                //
                //--------------------------------------------------------------
                Lexeme::Pointer lexeme( lexer.get(source) );
                if(lexeme.is_valid())
                {
                    //----------------------------------------------------------
                    //
                    // check label
                    //
                    //----------------------------------------------------------
                    if( *(lexeme->label) == *name )
                    {
                        // matching !
                        Y_JIVE_PRINTLN("= '" << lexeme << "'");
                        Lexeme *lx = lexeme.yield();
                        Grow(tree,Node::Acquire(*this,lx));
                        guess.mind(lx);
                        return true;
                    }
                    else
                    {
                        //! different!
                        Y_JIVE_PRINTLN("!= <" << lexeme->label << ">");
                        lexer.unget(lexeme.yield());
                        return false;
                    }
                }
                else
                {
                    Y_JIVE_PRINTLN("=> [EOS]");
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

