
#include "y/jive/language/grammar.hpp"

namespace upsylon
{
    namespace Jive
    {
        namespace Language
        {

            

            Grammar:: ~Grammar() throw()
            {
                std::cerr << "Cleaning Grammar..." << std::endl;
            }

            void Grammar:: declare(Axiom *axiom)
            {
                assert(axiom);
                assert(axiom->is_single());
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
                assert(getRoot());
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

            bool Grammar::  owns(const Axiom &axiom) const throw()
            {
                return axioms.owns( &axiom );
            }
            
            string Grammar:: makeAxiomsID(const Axioms &ax,
                                        const char   *fn,
                                        const char    sep) const
            {
                assert(fn);
                if(ax.size<=0) throw exception("%s.%s(empty list)",**name,fn);
                for(size_t i=0;i<ax.size;++i)
                {
                    const Axiom &axiom = ax[i];
                    if(!owns(axiom)) throw exception("%s.%s(unregistered <%s>)",**name,fn,**(axiom.name));
                }
                
                string id = '(' + *(ax[0].name);
                for(size_t i=1;i<ax.size;++i)
                {
                    id << sep << *(ax[i].name);
                }
                id << ')';
                return id;
                
            }
            
            static inline void sendAxiomsTo(Compound &cm, const Axioms &ax)
            {
                for(size_t i=0;i<ax.size;++i)
                {
                    cm << ax[i];
                }
            }
            
            const Axiom & Grammar:: cat(const Axioms &ax)
            {
                const string id = makeAxiomsID(ax, "cat", ' ');
                Compound    &cm = grp(id);
                sendAxiomsTo(cm,ax);
                return          cm;
            }
            
            const Axiom & Grammar:: pick(const Axioms &ax)
            {
                const string id = makeAxiomsID(ax, "pick", '|');
                Compound    &cm = alt(id);
                sendAxiomsTo(cm,ax);
                return          cm;
            }

            Terminal::Type Grammar:: lexemeType(const Lexeme *lx) const
            {
                assert(lx);
                const Axiom *axiom = query( *(lx->label) );
                if(!axiom)
                {
                    throw exception("%s has no axiom for lexeme <%s>",**name,**(lx->label));
                }
                if(axiom->uuid!=Terminal::UUID)
                {
                    throw exception("%s axiom for <%s> is not a Language::Terminal!",**name,**(lx->label));
                }
                return axiom->as<Terminal>().type;
            }


            bool Grammar:: isStandard(const Lexeme *lx) const
            {
                assert(lx);
                return Terminal::Standard == lexemeType(lx);
            }


        }

    }

}

