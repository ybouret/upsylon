
#include "y/jargon/pattern/logic/or.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Pattern * Logical:: Among(const string &s)
        {
            return Compact(NoMulti(Feed(OR::Create(),s)));
        }
        
        Y_PATTERN_CLID(OR);
        
        OR:: OR() throw() : Logical(UUID)
        {
            Y_PATTERN_SELF(OR);
        }
        
        OR:: OR(const OR &other) : Logical(other)
        {
            Y_PATTERN_SELF(OR);
        }
        
        OR:: ~OR() throw()
        {
        }
        
        Logical * OR::Create()
        {
            return new OR();
        }
        
       

        
        bool OR:: feeble() const throw()
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                if( op->feeble() ) return false;
            }
            return false;
        }
        
        bool OR:: univocal() const throw()
        {
            return (0==size) || (1==size && head->univocal());
        }
        
        Pattern *OR:: clone() const
        {
            return new OR(*this);
        }
        
        bool OR:: alike(const Pattern *p) const throw()
        {
            assert(p);
            return ( (UUID==p->uuid) && Alike(this,p->as<OR>()) );
        }
        
        void OR:: vizCore(ios::ostream &fp)  const
        {
            endl(fp << " [label=\"||\",shape=egg,style=" << vizStyle() << "]");
            viz(fp);
        }
        
        bool OR:: match(Token &token, Source &source)   const
        {
            assert(0==token.size);
            for(const Pattern *op=head;op;op=op->next)
            {
                if(op->match(token,source)) return true;
                assert(0==token.size);
            }
            return false;
        }
        
        void OR:: adjoin(FirstChars &fc) const
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                op->adjoin(fc);
            }
        }
    }
}

