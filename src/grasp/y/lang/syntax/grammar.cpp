#include "y/lang/syntax/grammar.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            RuleReference:: ~RuleReference() throw() {}

            RuleReference:: RuleReference(const Rule &r) throw() :
            rule(r)
            {}

            const string & RuleReference:: key() const throw()
            {
                return rule.name;
            }

            RuleReference:: RuleReference( const RuleReference &other ) throw() :
            rule(other.rule)
            {
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

            RuleReferenceSet:: ~RuleReferenceSet() throw() {}

            RuleReferenceSet:: RuleReferenceSet() :
            RuleReference::Set(10,as_capacity)
            {
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
            Grammar:: ~Grammar() throw()
            {
            }

            Grammar:: Grammar(const Tag &id) :
            Object(),
            name(id),
            rules(),
            verbose(false)
            {
            }

            void Grammar:: setVerbose(const bool flag) throw()
            {
                verbose = flag;
                for(Rule *r=rules.head;r;r=r->next)
                {
                    r->verbose = flag;
                }
            }
        }
    }
}
