#include "y/jargon/pattern/logic/and.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Y_PATTERN_CLID(And);
        
        And:: And() throw() : Logic(UUID)
        {
            Y_PATTERN_SELF(And);
        }
        
        And:: And(const And &other) : Logic(other)
        {
            Y_PATTERN_SELF(And);
        }
        
        And:: ~And() throw()
        {
        }
        
        Logic * And::Create()
        {
            return new And();
        }
        
        bool And:: feeble() const throw()
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                if( op->strong() ) return false;
            }
            return true;
        }
        
        bool And:: univocal() const throw()
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                if( op->multiple() ) return false;
            }
            return true;
        }

        Pattern *And:: clone() const
        {
            return new And(*this);
        }
        
        bool And:: alike(const Pattern *p) const throw()
        {
            assert(p);
            return ( (UUID==p->uuid) && Alike(*this, *(p->as<And>()) ) );
        }
        
        void And:: vizCore(ios::ostream &fp)  const
        {
            endl(fp << " [label=\"&&\",shape=house,style=" << vizStyle() << "]");
            viz(fp);
        }
        
        bool And:: match(Token &token, Source &source)   const
        {
            assert(0==token.size);
            try
            {
                
                for(const Pattern *op=head;op;op=op->next)
                {
                    Token tmp( source.cache() );
                    if( !op->match(tmp,source) )
                    {
                        assert(0==tmp.size);
                        source.unget(token);
                        return false;
                    }
                    token.merge_back(tmp);
                }
                return true;
            }
            catch(...)
            {
                source.unget(token);
                throw;
            }
        }
        
    }
}

