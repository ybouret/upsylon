
#include "y/jive/language/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            Grammar:: ~Grammar() throw()
            {
            }

            void Grammar:: declare(Axiom *axiom)
            {
                assert(axiom);
                Axiom::List &l = aliasing::_(axioms);
                l.push_back(axiom);
                try
                {
                    if( ! aliasing::_(registry).insert( *(axiom->name), axiom ) )
                    {
                        throw exception("[%s] mulitple <%s>", **name, **(axiom->name) );
                    }
                }
                catch(...)
                {
                    delete l.pop_back();
                    throw;
                }
                const size_t sz = axiom->name->size();
                if(sz>aligned)
                {
                    aliasing::_(aligned) = sz;
                }
            }

            void Grammar:: setRoot(const Axiom &axiom)
            {
                static const char fn[] = "setRoot";

                const string         & key = *(axiom.name);
                const Axiom  * const * ptr = registry.search(*(axiom.name));
                if(!ptr)           throw exception("%s.%s(unregistered <%s>)", **name, fn, *key);
                if(*ptr != &axiom) throw exception("%s.%s(corrupted <%s>)", **name, fn, *key);
                aliasing::_(axioms).move_to_front( & aliasing::_(axiom) );
            }

            const Axiom * Grammar:: getRoot() const throw()
            {
                return axioms.head;
            }


            const Repeat & Grammar:: rep(const Axiom &axiom, const size_t atLeast)
            {
                const string id = *(axiom.name) + Repeat::Mark(atLeast);
                return add( new Repeat(id,axiom,atLeast,true) );
            }

            const Option & Grammar:: opt(const Axiom &axiom)
            {
                const string id = *(axiom.name) + '?';
                return opt(id,axiom);
            }

            const Repeat & Grammar:: zeroOrMore(const Axiom &axiom)
            {
                return rep(axiom,0);
            }

            const Repeat & Grammar:: oneOrMore(const Axiom &axiom)
            {
                return rep(axiom,1);
            }

        }

    }

}

