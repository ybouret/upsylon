//! \file
#ifndef Y_LANG_PARSER_INCLUDED
#define Y_LANG_PARSER_INCLUDED 1

#include "y/lang/syntax/grammar.hpp"
#include "y/lang/lexical/translator.hpp"
#include "y/code/utils.hpp"

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
                typedef intr_ptr<string,Parser> Pointer; //!< for compiler
                typedef Syntax::Rule            Rule;    //!< alias
                typedef const Rule              RULE;    //!< alias
                typedef Syntax::Aggregate       AGG;     //!< alias
                typedef Syntax::Alternate       ALT;     //!< alias
                typedef Syntax::Terminal        TERM;    //!< alias

                explicit Parser(const string &id); //!< initialize
                virtual ~Parser() throw();         //!< destructor

                //! return the label==name as key
                const string & key() const throw();

                //! create a new terminal or recall previously created (MUST match)
                Terminal & __term(const string &id, const string &rx, const Terminal::Attribute attr);
                
                //! a regular terminal
                inline
                const Rule & term( const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Standard);
                }

                //! a regular terminal linking to a plugin rule
                const Rule & term( const string &id );


                //! a regular terminal
                inline
                const Rule & term(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return term(_,__);
                }

                //! an univocal=one sole possible result
                inline
                const Rule & sole( const string &id, const string &rx)
                {
                    return __term(id,rx,Terminal::Univocal);
                }

                //! an univocal=one sole possible result with name=regexp
                inline
                const Rule & sole( const string &rx)
                {
                    return sole(rx,rx);
                }

                //! an univocal=one sole possible terminal
                inline
                const Rule & sole(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return sole(_,__);
                }

                //! an univocal=one sole possible result with name=regexp
                inline
                const Rule & sole( const char *rx)
                {
                    return sole(rx,rx);
                }


                //! just a semantic marker
                inline
                const Rule & mark(const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Semantic);
                }

                //! just a semantic marker
                inline
                const Rule & mark(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return mark(_,__);
                }

                //! make a single char mark
                inline
                const Rule & mark( const char C )
                {
                    const unsigned B     = C;
                    const char     id[4] = { C, 0, 0, 0 };
                    const char     rx[8] = { '\\', 'x', hexadecimal::digit(B>>4), hexadecimal::digit(B), 0 };
                    return mark(id,rx);
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

                //! parse the source
                Node *parse( Source &source, bool keepRaw=true);
                
            protected:
                Scanner &root; //!< root scanner

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            public:
                auto_ptr<Node> raw; //!< last raw tree, if asked for
            };
        }
    }
}

#endif


