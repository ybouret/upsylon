//! \file
#ifndef Y_LANG_SYNTAX_PARSER_INCLUDED
#define Y_LANG_SYNTAX_PARSER_INCLUDED 1

#include "y/lang/lexical/translator.hpp"
#include "y/lang/syntax/grammar.hpp"

#include "y/lang/syntax/terminal.hpp"
#include "y/lang/syntax/joker.hpp"
#include "y/lang/syntax/compound.hpp"


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
                typedef Syntax::Rule      RULE; //!< alias for derived types
                typedef Syntax::Terminal  TERM; //!< alias for derived types
                typedef Syntax::Aggregate AGG;  //!< alias for derived types
                typedef Syntax::Alternate ALT;  //!< alias for derived types

                TERM & term( const string &id, const string &rx ); //!< create a terminal id matching rx
                TERM & term( const char   *id, const char   *rx ); //!< create a terminal id matching rx, wrapper
                TERM & term( const string &id, const char    C );  //!< create a terminal id matching a single char
                TERM & term( const char   *id, const char    C );  //!< create a terminal id matching a single char, wrapper
                TERM & term( const char C );                       //!< create a terminal matching a single C with the same name
                TERM & term( const string &rx );                   //!< create a terminal rx matching rx
                TERM & term( const char   *rx );                   //!< create a terminal rx matching rx, wrapper

                template <typename T>
                TERM & mark( const T &arg ) { return term(arg).setSemantic(); }

                //! zero arguments plugin
                template <typename PLUGIN> inline RULE & hook( const string &id )
                {
                    Lexical::Translator::hook<PLUGIN>(**this,id);
                    return terminal(id).setStandard();
                }

                //! zero arguments plugin
                template <typename PLUGIN> inline RULE & hook( const char *id )
                {
                    const string _(id); return hook<PLUGIN>(_);
                }


                //______________________________________________________________
                //
                // members
                //______________________________________________________________


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };

        }
    }
}

#endif

