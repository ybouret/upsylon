#include "y/jargon/pattern/joker/repeating.hpp"
#include "y/ios/ostream.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        const char Repeating::CLID[8]  = Y_FOURCC_CHAR8(Repeating::UUID);
        const char Repeating::CLID0[8] = Y_FOURCC_CHAR8(Repeating::UUID0);
        const char Repeating::CLID1[8] = Y_FOURCC_CHAR8(Repeating::UUID1);
        
        const char * Repeating:: className() const throw()
        {
            switch(minimalCount)
            {
                case 0: return CLID0;
                case 1: return CLID1;
                default:
                    break;
            }
            return CLID;
        }
        
        Pattern * Repeating:: Create(Pattern *p, const size_t nmin)
        {
            assert(p);
            const Motif m(p);
            if(p->feeble()) throw exception("Jargon::Repeating::Create(%s)",feeble_pattern);
            return new Repeating(m,nmin);
        }
        
        Pattern * Repeating:: ZeroOrMore(Pattern *p)
        {
            assert(p);
            return Create(p,0);
        }
        
        Pattern * Repeating:: OneOrMore(Pattern *p)
        {
            assert(p);
            return Create(p,1);
        }
        
        
        Repeating:: ~Repeating() throw()
        {
        }
        
        Repeating:: Repeating(const Motif &m, const size_t n) throw() :
        Joker(UUID,m),
        minimalCount(n)
        {
            Y_PATTERN_SELF(Repeating);
        }
        
        Repeating:: Repeating(const Repeating &other) :
        Joker(other),
        minimalCount(other.minimalCount)
        {
            Y_PATTERN_SELF(Repeating);
        }
        
        
        size_t Repeating:: serialize(ios::ostream &fp) const
        {
            size_t prolog = 0;
            {
                uint32_t u = UUID;
                switch(minimalCount)
                {
                    case 0: u = UUID0; break;
                    case 1: u = UUID1; break;
                    default:
                        break;
                }
                prolog = fp.write_nbo(u);
            }
            
            if(minimalCount>1)
            {
                prolog += fp.write_upack(minimalCount);
            }
            return prolog + motif->serialize(fp);
        }
        
        bool Repeating:: univocal() const throw()
        {
            return false;
        }
        
        bool Repeating:: feeble() const throw()
        {
            return minimalCount<=0;
        }
        
        void Repeating:: vizCore(ios::ostream &fp) const
        {
            const unsigned long nmin = static_cast<unsigned long>(minimalCount);
            
            fp << " [label=\"";
            switch(nmin)
            {
                case 0: fp << "*"; break;
                case 1: fp << "+"; break;
                default: {
                    const string label = vformat(">=%lu",nmin);
                    fp << label;
                } break;
            }
            endl(fp << "\",shape=diamond]");
            motif->vizSave(fp);
            endl(vizJoin(fp, & *motif));
        }
        
        Pattern * Repeating:: clone() const
        {
            return new Repeating(*this);
        }
        
        bool Repeating::alike(const Pattern *p) const throw()
        {
            if(UUID==p->uuid)
            {
                const Repeating *opt = p->as<Repeating>();
                return (minimalCount==opt->minimalCount) &&  (motif->alike( opt->motif.content() ));
            }
            else
            {
                return false;
            }
        }
        
        bool Repeating:: match(Token &token, Source &source) const
        {
            assert(0==token.size);
            size_t count = 0;
            {
                Token  tmp( source.io.cache );
                while( motif->match(tmp,source) )
                {
                    ++count;
                    token.merge_back(tmp);
                }
            }
            if(count>=minimalCount)
            {
                return true;
            }
            else
            {
                source.unget(token);
                return false;
            }
        }
        
    }
    
}

