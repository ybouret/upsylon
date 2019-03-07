
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
Lexer(ID), Grammar(label), CountedObject(), ready(false)

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
                if(!ready)
                {
                    finalize();
                    dict.release();
                    (bool&)ready = true;
                }
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

#include "y/exception.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node * Parser:: run(Source &source)
            {
                if(!ready) throw exception("{%s} is not ready, call end() before use",**name);

                reset();
                Node *raw = accept(source,*this);
                if(false)
                {
                    auto_ptr<Node> guard = raw;
                    raw->graphViz( *name + "_raw.dot" );
                    guard.dismiss();
                }
                Node *ast = Node::AST(raw);
                if(false)
                {
                    auto_ptr<Node> guard = ast;
                    raw->graphViz( *name + "_ast.dot" );
                    guard.dismiss();
                }
                if(hasOperators)
                {
                    return Node::AST(Node::Rewrite(ast,*this));
                }
                else
                {
                    return ast;
                }
            }

            Node * Parser:: run(Module *module)
            {
                if(!module) throw exception("{%s} run(NULL module)", **name);
                Source source(module);
                return run(source);
            }

        }

    }
}

