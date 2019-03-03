
#include "y/lang/syntax/parser.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Parser:: ~Parser() throw()
            {
            }

#define Y_LANG_SYNTAX_PARSER_CTOR(ID)  \
Lexer(ID), Grammar(label), CountedObject()
//, root( **this )

            Parser:: Parser(const string &id) : Y_LANG_SYNTAX_PARSER_CTOR(id)
            {
            }

            Parser:: Parser(const char *id) : Y_LANG_SYNTAX_PARSER_CTOR(id)
            {
            }

            const string & Parser:: key() const throw()
            {
                return *name;
            }

        }
    }
}

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Terminal & Parser:: term(const string &id, const string &rx )
            {
                // emit lexer rule
                (**this).emit(id,rx);

                // emit grammar rule
                return terminal(id);
            }

            Terminal & Parser:: term( const char   *id, const char   *rx)
            {
                const string _(id), __(rx); return term(_,__);
            }

            Terminal & Parser:: term( const string &id, const char C)
            {
                const string rx = StringToRegExp(C);
                return term(id,rx);
            }



        }

    }
}

