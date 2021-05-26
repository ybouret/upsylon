
#include "y/jive/syntax/axiom/end-line.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            EndLine:: ~EndLine() throw()
            {
            }

            Y_JIVE_AXIOM_ACCEPT_IMPL(EndLine)
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
                        source.newLine();
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

            void EndLine:: attach(Axiom::Registry &db) const
            {
                (void) new_in(db);
            }


        }

    }

}

