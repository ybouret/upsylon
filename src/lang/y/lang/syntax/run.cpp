
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
                Rule *root  = rules.head;
                Node *tree  = 0;
                int   depth = 0;
                if(!root->accept(tree,lexer,source,depth))
                {
                    // Syntax Error
                    string        err  = *((*source)->origin);
                    const Lexeme *last = lexer.last();

                    if(!last)
                    {
                        throw exception("%s is empty for {%s}", *err, **name);
                    }
                    else
                    {
                        err += vformat(":%d:%d: ",last->line(),last->column());
                        err << "unexpected ";
                        if(!source.active())
                        {
                            err << "final ";
                        }
                        err  << *(last->label);
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


                    throw exception("{%s} unknown syntax error",**name);
                }

                //______________________________________________________________
                //
                // accept a NULL tree
                //______________________________________________________________
                if(!tree)
                {
                    // shouldn't accept check accept empty...
                    throw exception("{%s} is weak, found no syntax tree in '%s'",**name,**((*source)->origin));
                }

                //______________________________________________________________
                //
                // check status
                //______________________________________________________________
                auto_ptr<Node> guard(tree);
                const Lexeme  *curr = lexer.peek(source);
                if( curr )
                {
                    string        err  = *((*source)->origin);
                    const string  currToken = curr->to_print();
                    const Lexeme *last      = lexer.last();
                    if(last&&last!=curr)
                    {
                        err += vformat(":%d:%d: ",last->line(),last->column());
                        const string lastToken = last->to_print();
                        throw exception("%sunexpected '%s' after '%s' for {%s}", *err, *lastToken, *currToken, **name );
                    }
                    else
                    {
                        err += vformat(":%d:%d: ",curr->line(),curr->column());
                        throw exception("%sunexpected '%s' for {%s}", *err, *currToken, **name);
                    }
                }

                return guard.yield();
            }
        }

    }

}
