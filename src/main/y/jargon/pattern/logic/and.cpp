#include "y/jargon/pattern/logic/and.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Pattern * Logical:: Equal(const string &s )
        {
            return Compact( Feed(AND::Create(),s) );
        }
        
        Y_PATTERN_CLID(AND);
        
        AND:: AND() throw() : Logical(UUID)
        {
            Y_PATTERN_SELF(AND);
        }
        
        AND:: AND(const AND &other) : Logical(other)
        {
            Y_PATTERN_SELF(AND);
        }
        
        AND:: ~AND() throw()
        {
        }
        
        Logical * AND::Create()
        {
            return new AND();
        }
        
       
        bool AND:: feeble() const throw()
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                if( op->strong() ) return false;
            }
            return true;
        }
        
        bool AND:: univocal() const throw()
        {
            for(const Pattern *op=head;op;op=op->next)
            {
                if( op->multiple() ) return false;
            }
            return true;
        }

        Pattern *AND:: clone() const
        {
            return new AND(*this);
        }
        
        bool AND:: alike(const Pattern *p) const throw()
        {
            assert(p);
            return ( (UUID==p->uuid) && Alike(this,p->as<AND>()) );
        }
        
        void AND:: vizCore(ios::ostream &fp)  const
        {
            endl(fp << " [label=\"&&\",shape=house,style=" << vizStyle() << "]");
            viz(fp);
        }
        
        bool AND:: match(Token &token, Source &source)   const
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

