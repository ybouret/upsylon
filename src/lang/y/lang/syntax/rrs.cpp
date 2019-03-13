
#include "y/lang/syntax/rrs.hpp"

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

            RuleReferenceSet:: RuleReferenceSet() throw() :
            RuleReference::Set()
            {
            }

            RuleReferenceSet:: RuleReferenceSet(size_t n)  :
            RuleReference::Set(n,as_capacity)
            {
            }

            bool    RuleReferenceSet:: declare(const Rule *r)
            {
                assert(r!=NULL);
                const RuleReference R( *r );
                return insert(R);
            }

        }
    }
}
