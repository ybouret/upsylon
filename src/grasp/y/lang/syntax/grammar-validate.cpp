#include "y/lang/syntax/grammar.hpp"
#include "y/lang/syntax/rule-probe.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {

            void Grammar:: validate() const
            {
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << *name << "} validating..." << std::endl);
                if(rules.size<=0)
                {
                    throw exception("{%s} no top level rule", **name );
                }

                // prepare the database
                RuleProbe probe;
                probe.ensure(rules.size);
                probe.verbose = verbose;
                
                // recursively check rules
                probe.visit_from(rules.head,**name);

                // check everyone was visited
                for(const Rule *r=rules.head;r;r=r->next)
                {
                    if(! probe.search(r->name) )
                    {
                        throw exception("{%s} standalone %s <%s>", **name, r->typeName(), *(r->name) );
                    }
                }
                Y_LANG_SYNTAX_VERBOSE(std::cerr << "{" << *name << "} seems valid!" << std::endl);
            }

        }

    }
}

