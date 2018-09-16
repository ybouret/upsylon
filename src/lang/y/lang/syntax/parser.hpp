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
            class Parser : public Lexical::Translator, public Grammar
            {
            public:
                explicit Parser(const string &id); //!< initialize
                virtual ~Parser() throw();         //!< destructor

                //! create a new terminal or recall previously created (MUST match)
                const Rule & __term(const string &id, const string &rx, const Terminal::Attribute attr);

                //! a regular terminal
                inline
                const Rule & term( const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Standard);
                }

                //! a regular terminal linking to a lexical rule (a.k.a plugin)
                const Rule & term( const string &id );


                //! a regular terminal
                inline
                const Rule & term(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return term(_,__);
                }

                //! an univocal=one sole possible result
                inline
                const Rule & sole( const string &id, const string &rx )
                {
                    return __term(id,rx,Terminal::Univocal);
                }

                //! an univocal=one sole possible terminal
                inline
                const Rule & sole(const char *id, const char *rx)
                {
                    const string _(id),__(rx); return sole(_,__);
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


                //! parse the source
                inline Node *parse( Source &source )
                {
                    return Node::AST( run(*this,source) );
                }

            protected:
                Scanner &root; //!< root scanner

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Parser);
            };
        }
    }
}

#endif


