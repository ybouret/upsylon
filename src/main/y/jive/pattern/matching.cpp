
#include "y/jive/pattern/matching.hpp"
#include "y/type/aliasing.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Jive
    {

        Matching:: ~Matching() throw()
        {
            cleanup();
        }

        Matching::Matching(const Matching &m):
        CountedObject(),
        motif( m.motif->clone() ),
        token(),
        firstChars()
        {
            try
            {
                Leading temp(m.firstChars);
                aliasing::_(firstChars).commute(temp);
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }

        void Matching:: setup()
        {
            try
            {
                motif->start( aliasing::_(firstChars) );
            }
            catch(...)
            {
                cleanup();
                throw;
            }
        }

        const Token *Matching:: exactly_(Module *module)
        {
            Source         source(module);
            token.release();
            if(motif->accept(token,source))
            {
                return source.is_active() ? NULL : &token;
            }
            else
            {
                return NULL;
            }
        }

        const Token *Matching:: foundIn_(Module *module)
        {
            Source         source(module);
            token.release();
            if( source.find(firstChars) )
            {
                assert( source.in_cache() > 0);
                if( !motif->accept(token,source) )
                {
                    throw exception("Jive::Matching::Somehow(corrupted firstChars)");
                }
                return &token;
            }
            else
            {
                return NULL;
            }
        }

        size_t Matching:: collect_(sequence<Token::Handle> &tokens, Module *module)
        {
            Source         source(module);
            size_t         n     = 0;
            token.release();
            while( source.find(firstChars) )
            {
                assert(source.in_cache() > 0);
                Token::Handle h = new Token();
                if( !motif->accept(*h,source) )
                {
                    throw exception("Jive::Matching::Somehow(corrupted firstChars)");
                }
                tokens.push_back(h);
                ++n;
                if(h->size<=0)
                {
                    assert(motif->feeble());
                    source.skip();
                }
            }
            return n;
        }


        Matching:: Matching(Pattern *p) :
        CountedObject(),
        motif(p), token(), firstChars()
        {
            setup();
        }

        void Matching:: commute(Matching &other) throw()
        {
            _cswap(motif,other.motif);
            token.swap_with(other.token);
            aliasing::_(firstChars).commute( aliasing::_(other.firstChars) );
        }

        void Matching:: cleanup() throw()
        {
            assert(motif);
            delete aliasing::_(motif);
            aliasing::_(motif) = 0;
        }

        Matching:: Map:: Map() : MapType()
        {
        }

        Matching:: Map:: ~Map() throw()
        {
        }

        Matching:: Map:: Map(const Map &other) : collection(), MapType(other)
        {
        }

        Matching &  Matching:: Map:: operator[](const string &id) const
        {
            const Matching::Pointer *ppM = search(id);
            if(!ppM) throw exception("no Matching::Map['%s']",*id);
            return aliasing::_(**ppM);
        }

        Matching &  Matching:: Map:: operator[](const char *id) const
        {
            const string _(id); return (*this)[_];
        }
    }

}

