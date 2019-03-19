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

                // prepare the database
                RuleProbe probe;
                probe.ensure(rules.size);
                probe.verbose = verbose;
                
                // recursively check rules
                probe.visitFrom(rules.head,**name);

                // check everyone was visited
                bool hasTerminal = false;
                for(const Rule *r=rules.head;r;r=r->next)
                {
                    if(! probe.search(r->name) )
                    {
                        throw exception("{%s} standalone %s <%s>", **name, r->typeName(), *(r->name) );
                    }

                    if( r->uuid == Syntax::Terminal::UUID )
                    {
                        hasTerminal = true;
                    }

                    r->checkReady();

                    if(r->uuid==Terminal::UUID&&r->as<Terminal>().attr == Operator )
                    {
                        ops = true;
                        Y_LANG_SYNTAX_VERBOSE(std::cerr << "#{" << *name << "} <" << r->name << "> is an operator" << std::endl);
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

