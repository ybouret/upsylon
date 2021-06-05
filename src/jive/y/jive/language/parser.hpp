
//! \file

#ifndef Y_LANG_PARSER_INCLUDED
#define Y_LANG_PARSER_INCLUDED 1

#include "y/jive/language/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {
            //__________________________________________________________________
            //
            //
            //! Parser is a Grammar built with a Lexer
            //
            //__________________________________________________________________
            class Parser : public Grammar, public Lexer
            {
            private:
                enum TerminalUsage
                {
                    SemanticTerminal,
                    DivisionTerminal
                };
                
                Terminal::Type Check(const Lexical::Rule &, const TerminalUsage ) const;
                
            public:
                //______________________________________________________________
                //
                // Types and definitions
                //______________________________________________________________
                Y_LANG_ECHO(Axiom);     //!< alias
                Y_LANG_ECHO(Axioms);    //!< alias
                Y_LANG_ECHO(Terminal);  //!< alias
                Y_LANG_ECHO(Aggregate); //!< alias
                Y_LANG_ECHO(Alternate); //!< alias
                
                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Parser() throw();

                //______________________________________________________________
                //
                // building Terminals
                //______________________________________________________________
                
                //! standard/univocal terminal
                template <typename ID, typename RX> inline
                const Terminal & terminal(const ID &id, const RX &rx)
                {
                    return term(id, Check(emit(id,rx),SemanticTerminal) ) ;
                }

                //! standard/univocal terminal
                template <typename ID> inline
                const Terminal & terminal(const ID &id)
                {
                    return terminal(id,id);
                }

                //! terminal acting as division only
                template <typename ID, typename RX> inline
                const Terminal &division(const ID &id, const RX &rx)
                {
                    return term(id, Check(emit(id,rx),DivisionTerminal) );
                }


                //! terminal acting as division only
                template <typename ID> inline
                const Terminal & division(const ID &id)
                {
                    return division(id,id);
                }

                //! use a plugin as terminal
                template <typename PLUGIN,typename ID>
                const Terminal & plugin(const ID &id)
                {
                    return term(call(plug<PLUGIN>(id)).label,Terminal::Standard);
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! validateWith(this)
                void validate() const;

                //! parse
                Node *parse(Source &source);


            protected:
                //! setup a new parser
                template <typename ID> inline
                explicit Parser(const ID &id) :
                Grammar(id), Lexer(id)
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
              
                
            };
        }
    }
}


#endif
