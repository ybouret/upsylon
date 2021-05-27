
#include "y/jive/syntax/grammar.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"
#include "y/ios/align.hpp"

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
                const string id = *axiom.name + Repeat::CreateMark(atLeast);
                const Axiom &ax = repeat(id,axiom,atLeast,true);
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
                Aggregate      &ax = grp(id);
                ++indxAggregate;
                return ax;
            }

#define Y_JIVE_CAT_SYMBOL ":"

            const Axiom & Grammar:: cat(const Axiom &a, const Axiom &b)
            {
                const string id = '(' + *a.name + Y_JIVE_CAT_SYMBOL + *b.name + ')';
                Aggregate &ans = grp(id);
                ans << a << b;
                return ans;
            }
            
            const Axiom & Grammar:: cat(const Axiom &a,
                                        const Axiom &b,
                                        const Axiom &c)
            {
                const string id  = '(' +*a.name + Y_JIVE_CAT_SYMBOL + *b.name + Y_JIVE_CAT_SYMBOL + *c.name + ')';
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

            const Axiom::Registry & Grammar:: getRegistry() const
            {
                return registry;
            }
            
            
            std::ostream & Grammar:: displayTerminals(std::ostream &os, const Lexical::Scanner *scanner) const
            {
                os << "<" << **name << ".Terminals>" << std::endl;
                for(const Axiom *axiom=axioms.head;axiom;axiom=axiom->next)
                {
                    switch(axiom->uuid)
                    {
                        case Terminal::UUID: {
                            const string &aname = *(axiom->name);
                            os << "\t" << ios::align(aname,ios::align::left,maxNameLength) << ":";
                            if(scanner)
                            {
                                std::cerr << '\'' << scanner->getPattern(aname).toRegExp() << '\'';
                            }
                            os << std::endl;
                        } break;
                            
                        default:
                            break;
                    }
                }
                os << "<" << **name << ".Terminals/>" << std::endl;
                return os;
            }

            
        }
        
    }
    
}


