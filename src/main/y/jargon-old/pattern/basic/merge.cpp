
#include "y/jargon/pattern/basic/merge.hpp"
#include "y/type/utils.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static inline
        Pattern * _try_merge( const Single *s, const Range *r )
        {
            const int ch = s->code;
            const int lo = r->lower;
            const int up = r->upper;
            
            if(ch>=lo&&ch<=up)
            {
                // single is included in range
                return static_cast<const Pattern *>(r)->clone();
            }
            
            if(lo>0)
            {
                const uint8_t lom = lo-1;
                if(lom==ch)
                {
                    // single is at lower boundary
                    return Range::Create(lom,up);
                }
            }
            
            if(up<255)
            {
                const uint8_t upp = up+1;
                if(upp==ch)
                {
                    // single is at upper boundary
                    return Range::Create(lo,upp);
                }
            }
            
            return NULL;
        }
        
        static inline
        Pattern * _try_merge( const Single *lhs, const Single *rhs )
        {
            int L = lhs->code;
            int R = rhs->code;
            if(L==R)
            {
                return  Single::Create(lhs->code);
            }
            else
            {
                if(L>R) cswap(L,R);
                assert(L<R);
                if(1==R-L)
                {
                    return Range::Create(uint8_t(L),uint8_t(R));
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
            
            const int a_up = a->upper;
            const int b_lo = b->lower;
            if(b_lo<=a_up+1)
            {
                return Range::Create(a->lower, max_of(a->upper,b->upper) );
            }
            
            return NULL;
        }
        
        
        
        Pattern * Merge::Try(const Pattern *lhs, const Pattern *rhs)
        {
            assert(lhs);
            assert(rhs);
            
            const uint32_t L = lhs->uuid;
            const uint32_t R = rhs->uuid;
            
            if(Single:: UUID==L&& Single:: UUID==R) return _try_merge( lhs->as<Single>(), rhs->as<Single>() );
            if(Single:: UUID==L&& Range::  UUID==R) return _try_merge( lhs->as<Single>(), rhs->as<Range>() );
            if(Single:: UUID==R&& Range::  UUID==L) return _try_merge( rhs->as<Single>(), lhs->as<Range>() );
            if(Range::  UUID==R&& Range::  UUID==L) return _try_merge( lhs->as<Range>(),  rhs->as<Range>()  );
            
            
            return NULL;
        }
    }
    
}
