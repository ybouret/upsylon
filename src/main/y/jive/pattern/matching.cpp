
#include "y/jive/pattern/matching.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {

        Matching:: ~Matching() throw()
        {
        }

        Matching::Matching(const Matching &m):
        Motif(m), firstChars(m.firstChars)
        {
        }

        void Matching:: setup()
        {
            (**this).start( aliasing::_(firstChars) );
        }

        const Token *Matching:: exactly_(Module *module)
        {
            Source         source(module);
            Token         &token = *this;
            const Pattern &p     = **this;
            token.release();
            if(p.accept(token,source))
            {
                return source.alive() ? NULL : this;
            }
            else
            {
                return NULL;
            }
        }

        const Token *Matching:: foundIn_(Module *module)
        {
            Source         source(module);
            Token         &token = *this;
            const Pattern &p     = **this;
            token.release();
            if( source.find(firstChars) )
            {
                assert( source.in_cache() > 0);
                if( !p.accept(token,source) )
                {
                    throw exception("Jive::Matching::Somehow(corrupted firstChars)");
                }
                return this;
            }
            else
            {
                return NULL;
            }
        }

        size_t Matching:: collect_(sequence<Token::Handle> &tokens, Module *module)
        {
            Source         source(module);
            const Pattern &p     = **this;
            Token         &token = *this;
            size_t         n     = 0;
            token.release();
            while( source.find(firstChars) )
            {
                assert(source.in_cache() > 0);
                Token::Handle h = new Token();
                if( !p.accept(*h,source) )
                {
                    throw exception("Jive::Matching::Somehow(corrupted firstChars)");
                }
                tokens.push_back(h);
                ++n;
                if(h->size<=0)
                {
                    assert(p.feeble());
                    source.skip();
                }
            }
            return n;
        }


    }

}

