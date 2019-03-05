#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/ios/graphviz.hpp"

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
            hasOperators(false),
            verbose(false),
            iAlt(1),
            maxRuleNameLength(0)
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

            bool Grammar:: getVerbose() const throw() { return verbose; }

            void Grammar:: add( Rule *r )
            {
                auto_ptr<Rule> pRule(r);
                if(verbose) { std::cerr << "  \\_{" << name << "} adding new rule '" << r->name << "'" << std::endl; }

                // sanity check
                if(pRule.is_empty())     throw exception("{%s}.add(NULL)",**name);
                if(!rrs)                 throw exception("{%s}.add(LOCKED)",**name);
                if(rrs->search(r->name)) throw exception("{%s}.add(Multiple '%s')", **name, *(r->name) );

                // make a reference
                if(!rrs->declare(r))     throw exception("{%s}.add(unexpected multiple '%s')",**name,*(r->name));

                // append
                pRule->verbose = verbose;
                rules.push_back( pRule.yield() );
                (size_t &)maxRuleNameLength = max_of( maxRuleNameLength, r->name.size() );
            }

            void Grammar:: finalize()
            {
                validate();
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
                throw exception("{%s} has no Rule <%s>", **name, *id);
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
                if(rules.size<=0) throw exception("{%s} no top level rule", **name);
                assert(rules.head);
                return *(rules.head);
            }

            void Grammar:: topLevel( const Rule &r )
            {
                if(!ownsRule(r))
                {
                    throw exception("{%s} topLevel(foreign rule <%s>)", **name, *(r.name) );
                }
                rules.move_to_front( (Rule *)&r );
            }

            void Grammar:: graphViz( const string &dotfile ) const
            {
                {
                    ios::ocstream fp(dotfile);
                    fp << "digraph G {\n";

                    // declare all nodes
                    for(const Rule *r=rules.head;r;r=r->next)
                    {
                        r->graphVizProlog(fp);
                    }

                    // link all nodes
                    for(const Rule *r=rules.head;r;r=r->next)
                    {
                        r->graphVizEpilog(fp);
                    }
                    fp << "}\n";
                }

                ios::GraphViz::Render(dotfile);

            }

            void Grammar:: graphViz( const char *dotfile ) const
            {
                const string _(dotfile); graphViz(_);
            }


        }
    }
}
