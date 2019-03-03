//! \file
#ifndef Y_LANG_SYNTAX_PARSER_INCLUDED
#define Y_LANG_SYNTAX_PARSER_INCLUDED 1

#include "y/lang/lexical/translator.hpp"
#include "y/lang/syntax/grammar.hpp"

#include "y/lang/syntax/terminal.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            class Parser : public Lexer, public Grammar, public CountedObject
            {
            public:
                typedef intr_ptr<string,Parser> Pointer;

                //______________________________________________________________
                //
                // Management
                //______________________________________________________________

                virtual ~Parser() throw();          //!< destructor
                explicit Parser(const string &id);  //!< setup with *label = *name
                explicit Parser(const char   *id);  //!< setup with *label = *name
                const string & key() const throw(); //!< *name


                //______________________________________________________________
                //
                // Setting up
                //______________________________________________________________
                typedef Syntax::Rule     RULE; //!< alias for derived types
                typedef Syntax::Terminal TERM; //!< alias for derived types


                TERM & term( const string &id, const string &rx ); //!< create a terminal id matching rx
                TERM & term( const char   *id, const char   *rx ); //!< create a terminal id matching rx, wrapper
                TERM & term( const string &id, const char    C );  //!< create a terminal id matching a single char
                TERM & term( const char   *id, const char    C );  //!< create a terminal id matching a single char, wrapper
                TERM & term( const char C);                        //!< create a terminal matching a single C with the same name

                //______________________________________________________________
                //
                // members
                //______________________________________________________________
                void setOperator(); //!< promote to operator

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }
    }
}

#endif

