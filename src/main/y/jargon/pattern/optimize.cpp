#include "y/jargon/pattern/basic/all.hpp"
#include "y/jargon/pattern/logic/all.hpp"


namespace upsylon {
    
    namespace Jargon {
        
        
        
        static inline
        Pattern * __optimize_AND( Pattern *p ) throw()
        {
            AND *q = p->as<AND>();
            
            // recursive
            Pattern::Transform(*q, Pattern::Optimize );
            
            // fusion AND
            {
                Pattern::List tmp;
                while(q->size)
                {
                    Pattern *sub = q->pop_front();
                    if(AND::UUID==sub->uuid)
                    {
                        tmp.merge_back( *sub->as<AND>() );
                        delete sub;
                    }
                    else
                    {
                        tmp.push_back(sub);
                    }
                }
                q->swap_with(tmp);
            }
            
            // compact
            return Logical::Compact(q);
        }
        
        static inline
        Pattern * __optimize_OR( Pattern *p ) throw()
        {
            OR *q = p->as<OR>();
            
            // recursive
            Pattern::Transform(*q, Pattern::Optimize );
            
            // no multiple
            Pattern::RemoveRedundant(*q);
            
            // fusion OR
            {
                Pattern::List tmp;
                while(q->size)
                {
                    Pattern *sub = q->pop_front();
                    if(OR::UUID==sub->uuid)
                    {
                        tmp.merge_back( *sub->as<OR>() );
                        delete sub;
                    }
                    else
                    {
                        tmp.push_back(sub);
                    }
                }
                q->swap_with(tmp);
            }
            
            // merge
            Pattern::PairwiseMerge(*q);
            
            // compact
            return Logical::Compact(q);
        }
        
        static inline
        Pattern * __optimize_NONE( Pattern *p ) throw()
        {
            NONE *q = p->as<NONE>();
            
            // recursive
            Pattern::Transform(*q, Pattern::Optimize );
            
            // no multiple
            Pattern::RemoveRedundant(*q);
            
            // merge
            Pattern::PairwiseMerge(*q);

            
            // compact return Logical::Compact(q);
            return q;
        }
        
        
        
        Pattern *Pattern:: Optimize( Pattern *p ) throw()
        {
            assert(p);
            switch(p->uuid)
            {
                case AND::  UUID: return __optimize_AND(p);
                case OR::   UUID: return __optimize_OR(p);
                case NONE:: UUID: return __optimize_NONE(p);
                default:
                    break;
            }
            return p;
        }
        
    }
    
}
