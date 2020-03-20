#include "y/jargon/pattern/basic/all.hpp"
#include "y/jargon/pattern/logic/all.hpp"


namespace upsylon {
    
    namespace Jargon {
        
        
        static inline void     optimize(Pattern::List *p) throw()
        {
            Pattern::List tmp;
            while(p->size)
            {
                tmp.push_back( Pattern::Optimize(p->pop_front()));
            }
            p->swap_with(tmp);
        }
        
        static inline Logical *simplify(Logical *p) throw()
        {
            optimize(p);
            return p;
        }
        
        static inline Pattern *reduce(Logical *p) throw()
        {
            (void) simplify(p);
            if(1==p->size)
            {
                Pattern *q = p->pop_front();
                delete   p;
                return   q;
            }
            else
            {
                return p;
            }
        }
        
        
        
        Pattern *Pattern:: Optimize( Pattern *p ) throw()
        {
            assert(p);
            switch(p->uuid)
            {
                    
                default:
                    break;
            }
            return p;
        }
        
    }
    
}
