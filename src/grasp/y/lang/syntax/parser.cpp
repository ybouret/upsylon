
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

                // link to a terminal
                Terminal &t = terminal(id);

                // finalize by setting univocal/regular
                const Lexical::Rule &r = (**this).last();
                assert(!r.motif->weak());
                assert(id==*(r.label));
                
                if(r.motif->univocal())
                {
                    return t.setUnivocal();
                }
                else
                {
                    return t.setStandard();
                }

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

            Terminal & Parser:: term( const char   *id, const char    C )
            {
                const string _(id); return term(_,C);
            }

            Terminal & Parser:: term( const char C)
            {
                const string _(C); return term(_,C);
            }
        }

    }
}

