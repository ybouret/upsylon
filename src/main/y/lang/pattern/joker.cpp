
#include "y/lang/pattern/joker.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Lang
    {
        Joker:: ~Joker() throw() {}

        Joker:: Joker(const uint32_t id, Pattern *jk ) throw() :
        Pattern(id),
        motif(jk)
        {
        }

        void Joker:: vizlink( ios::ostream &fp ) const
        {
            motif->viz(fp);
            link( & *motif, fp );
        }

        void Joker:: optimize() throw()
        {
            auto_ptr<Pattern> &m = (auto_ptr<Pattern> &)motif;
            Pattern *p = Pattern::Optimize( m.yield() );
            m = p;
        }

    }
}

namespace upsylon
{
    namespace Lang
    {

        void Optional:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=diamond,style=%s,label=\"?\"];",vizStyle());
            vizlink(fp);
        }

        void Optional:: write(ios::ostream &fp) const
        {
            fp.emit(UUID);
            motif->write(fp);
        }

        bool Optional :: match(Token &tkn, Source &src ) const
        {
            assert(0==tkn.size);
            (void)(motif->match(tkn,src));
            return true;
        }

        Pattern * Optional:: Create( Pattern *jk )
        {
            Motif guard(jk);
            Pattern *p = new Optional(jk);
            guard.dismiss();
            return p;
        }

    }
}

namespace upsylon
{
    namespace Lang
    {

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

        void Repeating:: __viz( ios::ostream &fp ) const
        {
            fp("[shape=diamond,style=%s,label=\"", vizStyle());
            switch(nmin)
            {
                case 0: fp << '*'; break;
                case 1: fp << '+'; break;
                default:
                    fp(">=%u", unsigned(nmin));
            }
            fp << "\"];\n";
            vizlink(fp);
        }

        void Repeating:: write(ios::ostream &fp) const
        {
            fp.emit(UUID);
            fp.emit_upack(nmin);
            motif->write(fp);
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

    }
}

namespace upsylon
{
    namespace Lang
    {

        Pattern *Counting:: Create( Pattern *jk, const size_t n, const size_t m)
        {
            Motif guard(jk);
            if(guard->weak()) throw exception("Lang::Pattern::Counting WEAK pattern");
            Pattern *p = new Counting(jk,min_of(n,m),max_of(n,m));
            guard.dismiss();
            return p;
        }

        void Counting:: __viz( ios::ostream &fp ) const
        {
            fp(" [shape=diamond,style=%s,label=\"[%u:%u]\"];\n", vizStyle(),unsigned(nmin), unsigned(nmax) );
            vizlink(fp);
        }

        void Counting:: write(ios::ostream &fp) const
        {
            fp.emit(UUID);
            fp.emit_upack(nmin);
            fp.emit_upack(nmax);
            motif->write(fp);
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

    }

}


