
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
                if(rules.size<=0) throw exception("{%s} has no rules", **name);

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
                    string        err  = *((*source)->origin);
                    const Lexeme *last = lexer.last();

                    if(!last)
                    {
                        throw exception("%s is empty for {%s}", *err, **name);
                    }
                    else
                    {
                        err += vformat(":%d:%d:",last->line(),last->column());
                        err  << "unexpected " << *(last->label);
                        if( isStandardTerminal(last) )
                        {
                            const string str = last->to_print();
                            err << " '" << str << "'";
                        }
                        if(last->prev)
                        {
                            err << " after " << *(last->prev->label);
                            if(isStandardTerminal(last->prev))
                            {
                                const string str = last->prev->to_print();
                                err << " '" << str << "'";
                            }
                        }
                        err << " for {" << *name << "}";
                        throw exception("%s",*err);
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
