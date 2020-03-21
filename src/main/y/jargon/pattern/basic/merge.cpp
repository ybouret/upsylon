
#include "y/jargon/pattern/basic/merge.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static inline
        Pattern * _try_merge( const Single *s, const Range *r )
        {
            const uint8_t ch = s->code;
            const uint8_t lo = r->lower;
            const uint8_t up = r->upper;
            
            if(ch>=lo&&ch<=up)
            {
                return static_cast<const Pattern *>(r)->clone();
            }
            
            if(lo>0)
            {
                const uint8_t lom = lo-1;
                if(lom==ch)
                {
                    return Range::Create(lom,up);
                }
            }
            
            if(up<255)
            {
                const uint8_t upp = up+1;
                if(upp==ch)
                {
                    return Range::Create(lo,upp);
                }
            }
            
            return NULL;
        }
        
        static inline
        Pattern * _try_merge( const Range *a, const Range *b )
        {
            if(a->lower>b->lower)
            {
                cswap(a,b);
            }
            assert(a->lower<=b->lower);
            
            return NULL;
        }
        
        Pattern * Merge::Try(const Pattern *lhs, const Pattern *rhs)
        {
            assert(lhs);
            assert(rhs);
            
            const uint32_t L = lhs->uuid;
            const uint32_t R = rhs->uuid;
            
            if(Single::UUID==L&&Range::UUID==R) return _try_merge( lhs->as<Single>(), rhs->as<Range>() );
            if(Single::UUID==R&&Range::UUID==L) return _try_merge( rhs->as<Single>(), lhs->as<Range>() );
            if(Range::UUID==R&&Range::UUID==L)  return _try_merge( lhs->as<Range>(),  rhs->as<Range>()  );
            
            
            return NULL;
        }
    }
    
}
