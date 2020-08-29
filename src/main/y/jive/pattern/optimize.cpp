
#include "y/jive/pattern/all.hpp"


namespace upsylon {

    namespace Jive
    {

        template <typename T>
        Pattern *strip( Pattern *p )
        {
            Pattern *q = p->as<T>()->pop_back();
            delete p;
            return q;
        }
        
        Pattern * Pattern::Optimize(Pattern *p) throw()
        {
            p->optimize();
            
            switch(p->uuid)
            {
                case And::UUID:
                    if(p->as<And>()->size == 1) return strip<And>(p);
                    break;
                    
                case Or:: UUID:
                    if(p->as<Or>()->size  == 1) return strip<Or>(p);
                    break;
                    
                default:
                    break;
            }
            return p;
        }

    }

}

