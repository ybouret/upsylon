//! \file
#ifndef Y_LANG_PARSER_INCLUDED
#define Y_LANG_PARSER_INCLUDED 1

#include "y/lang/syntax/grammar.hpp"
#include "y/lang/lexical/translator.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            //! a parser linking a Lexer and a Grammar
            class Parser : public Lexical::Translator, public Grammar, public counted
            {
            public:
                typedef intr_ptr<string,Parser> Pointer;  //!< for compiler
                typedef Syntax::Rule            Rule;     //!< alias
                typedef const Rule              RULE;     //!< alias
                typedef Syntax::Aggregate       AGG;      //!< alias
                typedef Syntax::Alternate       ALT;      //!< alias
                typedef Syntax::Terminal        TERM;     //!< alias
                typedef Syntax::Compound        COMPOUND; //!< alias

                explicit Parser(const string &id); //!< initialize
                virtual ~Parser() throw();         //!< destructor

                //! return the label==name as key
                const string & key() const throw();

                //! create a new terminal or recall previously created (MUST match)
                Terminal & __term(const string &id, const string &rx, const Terminal::Attribute attr);

                //______________________________________________________________
                //
                // Regular/Plugin Terminal
                //______________________________________________________________

                //! a regular terminal
                inline
                RULE & term( const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Standard);
                }

                //! a regular terminal linking to a plugin rule
                RULE & term( const string &id );


                //! a regular terminal
                inline
                RULE & term(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return term(_,__);
                }

                //! a regular terminal linking to a plugin rule
                inline
                RULE & term( const char *id ) { const string _(id); return term(_); }


                //______________________________________________________________
                //
                // Univocal Terminal
                //______________________________________________________________

                //! an univocal=one sole possible interpretation
                inline
                RULE & sole( const string &id, const string &rx)
                {
                    return __term(id,rx,Terminal::Univocal);
                }

                //! an univocal=one sole possible interpretation with name=rx
                inline
                RULE & sole( const string &rx)
                {
                    return sole(rx,rx);
                }

                //! an univocal=one sole possible terminal
                inline
                RULE & sole(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return sole(_,__);
                }

                //! an univocal=one sole possible result with name=rx
                inline
                RULE & sole(const char *rx)
                {
                    return sole(rx,rx);
                }

                //! an univocal char
                inline
                RULE & sole(const char C)
                {
                    make_id_and_rx_from(C);
                    return sole(__id,__rx);
                }


                //______________________________________________________________
                //
                // Operator to rewrite AST
                //______________________________________________________________

                //! operator name/regexp
                /**
                 if already declared, then the isOperator is updated!!!
                 */
                inline
                RULE & op( const string &id, const string &rx)
                {
                    return __term(id,rx,Terminal::Univocal).setOperator();
                }

                //! operator name/regexp
                inline
                RULE & op( const char *id, const char *rx)
                {
                    const string _(id);
                    const string __(rx);
                    return op(_,__);
                }

                //! operator name=regexp
                inline
                RULE & op( const string &rx)
                {
                    return op(rx,rx);
                }

                //! operator name=regexp
                inline
                RULE & op(const char *rx)
                {
                    return op(rx,rx);
                }

                //! one char
                inline
                RULE & op(const char C)
                {
                    make_id_and_rx_from(C);
                    return op(__id,__rx);
                }


                //______________________________________________________________
                //
                // Semantic Terminal
                //______________________________________________________________

                //! just a semantic marker
                inline
                RULE & mark(const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Semantic);
                }

                //! just a semantic marker
                inline
                RULE & mark(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return mark(_,__);
                }

                //! make a single char mark
                inline
                RULE & mark( const char C )
                {
                    make_id_and_rx_from(C);
                    return mark(__id,__rx);
                }

                //! lexical plugin, no lexeme production, 0 argument
                template <typename PLUGIN> inline
                void hook( const string &pluginName )
                {
                    Lexer &L = *this;
                    L.hook<PLUGIN>(*L,pluginName);
                }

                //! lexical plugin, no lexeme production, 0 argument
                template <typename PLUGIN> inline
                void hook( const char *pluginName )
                {
                    const string _(pluginName); hook<PLUGIN>(_);
                }

                //! lexical plugin, no lexeme production, 0 argument
                template <typename PLUGIN> inline
                void hook( const string &pluginName, const char *rx )
                {
                    Lexer &L = *this;
                    L.hook<PLUGIN>(*L,pluginName,rx);
                }

                //! lexical plugin, no lexeme production
                template <typename PLUGIN> inline
                void hook( const char *pluginName, const char *rx )
                {
                    const string _(pluginName), __(rx); hook<PLUGIN>(_,__);
                }

                //! parse the source to AST, without rewriting
                Node *parse( Source &source, bool keepRaw);
                
            protected:
                Scanner &root; //!< root scanner

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
                char __id[4];
                char __rx[8];
                void make_id_and_rx_from(const char C) throw();

            public:
                auto_ptr<Node> raw; //!< last raw tree, if asked for
            };
        }
    }
}

#endif


