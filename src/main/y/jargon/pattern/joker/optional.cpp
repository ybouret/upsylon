

#include "y/jargon/pattern/joker/optional.hpp"
#include "y/ios/ostream.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(Optional);
        
        Pattern * Optional:: Create(Pattern *p)
        {
            assert(p);
            const Motif m(p);
            if(p->feeble()) throw exception("Jargon::Optional::Create(%s)",feeble_pattern);
            return new Optional(m);
        }
        
        Optional:: ~Optional() throw()
        {
        }
        
        Optional:: Optional(const Motif &m) throw() :
        Joker(UUID,m)
        {
            Y_PATTERN_SELF(Optional);
        }
        
        Optional:: Optional(const Optional &other) :
        object(),
        Joker(other)
        {
            Y_PATTERN_SELF(Optional);
        }
        
        
        size_t Optional:: serialize(ios::ostream &fp) const
        {
            const size_t n = fp.write_nbo(UUID);
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
            endl(fp << " [label=\"?\",shape=diamond]");
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
            assert(0==token.size);
            if( motif->match(token,source) )
            {
                return true;
            }
            assert(0==token.size);
            return true;
        }
        
        void Optional:: express(ios::ostream &fp) const
        {
            motif->express(fp); fp << '?';
        }
    }
    
}

