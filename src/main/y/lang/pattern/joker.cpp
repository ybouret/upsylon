
#include "y/lang/pattern/joker.hpp"
#include "y/exception.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {

    namespace Lang {

        Joker:: ~Joker() throw() {}

        Joker:: Joker(const uint32_t id, Pattern *jk ) throw() :
        Pattern(id),
        motif(jk)
        {
        }

        void Joker:: vizlink( ios::ostream &fp ) const
        {
            motif->vizSave(fp);
            endl( vizJoin( fp, & *motif ) );
        }

        void Joker:: optimize() throw()
        {
            auto_ptr<Pattern> &m = aliasing::_(motif);
            Pattern           *p = Pattern::Optimize( m.yield() );
            m = p;
        }




    }
}

namespace upsylon {

    namespace Lang {

        Optional:: ~Optional() throw() {}

        Optional:: Optional( Pattern *jk ) throw() : Joker(UUID,jk)
        {
            Y_LANG_PATTERN_IS(Optional);
        }


        Pattern * Optional:: clone() const { return Create( motif->clone() ); }

        bool Optional:: weak() const throw() { return true; }

        void Optional:: vizCore(ios::ostream &fp) const
        {
            endl(fp(" [shape=diamond,style=%s,label=\"?\"]",vizStyle()));
            vizlink(fp);
        }

        size_t Optional:: serialize(ios::ostream &fp) const
        {
            size_t count = 0;
            (void) fp.emit_net<uint32_t>(uuid, &count); assert(4==count);
            return count + motif->serialize(fp);
        }

        bool Optional :: match(Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            (void)(motif->match(tkn,src));
            return true;
        }

        Pattern * Optional:: Create( Pattern *jk )
        {
            Motif  guard(jk);
            Pattern *p = new Optional(jk);
            guard.dismiss();
            return p;
        }

        bool Optional:: univocal() const throw()
        {
            return false;
        }

        Y_LANG_PATTERN_CLID(Optional);

        const char *Optional:: className() const throw()
        {
            return CLID;
        }

        bool   Optional::  equals(const Pattern &p) const throw()
        {
            if(UUID==p.uuid)
            {
                const Optional &rhs = *static_cast<const Optional *>(p.priv);
                return motif->equals(*rhs.motif);
            }
            else
            {
                return false;
            }

        }


    }
}

namespace upsylon {

    namespace Lang {

        Repeating:: ~Repeating() throw() {}

        Pattern * Repeating:: clone() const { return Create( motif->clone(), nmin ); }

        bool Repeating::  weak() const throw()
        {
            assert(!motif->weak());
            return (nmin<=0);
        }

        Pattern * Repeating:: Create( Pattern *jk, const size_t n)
        {
            Motif guard(jk);
            if(guard->weak())
            {
                throw exception("Lang::Pattern::Repeating WEAK pattern");
            }
            Pattern *p = new Repeating(jk,n);
            guard.dismiss();
            return p;
        }

        void Repeating:: vizCore( ios::ostream &fp ) const
        {
            fp("[shape=diamond,style=%s,label=\"", vizStyle());
            switch(nmin)
            {
                case 0: fp << '*'; break;
                case 1: fp << '+'; break;
                default:
                    fp(">=%u", unsigned(nmin));
            }
            fp << "\"]";
            endl(fp);
            vizlink(fp);
        }


        size_t Repeating:: serialize(ios::ostream &fp) const
        {
            size_t count = 0;
            (void) fp.emit_net<uint32_t>(uuid,&count); assert(4==count);
            if(nmin>1)
            {
                size_t nNMIN = 0;
                (void) fp.emit_upack(nmin,&nNMIN);
                assert(4==count);
                count += nNMIN;
            }
            return count + motif->serialize(fp);

        }

        bool Repeating:: match(Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            size_t count = 0;
            while(true)
            {
                Token tmp;
                if(motif->match(tmp,src))
                {
                    ++count;
                    tkn.merge_back(tmp);
                    continue;
                }
                break;
            }
            if(count>=nmin)
            {
                return true;
            }
            else
            {
                src.unget(tkn);
                return false;
            }
        }

        bool Repeating:: univocal() const throw()
        {
            return false;
        }

        Pattern * Repeating:: ZeroOrMore( Pattern *p ) { return Repeating::Create(p,0); }

        Pattern * Repeating:: OneOrMore( Pattern * p ) { return Repeating::Create(p,1); }


        static inline
        uint32_t uuid_for( const size_t n ) throw()
        {
            switch(n)
            {
                case 0:  return Repeating::_ZOM;
                case 1:  return Repeating::_OOM;
                default: break;
            }
            return Repeating::UUID;
        }

        Repeating:: Repeating( Pattern *jk, const size_t n) throw() : Joker(uuid_for(n),jk), nmin(n)
        {
            assert(! motif->weak() );
            Y_LANG_PATTERN_IS(Repeating);
        }

        Y_LANG_PATTERN_CLID(Repeating);

        const char Repeating:: _ID0[8] = Y_FOURCC_CHAR8(_ZOM);


        const char Repeating:: _ID1[8] = Y_FOURCC_CHAR8(_OOM);
        

        const char *Repeating:: className() const throw()
        {
            switch(nmin)
            {
                case 0:  return _ID0;
                case 1:  return _ID1;
                default: break;
            }
            return CLID;
        }

        bool   Repeating::  equals(const Pattern &p) const throw()
        {
            if(UUID==p.uuid)
            {
                const Repeating &rhs = *static_cast<const Repeating *>(p.priv);
                return (nmin==rhs.nmin) && motif->equals(*rhs.motif);
            }
            else
            {
                return false;
            }

        }

    }
}

namespace upsylon {

    namespace Lang {

        Counting:: ~Counting() throw() {}

        Pattern * Counting:: clone() const { return Create( motif->clone(), nmin, nmax); }

        bool Counting:: weak() const throw()
        {
            assert(! motif->weak() );
            return (nmin<=0);
        }

        Counting:: Counting( Pattern *jk, const size_t n, const size_t m) throw() : Joker(UUID,jk), nmin(n), nmax(m)
        {
            Y_LANG_PATTERN_IS(Counting);
            assert(nmin<=nmax);
        }

        Pattern *Counting:: Create( Pattern *jk, const size_t n, const size_t m)
        {
            Motif guard(jk);
            if(guard->weak()) throw exception("Lang::Pattern::Counting WEAK pattern");
            Pattern *p = new Counting(jk,min_of(n,m),max_of(n,m));
            guard.dismiss();
            return p;
        }

        void Counting:: vizCore( ios::ostream &fp ) const
        {
            fp(" [shape=diamond,style=%s,label=\"[%u:%u]\"];\n", vizStyle(),unsigned(nmin), unsigned(nmax) );
            vizlink(fp);
        }

        size_t Counting:: serialize(ios::ostream &fp) const
        {
            size_t count = 0;
            {
                size_t nNMIN = 0;
                size_t nNMAX = 0;
                (void) fp.emit_net<uint32_t>(uuid,&count).emit_upack(nmin,&nNMIN).emit_upack(nmax,&nNMAX);
                assert(4==count);
                count += nNMIN;
                count += nNMAX;
            }
            return count + motif->serialize(fp);

        }

        bool Counting:: match(Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            size_t count = 0;
            while(true)
            {
                Token tmp;
                if(motif->match(tmp,src))
                {
                    ++count;
                    tkn.merge_back(tmp);
                    continue;
                }
                break;
            }
            if(count>=nmin&&count<=nmax)
            {
                return true;
            }
            else
            {
                src.unget(tkn);
                return false;
            }
        }

        bool Counting:: univocal() const throw()
        {
            return (nmin>0) && (nmin==nmax) && motif->univocal();
        }

        const char *Counting:: className() const throw()
        {
            return CLID;
        }

        Y_LANG_PATTERN_CLID(Counting);

        bool   Counting::  equals(const Pattern &p) const throw()
        {
            if(UUID==p.uuid)
            {
                const Counting &rhs = *static_cast<const Counting *>(p.priv);
                return (nmin==rhs.nmin) && (nmax==rhs.nmax) && motif->equals(*rhs.motif);
            }
            else
            {
                return false;
            }

        }

    }

}


