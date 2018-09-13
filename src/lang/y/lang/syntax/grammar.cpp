
#include "y/lang/syntax/grammar.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() throw() {}


            Grammar:: Grammar(const Origin &id) :
            name(id),
            rules()
            {
            }


            void Grammar:: add(Rule *rule)
            {
                assert(rule);
                // append tu rules which ownership and name checking
                auto_ptr<Rule> guard(rule);
                for(const Rule *r=rules.head;r;r=r->next)
                {
                    if(r==rule)             throw exception("{%s} recursive rule '%s'",     **name, *(rule->name) );
                    if(r->name==rule->name) throw exception("{%s} multiple rule name '%s'", **name, *(rule->name) );
                }
                rules.push_back( guard.yield() );

                // insert into database
                try
                {
                    const MetaRule m(*rule);
                    if(!rdb.insert(m))  throw exception("{%s} unexpected multiple rule '%s' in database!!!", **name, *(rule->name) );
                }
                catch(...)
                {
                    delete rules.pop_back();
                    throw;
                }
            }


            const Rule * Grammar:: getRuleByName(const string &id) const
            {
                const MetaRule *m = rdb.search(id);
                if(!m) return NULL; else return &(m->rule);
            }


            const Rule * Grammar:: top() const throw()
            {
                return rules.head;
            }

            void Grammar:: top(const Rule *rule)
            {
                if(rule==NULL)
                {
                    if(rules.size) throw exception("{%s} try to set top=NULL",**name);
                }
                else
                {
                    if(!rules.owns(rule)) throw exception("{%s}.top(unregistered '%s')", **name, *(rule->name) );
                    rules.move_to_front((Rule*)rule);
                }
            }

        }
    }
}





