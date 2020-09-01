
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

        bool Matching:: exactly_(Module *module) const
        {
            Source         source(module);
            Token          token;
            const Pattern &p = **this;
            if(p.accept(token,source))
            {
                return !source.alive();
            }
            else
            {
                return false;
            }
        }

        bool Matching:: somehow_(Token &token, Module *module) const
        {
            Source         source(module);
            const Pattern &p = **this;
            if( source.find(firstChars) )
            {
                assert( source.in_cache() > 0);
                if( !p.accept(token,source) )
                {
                    throw exception("Jive::Matching::Somehow(corrupted firstChars)");
                }
                return true;
            }
            else
            {
                return false;
            }
        }

        size_t Matching:: collect_( sequence<Token::Handle> &tokens, Module *module) const
        {
            Source         source(module);
            const Pattern &p = **this;
            size_t         n = 0;
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

