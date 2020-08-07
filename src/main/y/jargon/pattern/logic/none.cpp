#include "y/jargon/pattern/logic/none.hpp"
#include "y/ios/ostream.hpp"
#include "y/jargon/first-chars.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(NONE);
        
        NONE:: NONE() throw() : Logical(UUID)
        {
            Y_PATTERN_SELF(NONE);
        }
        
        NONE:: NONE(const NONE &other) : object(), Logical(other)
        {
            Y_PATTERN_SELF(NONE);
        }
        
        NONE:: ~NONE() throw()
        {
        }
        
        Logical * NONE::Create()
        {
            return new NONE();
        }
        
        
        bool NONE:: feeble() const throw()
        {
            return false;
        }
        
        bool NONE:: univocal() const throw()
        {
            return false;
        }
        
        Pattern *NONE:: clone() const
        {
            return new NONE(*this);
        }
        
        bool NONE:: alike(const Pattern *p) const throw()
        {
            assert(p);
            return ( (UUID==p->uuid) && Alike(this,p->as<NONE>()) );
        }
        
        void NONE:: vizCore(ios::ostream &fp)  const
        {
            endl(fp << " [label=\"!!\",shape=invhouse,style=" << vizStyle() << "]");
            viz(fp);
        }
        
        bool NONE:: match(Token &token, Source &source)   const
        {
            assert(0==token.size);
            for(const Pattern *op=head;op;op=op->next)
            {
                if(op->match(token,source))
                {
                    source.unget(token);
                    return false;
                }
            }
            assert(0==token.size);
            Char *ch = source.get();
            if(ch)
            {
                token.push_back(ch);
                return true;
            }
            else
            {
                return false;
            }
        }
        
        void NONE:: adjoin(FirstChars &fc) const
        {
            FirstChars allowed(256);
            allowed.loadAll();
            {
                FirstChars content;
                for(const Pattern *op=head;op;op=op->next)
                {
                    op->adjoin(content);
                }
                allowed.strip(content);
            }
            fc.merge(allowed);
            
        }
        
        void NONE:: express(ios::ostream &fp) const
        {
            const bool paren = (size>1);
            if(paren) fp << '(';
            for(const Pattern *op=head;op;)
            {
                op->express(fp);
                op = op->next;
                if(op) fp << '|';
            }
            if(paren) fp << ')';
            fp << '~';
        }
        
    }
}

