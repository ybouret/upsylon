

#include "y/jargon/pattern/joker/optional.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(Optional);
        
        Pattern * Optional:: Create(Pattern *p)
        {
            assert(p);
            const Motif m(p);
            
            return new Optional(m);
        }
        
        Optional:: ~Optional() throw()
        {
        }
        
        Optional:: Optional(const Motif &m) throw() :
        Joker(UUID,m)
        {
        }
        
        Optional:: Optional(const Optional &other) :
        Joker(other)
        {
        }
        
        
        size_t Optional:: serialize(ios::ostream &fp) const
        {
            const size_t n = emitUUID(fp);
            return n + motif->serialize(fp);
        }
        
        bool Optional:: univocal() const throw()
        {
            return motif->univocal();
        }
        
        bool Optional:: feeble() const throw()
        {
            return true;
        }
        
        void Optional:: vizCore(ios::ostream &fp) const
        {
            endl(fp << " [label=\"?\",shape=circle]");
            motif->vizSave(fp);
            endl(vizJoin(fp, & *motif));
        }
        
        Pattern * Optional:: clone() const
        {
            return new Optional(*this);
        }
        
        bool Optional::alike(const Pattern *p) const throw()
        {
            if(UUID==p->uuid)
            {
                const Optional *opt = p->as<Optional>();
                return motif->alike( opt->motif.content() );
            }
            else
            {
                return false;
            }
        }
        
        bool Optional:: match(Token &token, Source &source) const
        {
            
            return true;
        }
        
    }
    
}

