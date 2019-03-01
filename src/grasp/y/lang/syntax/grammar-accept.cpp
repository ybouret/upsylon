
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node * Grammar:: accept(Source &source, Lexer &lexer)
            {
                //______________________________________________________________
                //
                // initialize
                //______________________________________________________________

                const Rule *top    = rules.head; if(!top) throw exception("{%s} has no rule",**name);
                Node       *tree   = 0;
                const char *id     = **name;
                const char *top_id = *(top->name);

                //______________________________________________________________
                //
                // try acceptance of the full tree
                //______________________________________________________________
                if(!top->accept(source,lexer,tree))
                {
                    assert(tree==0);
                    const Lexeme *lx = lexer.peek(source);
                    if(lx)
                    {
                        const string content = lx->to_print();
                        throw exception("{%s}'%s' unexpected '%s'='%s'", id, top_id, **(lx->label), *content);
                    }
                    else
                    {
                        throw exception("{%s}'%s' doesn't accept empty source", **name, *(top->name));
                    }
                }

                //______________________________________________________________
                //
                // check 
                //______________________________________________________________
                assert(tree!=NULL);
                auto_ptr<Node> guard(tree);

                {
                    auto_ptr<Lexeme> next = lexer.get(source);
                    if(next.is_valid())
                    {
                        const string content = next->to_print();
                        throw exception("{%s} extraneous %s='%s'", **name, **(next->label),*content);
                    }
                }


                return guard.yield();
            }

        }

    }

}

