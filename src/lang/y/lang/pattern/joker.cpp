
#include "y/lang/pattern/joker.hpp"

namespace upsylon
{
    namespace Lang
    {
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


        void Optional:: __viz(ios::ostream &fp) const
        {
            fp(" [shape=diamond,label=\"?\"];");
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


        void Repeating:: __viz( ios::ostream &fp ) const
        {
            fp << " [shape=diamond,label=\"";
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
            fp.emit<uint32_t>(nmin);
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

        void Counting:: __viz( ios::ostream &fp ) const
        {
            fp(" [shape=diamond,label=\"[%u:%u]\"];\n", unsigned(nmin), unsigned(nmax) );
            vizlink(fp);
        }

        void Counting:: write(ios::ostream &fp) const
        {
            fp.emit(UUID);
            fp.emit<uint32_t>(nmin);
            fp.emit<uint32_t>(nmax);
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


