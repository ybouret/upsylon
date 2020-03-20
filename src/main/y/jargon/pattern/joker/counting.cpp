
#include "y/jargon/pattern/joker/counting.hpp"
#include "y/ios/ostream.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(Counting);
        
        static const char fn[] = "Jargon::Counting::Create";
        
        Pattern * Counting:: Create(Pattern *p, const size_t nmin, const size_t nmax)
        {
            assert(p);
            const Motif m(p);
            if(p->feeble()) throw exception("%s(%s)",fn,feeble_pattern);
            if(nmin>nmax)
            {
                _cswap(nmin,nmax);
            }
            
            return new Counting(m,nmin,nmax);
            
        }
        
        Counting:: ~Counting() throw()
        {
        }
        
        Counting:: Counting(const Motif &m, const size_t nmin, const size_t nmax) throw() :
        Joker(UUID,m),
        minimalCount(nmin),
        maximalCount(nmax)
        {
            assert(nmin<=nmax);
            Y_PATTERN_SELF(Counting);
        }
        
        Counting:: Counting(const Counting &other) :
        Joker(other),
        minimalCount(other.minimalCount),
        maximalCount(other.maximalCount)
        {
            Y_PATTERN_SELF(Counting);
        }
        
        
        size_t Counting:: serialize(ios::ostream &fp) const
        {
            const size_t nuid = emitUUID(fp);
            size_t       nmin = 0; fp.emit_upack(minimalCount,&nmin);
            size_t       nmax = 0; fp.emit_upack(maximalCount,&nmax);
            return nuid+nmin+nmax+motif->serialize(fp);
        }
        
        bool Counting:: univocal() const throw()
        {
            return (minimalCount==maximalCount) && motif->univocal();
        }
        
        bool Counting:: feeble() const throw()
        {
            return minimalCount<=0;
        }
        
        void Counting:: vizCore(ios::ostream &fp) const
        {
            fp << " [label=\"";
            {
                const unsigned long nmin = static_cast<unsigned long>(minimalCount);
                const unsigned long nmax = static_cast<unsigned long>(maximalCount);
                const string label = vformat("{%lu,%lu}",nmin,nmax);
                fp << label;
            }
            fp << "\",shape=diamond]";
            
            endl(fp);
            motif->vizSave(fp);
            endl(vizJoin(fp, & *motif));
        }
        
        Pattern * Counting:: clone() const
        {
            return new Counting(*this);
        }
        
        bool Counting::alike(const Pattern *p) const throw()
        {
            if(UUID==p->uuid)
            {
                const Counting *q = p->as<Counting>();
                return (minimalCount == q->minimalCount) && (maximalCount==q->maximalCount) && motif->alike( q->motif.content() );
            }
            else
            {
                return false;
            }
        }
        
        bool Counting:: match(Token &token, Source &source) const
        {
            return false;
        }
        
    }
    
}

