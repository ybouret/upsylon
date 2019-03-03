
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

            Terminal & Parser:: term(const string &id, const string &rx, const Attribute role)
            {
                // emit lexer rule
                (**this).emit(id,rx);

                // emit grammar rule
                return terminal(id,role);
            }

            Terminal & Parser:: term( const char   *id, const char   *rx, const Attribute role)
            {
                const string _(id), __(rx); return term(_,__,role);
            }

            Terminal & Parser:: term( const string &id, const char C, const Attribute role)
            {
                const string rx = StringToRegExp(C);
                return term(id,rx,role);
            }

            Terminal & Parser:: word( const string &id, const string &rs,  Attribute role )
            {
                switch(role)
                {
                    case Standard:
                        role=Univocal;
                    default:
                        break;
                }
                const string rx = StringToRegExp(rs);
                return term(id,rx,role);
            }

            Terminal & Parser:: word( const char *id, const char *rs,  Attribute role )
            {
                const string _(id), __(rs); return word(_,__,role);
            }

        }

    }
}

