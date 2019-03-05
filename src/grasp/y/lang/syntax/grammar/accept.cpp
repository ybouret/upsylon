
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            namespace
            {


                static inline
                void addContentTo(string        &err ,
                                  const Lexeme *lx,
                                  Grammar &G)
                {
                    Syntax::Rule &rule = G.getRuleByName( *(lx->label) );
                    if(rule.uuid==Terminal::UUID && static_cast<const Terminal *>(rule.derived)->ordinary)
                    {

                        const string content = lx->to_print();
                        err += '=';
                        err += '\'' + content + '\'';
                    }
                }
            }


            Node * Grammar:: accept(Source &source, Lexer &lexer)
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________

                const Rule   *top    = rules.head; if(!top) throw exception("{%s} has no rule",**name);
                Node         *tree   = 0;
                const char   *gid    = **name;
                const char   *org    = **((*source)->origin);

                //______________________________________________________________
                //
                //
                // try acceptance of the full tree
                //
                //______________________________________________________________
                if(!top->accept(source,lexer,tree))
                {
                    assert(tree==0);

                    //__________________________________________________________
                    //
                    // try to understand the syntax error
                    //__________________________________________________________
                    const Lexeme *lx = lexer.last();
                    if(lx)
                    {
                        //------------------------------------------------------
                        // some lexemes were extracted and this is the last
                        // lexeme that we got
                        //------------------------------------------------------
                        const int    l       = lx->line();
                        const int    c       = lx->column();
                        string       err     = vformat("%s:%d:%d: {%s} ",org,l,c,gid);
                        if(source.active())
                        {
                            err += "unexpected <" + *(lx->label) + ">";
                            addContentTo(err,lx,*this);
                            const Lexeme *prev = lx->prev;
                            if(prev)
                            {
                                err += " after <" + *(prev->label) << ">";
                                addContentTo(err,prev,*this);
                            }
                        }
                        else
                        {
                            err += "EOF after <" + *(lx->label) + ">";
                            addContentTo(err,lx,*this);
                        }

                        throw exception("%s",*err);
                    }
                    else
                    {
                        //------------------------------------------------------
                        // no lexemes
                        //------------------------------------------------------
                        throw exception("%s: {%s} doesn't accept empty source", org, gid);
                    }

                }

                //______________________________________________________________
                //
                //
                // check no extraneous lexemes
                //
                //______________________________________________________________
                assert(tree!=NULL);
                auto_ptr<Node> guard(tree);

                {
                    const Lexeme *lx = lexer.peek(source);
                    if(lx)
                    {
                        const int    l       = lx->line();
                        const int    c       = lx->column();
                        string       err     = vformat("%s:%d:%d: {%s} ",org,l,c,gid);
                        err += "extraneous <" + *(lx->label) + ">";
                        addContentTo(err,lx,*this);
                        throw exception("%s",*err);
                    }
                }

                //______________________________________________________________
                //
                //
                // return the raw tree
                //
                //______________________________________________________________
                return guard.yield();
            }

        }

    }

}

