
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
            public:
                Y_LANG_ECHO(Axiom);
                Y_LANG_ECHO(Terminal);
                Y_LANG_ECHO(Aggregate);
                Y_LANG_ECHO(Alternate);
                

                enum TerminalUsage
                {
                    SemanticTerminal,
                    DivisionTerminal
                };

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~Parser() throw();

                //______________________________________________________________
                //
                // building Terminals
                //______________________________________________________________

                Terminal::Type Check(const Lexical::Rule &, const TerminalUsage ) const;


                template <typename ID, typename RX> inline
                const Terminal & terminal(const ID &id, const RX &rx)
                {
                    return term(id, Check(emit(id,rx),SemanticTerminal) ) ;
                }

                template <typename ID> inline
                const Terminal & terminal(const ID &id)
                {
                    return terminal(id,id);
                }

                template <typename ID, typename RX> inline
                const Terminal &division(const ID &id, const RX &rx)
                {
                    return term(id, Check(emit(id,rx),DivisionTerminal) );
                }


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

                //! validate
                void validate() const;

                //! parse
                Node *parse(Source &source);


            protected:
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
