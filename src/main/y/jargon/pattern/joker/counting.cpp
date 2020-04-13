
#include "y/jargon/pattern/joker/counting.hpp"
#include "y/jargon/pattern/joker/optional.hpp"
#include "y/jargon/pattern/logic/none.hpp"

#include "y/ios/ostream.hpp"
#include "y/exception.hpp"

#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(Counting);
        
        static const char fn[] = "Jargon::Counting::Create";
        
        Pattern * Counting:: Create(Pattern *p, const size_t n)
        {
            return Create(p,n,n);
        }
        
        Pattern * Counting:: Create(Pattern *p, const size_t nmin, const size_t nmax)
        {
            //------------------------------------------------------------------
            //
            // securise pointer
            //
            //------------------------------------------------------------------
            assert(p);
            auto_ptr<Pattern> guard(p);
            
            //------------------------------------------------------------------
            //
            // check if strong
            //
            //------------------------------------------------------------------
            if(p->feeble()) throw exception("%s(%s)",fn,feeble_pattern);
           
            //------------------------------------------------------------------
            //
            // adjust parameters
            //
            //------------------------------------------------------------------
            if(nmin>nmax)
            {
                _cswap(nmin,nmax);
            }
            
            //------------------------------------------------------------------
            //
            // special case: nmax=0 => none
            //
            //------------------------------------------------------------------
            if(nmax==0)
            {
                assert(0==nmin);
                auto_ptr<Logical> none = NONE::Create();
                none->push_back( guard.yield() );
                return none.yield();
            }
            else
            {
                //--------------------------------------------------------------
                //
                // change original pointer owneship
                //
                //--------------------------------------------------------------
                const Motif m = guard.yield();
                
                if(nmin==0&&nmax==1)
                {
                    //----------------------------------------------------------
                    //
                    // special case: 0 or 1, it's an Optional
                    //
                    //----------------------------------------------------------
                    return new Optional(m);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // default case
                    //
                    //----------------------------------------------------------
                    return new Counting(m,nmin,nmax);
                }
            }
            
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
            const size_t nuid = fp.write_nbo(UUID);
            const size_t nmin = fp.write_upack(minimalCount);
            const size_t nmax = fp.write_upack(maximalCount);
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
            assert(0==token.size);
            size_t count = 0;
            {
                Token  tmp;
                while( motif->match(tmp,source) )
                {
                    ++count;
                    token.merge_back(tmp);
                }
            }
            if(minimalCount<=count && count<=maximalCount)
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

