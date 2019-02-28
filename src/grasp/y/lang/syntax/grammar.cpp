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

            RuleReferenceSet:: RuleReferenceSet() throw() :
            RuleReference::Set()
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

#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() throw()
            {
                no_rrs();
            }


            void Grammar:: no_rrs() throw()
            {
                if(rrs)
                {
                    delete rrs;
                    rrs=0;
                }
            }

            Grammar:: Grammar(const Tag &id) :
            Object(),
            name(id),
            rules(),
            rrs( new RuleReferenceSet() ),
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

            void Grammar:: add( Rule *r )
            {
                auto_ptr<Rule> pRule(r);
                if(verbose) { std::cerr << "[GRAMMAR] adding new rule" << std::endl; }
                // sanity check
                if(pRule.is_empty())     throw exception("[%s](NULL)",**name);
                if(!rrs)                 throw exception("[%s](LOCKED)",**name);
                if(rrs->search(r->name)) throw exception("[%s](Multiple '%s')", **name, *(r->name) );

                // make a reference
                if(!rrs->declare(r))     throw exception("%s(unexpected multiple '%s')",**name,*(r->name));

                // append
                rules.push_back( pRule.yield() );
            }

            void Grammar:: finalize() throw()
            {
                no_rrs();
            }

            Rule & Grammar:: getRuleByName(const string &id)
            {
                if(rrs)
                {
                    RuleReference *prr = rrs->search(id);
                    if(prr)
                    {
                        const Rule *pr = &(prr->rule);
                        return *(Rule *)pr;
                    }
                }
                else
                {
                    for(Rule *r=rules.head;r;r=r->next)
                    {
                        if(r->name==id) return *r;
                    }
                }
                throw exception("[%s] no Rule '%s'", **name, *id);
            }

            bool  Grammar:: ownsRule( const Rule &the_rule ) const throw()
            {
                if(rrs)
                {
                    const RuleReference *prr = rrs->search(the_rule.name);
                    return (NULL!=prr && ( &(prr->rule) == &the_rule) );
                }
                else
                {
                    return rules.owns(&the_rule);
                }
            }

            Rule & Grammar:: topLevel()
            {
                if(rules.size<=0) throw exception("[%s] no rule", **name);
                assert(rules.head);
                return *(rules.head);
            }

            void Grammar:: topLevel( Rule &r )
            {
                if(!ownsRule(r))
                {
                    throw exception("[%s] topLevel(foreign rule '%s')", **name, *(r.name) );
                }
                rules.move_to_front( &r );
            }

        }
    }
}
