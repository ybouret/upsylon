
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
            
            //! echo type into parser for easier setup
#define Y_LANG_PARSER_ECHO(CLASS) typedef Grammar::CLASS CLASS
            
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
                Y_LANG_PARSER_ECHO(Axiom);     //!< alias
                Y_LANG_PARSER_ECHO(Axioms);    //!< alias
                Y_LANG_PARSER_ECHO(Terminal);  //!< alias
                Y_LANG_PARSER_ECHO(Aggregate); //!< alias
                Y_LANG_PARSER_ECHO(Alternate); //!< alias
                Y_LANG_PARSER_ECHO(Compound); //!< alias

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
                
                //! standard/univocal end of line
                template <typename ID, typename RX> inline
                const Terminal & newline(const ID &id, const RX &rx)
                {
                    return term(id, Check(endl(id,rx,stated),SemanticTerminal) ) ;
                }

                //! standard/univocal end of line
                template <typename ID, typename RX> inline
                const Terminal & endline(const ID &id, const RX &rx)
                {
                    return term(id, Check(endl(id,rx,silent),SemanticTerminal) ) ;
                }

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________

                //! validateWith(this)
                void validate() const;

                //! parse function
                Node *parse(Source &source);

                //! parse a file
                template <typename ID> inline
                Node *parseFile(const ID &fileName)
                {
                    reset();
                    Source source( Module::OpenFile(fileName) );
                    ready(source,10);
                    return parse(source);
                }



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
