
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

            while( source.is_active() )
            {
                if( source.find(firstChars) && motif->accept(token,source) ) return &token;
                source.skip();
            }
            return NULL;

        }

        size_t Matching:: collect_(sequence<Token::Handle> &tokens, Module *module)
        {
            Source         source(module);
            size_t         n     = 0;

            while( source.is_active() && source.find(firstChars) )
            {
                token.release();
                if( motif->accept(token,source) )
                {
                    if(token.size>0)
                    {
                        Token::Handle h = new Token();
                        h->swap_with(token);
                        tokens.push_back(h);
                        ++n;
                    }
                    else
                    {
                        assert(motif->feeble());
                        source.skip();
                    }
                }
                else
                {
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

        Matching & Matching::Map:: insertMatching(const string &id, Matching::Pointer &p)
        {
            if(!insert(id,p)) throw exception("Matching::Map(multiple '%s')",*id);
            return *p;
        }

        Matching & Matching::Map:: relate(const string &id, Pattern *p)
        {
            assert(p);
            Matching::Pointer ptr = new Matching(p);
            return insertMatching(id,ptr);
        }

        Matching & Matching::Map:: relate(const char *id, Pattern *p)
        {
            const string _(id); return relate(_,p);
        }

    }

}

