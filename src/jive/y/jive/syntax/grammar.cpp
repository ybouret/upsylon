
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Syntax
        {
            Grammar:: ~Grammar() throw()
            {}
            
            void Grammar:: declare(Axiom *axiom)
            {
                assert(axiom!=NULL);
                axioms.push_back(axiom);
                try
                {
                    if(!registry.insert(*(axiom->name),axiom))
                    {
                        throw exception("%s: multiple [%s]",**name, **(axiom->name));
                    }
                }
                catch(...)
                {
                    delete axioms.pop_back();
                    throw;
                }
                if(axiom->name->size()>maxNameLength)
                {
                    aliasing::_(maxNameLength) = axiom->name->size();
                }
            }

            const Axiom * Grammar:: getRoot() const throw()
            {
                return axioms.head;
            }

            void Grammar:: setRoot(const Axiom &axiom)
            {
                if( ! axioms.owns(&axiom) )
                    throw exception("%s.setRoot(not owner of <%s>)",**name,**axiom.name);
                axioms.move_to_front( &aliasing::_(axiom) );
                assert( &axiom == axioms.head );
            }



            const Axiom & Grammar:: zeroOrMore(const Axiom &axiom)
            {
                return repeat(axiom,0);
            }

            const Axiom & Grammar:: oneOrMore(const Axiom &axiom)
            {
                return repeat(axiom,1);
            }



            const Axiom & Grammar:: repeat( const Axiom &axiom, const size_t atLeast)
            {
                string       id = *axiom.name;
                switch(atLeast)
                {
                    case 0: id += '*'; break;
                    case 1: id += '+'; break;
                    default: id += vformat(">=%u",unsigned(atLeast)); break;
                }
                const Axiom &ax = repeat(id,axiom,atLeast);
                return ax;
            }

            const Axiom & Grammar:: option(const Axiom &axiom)
            {
                const string id = *axiom.name + '?';
                const Axiom &ax = option(id,axiom);
                return ax;
            }

            Aggregate & Grammar:: grp()
            {
                const string    id = vformat("agg%c%u",BuiltIn,indxAggregate);
                Aggregate      &ax = agg(id);
                ++indxAggregate;
                return ax;
            }
            
            const Axiom & Grammar:: cat(const Axiom &a, const Axiom &b)
            {
                const string id = '(' + *a.name + '+' + *b.name + ')';
                Aggregate &ans = grp(id);
                ans << a << b;
                return ans;
            }
            
            const Axiom & Grammar:: cat(const Axiom &a,
                                        const Axiom &b,
                                        const Axiom &c)
            {
                const string id  = '(' +*a.name + '+' + *b.name + '+' + *c.name + ')';
                Aggregate   &ans = grp(id);
                ans << a << b << c;
                return ans;
            }

            

            Alternate & Grammar:: alt()
            {
                const string    id = vformat("alt%c%u",BuiltIn,indxAlternate);
                Alternate      &ax = alt(id);
                ++indxAlternate;
                return ax;
            }
            
            const Axiom & Grammar:: choice(const Axiom &a, const Axiom &b)
            {
                const string id = '(' + *a.name + '|' + *b.name + ')';
                Alternate    &ans = alt(id);
                ans << a << b;
                return ans;
            }
            
            const Axiom & Grammar:: choice(const Axiom &a,
                                           const Axiom &b,
                                           const Axiom &c)
            {
                const string id  = '(' +*a.name + '|' + *b.name + '|' + *c.name + ')';
                Alternate   &ans = alt(id);
                ans << a << b << c;
                return ans;
            }

        }
        
    }
    
}


