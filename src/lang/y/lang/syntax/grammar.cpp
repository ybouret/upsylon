
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
            rules(),
            rdb(),
            altID(0)
            {
            }


            string Grammar:: nextAltID()
            {
                const string ans = vformat("alt#%u",++altID);
                return ans;
            }



            const Rule & Grammar:: choice(const Rule &a1, const Rule &a2)
            {
                Strings names(2,as_capacity);
                names << a1.name << a2.name;
                const string id = MakeAltNameFrom(names);
                const Rule  *r  = getRuleByName(id);
                if(r)
                {
                    return *r;
                }
                else
                {
                    Compound  &a = alt(id);
                    return a<<a1<<a2;
                }
            }

            const Rule & Grammar:: choice(const Rule &a1, const Rule &a2, const Rule &a3)
            {
                Strings names(3,as_capacity);
                names << a1.name << a2.name<< a3.name;
                const string id = MakeAltNameFrom(names);
                const Rule  *r  = getRuleByName(id);
                if(r)
                {
                    return *r;
                }
                else
                {
                    Compound  &a = alt(id);
                    return a<<a1<<a2<<a3;
                }
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

            void Grammar:: top(const Rule &rule)
            {
                if(!rules.owns(&rule)) throw exception("{%s}.top(unregistered '%s')", **name, *(rule.name) );
                rules.move_to_front((Rule*)&rule);
            }


        }
    }
}




#include "y/ios/graphviz.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            void Grammar:: GraphViz( const string &filename ) const
            {
                {
                    ios::ocstream fp(filename);
                    fp << "digraph G {\n";
                    for(const Rule *rule = rules.head; rule; rule=rule->next )
                    {
                        rule->prolog(fp);
                    }
                    for(const Rule *rule = rules.head; rule; rule=rule->next )
                    {
                        rule->epilog(fp);
                    }
                    fp << "}\n";
                }
                ios::GraphViz::Render(filename);

            }

        }
    }
}

#include "y/sort/heap.hpp"

namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {
            string Grammar:: MakeAltNameFrom( array<string> &names )
            {
                const size_t n = names.size();
                switch(n)
                {
                    case 0: return string();
                    case 1: return names[1];
                    default:
                        break;

                }

                hsort(names,string::compare);
                string ans;
                ans << '(';
                for(size_t i=1;i<n;++i)
                {
                    ans << names[i] << '|';
                }
                ans << names[n];
                ans << ')';
                return ans;
            }
        }
    }
}

#include "y/lang/syntax/joker.hpp"
namespace upsylon
{
    namespace Lang
    {
        namespace Syntax
        {


            namespace
            {
                typedef map<string,int> visited_map;

                static inline
                void visit( const Rule *rule, visited_map &vmap, const char *g )
                {
                    assert(rule);
                    const string &id     = *(rule->name);
                    int          *pCount = vmap.search(id);
                    if(!pCount)
                    {
                        throw exception("{%s} unregistered rule '%s'", g, *id);
                    }
                    int & count = *pCount;
                    if(count>0)
                    {
                        return; // already visited
                    }
                    else
                    {
                        ++count;
                        std::cerr << "..visit '" << id << "'" << std::endl;
                        switch(rule->uuid)
                        {
                            case Terminal::UUID: // noting else to do!
                                break;

                            case Optional::UUID:
                            case Repeating::UUID:
                                assert(rule->data);
                                visit( static_cast<const Rule *>(rule->data), vmap, g);
                                break;

                            case Aggregate::UUID:
                            case Alternate::UUID:
                                assert(rule->data);
                                for( const Operand *op = static_cast<const Compound *>(rule->data)->head; op; op=op->next)
                                {
                                    visit( &(op->rule), vmap, g);
                                }
                                break;

                            default:
                                throw exception("{%s} found unexpected identifier for rule '%s'", g, *id);
                        }
                    }
                }

            }

            void Grammar:: checkValidity() const
            {
                if(rules.size<=0) throw exception("{%s} no rules",**name);
                const Rule *top = rules.head;
                if(top->hollow()) throw exception("{%s} top level rule '%s' is hollow", **name, *(top->name) );
                visited_map vmap(rules.size,as_capacity);
                for(const Rule *r=top;r;r=r->next)
                {
                    if(!vmap.insert(*(r->name),0))
                    {
                        throw exception("{%s} unexpected failure to visit '%s'", **name, *(r->name));
                    }
                }
                visit(top,vmap,**name);
                for(visited_map::iterator i=vmap.begin();i!=vmap.end();++i)
                {
                    const string &id    = i.key();
                    const int     count = *i;
                    if(!count) throw exception("{%s} standalone rule '%s'", **name, *id);
                }
            }

        }
    }
}




