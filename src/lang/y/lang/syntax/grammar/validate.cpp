#include "y/lang/syntax/grammar.hpp"
#include "y/lang/syntax/rule-probe.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            void Grammar:: validate() const
            {
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << *name << "} validating..." << std::endl);
                bool &ops = (bool &)hasOperators;
                ops = false;
                if(rules.size<=0)
                {
                    throw exception("{%s} no top level rule", **name );
                }

                //______________________________________________________________
                //
                // prepare the database
                //______________________________________________________________
                RuleProbe probe;
                probe.ensure(rules.size);
                probe.verbose = verbose;

                //______________________________________________________________
                //
                // recursively check rules
                //______________________________________________________________
                probe.visitFrom(rules.head,**name);


                bool hasTerminal = false;
                for(const Rule *r=rules.head;r;r=r->next)
                {
                    //__________________________________________________________
                    //
                    // check visited
                    //__________________________________________________________
                    if(! probe.search(r->name) )
                    {
                        throw exception("{%s} standalone %s <%s>", **name, r->typeName(), *(r->name) );
                    }

                    //__________________________________________________________
                    //
                    // check ready
                    //__________________________________________________________

                    r->checkReady();

                    //__________________________________________________________
                    //
                    // update status for terminal
                    //__________________________________________________________
                    if( r->uuid == Syntax::Terminal::UUID )
                    {
                        hasTerminal = true;
                        if(r->as<Terminal>().attr == Operator)
                        {
                            ops = true;
                            Y_LANG_SYNTAX_VERBOSE(std::cerr << "#{" << *name << "} <" << r->name << "> is an operator" << std::endl);
                        }
                    }


                }

                if(!hasTerminal)
                {
                    throw exception("{%s} no terminal were found", **name );
                }

                Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << *name << "} seems valid!" << std::endl);
            }

        }

    }
}

