
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
                try {
                    if(!registry.insert_by(*(axiom->name),axiom))
                    {
                        throw exception("%s: multiple [%s]",**name, **(axiom->name));
                    }
                }
                catch(...)
                {
                    delete axioms.pop_back();
                    throw;
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


            const Axiom & Grammar:: repeat( const Axiom &axiom, const size_t atLeast)
            {
                const string id = vformat("rep%c%u",BuiltIn,indxRepeat);
                const Axiom &ax = repeat(id,axiom,atLeast);
                ++indxRepeat;
                return ax;
            }

            const Axiom & Grammar:: option(const Axiom &axiom)
            {
                const string id = vformat("opt%c%u",BuiltIn,indxOption);
                const Axiom &ax = option(id,axiom);
                ++indxOption;
                return ax;
            }

            Compound & Grammar:: agg()
            {
                const string    id = vformat("agg%c%u",BuiltIn,indxAggregate);
                Compound       &ax = agg(id);
                ++indxAggregate;
                return ax;
            }



        }
        
    }
    
}


