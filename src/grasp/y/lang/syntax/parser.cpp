
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

            void Parser:: end() throw()
            {
                finalize();
                dict.release();
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

                // finalize by setting univocal
                const Lexical::Rule &r = (**this).last();
                assert(id==*(r.label));
                assert(!r.motif->weak());

                if(r.motif->univocal())
                {
                    (bool &)(t.ordinary) = false;
                    (bool &)(t.univocal) = true;
                }

                return t;
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

            Terminal & Parser:: term( const string &rx )
            {
                return term(rx,rx);
            }

            Terminal & Parser:: term( const char *rx )
            {
                const string _(rx);
                return term(_,_);
            }



        }
    }
}

#if 0
namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Terminal & Parser:: sole( const string &id, const string &rs)
            {
                const string rx = StringToRegExp(rs);
                return term(id,rx);
            }
            
            Terminal & Parser:: sole( const char *id, const char *rs)
            {
                const string _(id), __(rs); return sole(_,__);
            }

            Terminal & Parser:: sole( const string &rs )
            {
                const string rx = StringToRegExp(rs);
                return term(rs,rx);
            }

            Terminal & Parser:: sole( const char *rs )
            {
                const string _(rs); return sole(_);
            }


        }
    }
}
#endif

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node * Parser:: run(Source &source)
            {
                reset();
                return  Node::AST(accept(source, *this));
            }


        }

    }
}

