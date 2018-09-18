
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            Node * Grammar:: run(Lexer &lexer, Source &source)
            {
                //______________________________________________________________
                //
                // Sanity check
                //______________________________________________________________
                if(rules.size<=0) throw exception("{%s} no rules", **name);

                //______________________________________________________________
                //
                // try to probe rule
                //______________________________________________________________
                Rule *root = rules.head;
                Node *tree = 0;
                if(!root->accept(tree,lexer,source))
                {
                    // Syntax Error
                    std::cerr << "Syntax Error Detected" << std::endl;
                    const Lexeme::List &lexemes = lexer.found();
                    for(const Lexeme *lx = lexemes.head; lx; lx=lx->next)
                    {
                        std::cerr << "found " << *lx << std::endl;
                    }

                    throw exception("{%s} syntax error",**name);
                }

                //______________________________________________________________
                //
                // accept a NULL tree
                //______________________________________________________________
                if(!tree)
                {
                    // shouldn't accept check accept empty...
                    throw exception("{%s} is weak, found no syntax tree",**name);
                }

                //______________________________________________________________
                //
                // check status
                //______________________________________________________________
                auto_ptr<Node> guard(tree);
                const Lexeme  *nlx = lexer.peek(source);
                if( nlx )
                {
                    throw exception("{%s} unexpected extraneous <%s>", **name, **(nlx->label));
                }

                return guard.yield();
            }
        }

    }

}
