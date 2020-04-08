
#include "y/jargon/pattern.hpp"
#include "y/type/block/zset.hpp"
#include "y/ios/ostream.hpp"
#include "y/jargon/pattern/basic/merge.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Pattern:: ~Pattern() throw()
        {
            _bzset(uuid);
            self = 0;
        }
        
#define Y_JPATTERN(U) \
CountedObject(),      \
inode<Pattern>(),     \
Serializable(),       \
Vizible(),            \
uuid(U),              \
self(0),              \
entropy(-1)
        
        Pattern:: Pattern(const uint32_t u) throw() :
        Y_JPATTERN(u)
        {
        }
        
        Pattern:: Pattern(const Pattern &p) throw() :
        Y_JPATTERN(p.uuid)
        {
        }
        
        
        
        bool Pattern:: multiple() const throw()
        {
            return !univocal();
        }
        
        
        
        bool Pattern:: strong() const throw()
        {
            return !feeble();
        }
        
        const char *Pattern:: vizStyle() const throw()
        {
            return feeble() ? "dashed" : "solid";
        }
        
        
        void Pattern::RemoveRedundant(List &patterns)
        {
            List tmp;
            while(patterns.size)
            {
                Pattern *p     = patterns.pop_front();
                bool    found = false;
                for(const Pattern *q=tmp.head;q;q=q->next)
                {
                    if( p->alike(q) )
                    {
                        found = true;
                        break;
                    }
                }
                if(found)
                {
                    delete p;
                }
                else
                {
                    tmp.push_back(p);
                }
            }
            patterns.swap_with(tmp);
        }
        
        void Pattern:: Transform(List &patterns, Proc proc ) throw()
        {
            assert(proc);
            List tmp;
            while(patterns.size)
            {
                Pattern *p = patterns.pop_front(); assert(p);
                Pattern *q = proc(p);              assert(q);
                tmp.push_back(q);
            }
            patterns.swap_with(tmp);
        }
        
        void Pattern:: PairwiseMerge(List &rhs)
        {
            {
            CYCLE:
                List lhs;
                bool merged = false;
            SCAN:
                if(rhs.size)
                {
                    const Pattern *R = rhs.head;
                    for(Pattern *L=lhs.head;L;L=L->next)
                    {
                        Pattern *P = Merge::Try(L,R);
                        if(P)
                        {
                            merged = true;
                            delete lhs.replace(L,P);
                            delete rhs.pop_front();
                            goto SCAN;
                        }
                    }
                    lhs.push_back(rhs.pop_front());
                    goto SCAN;
                }
                rhs.swap_with(lhs);
                if(merged) goto CYCLE;
            }
            SortByEntropy(rhs);
        }
        
        
    }
    
}

#include "y/sort/merge.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static inline int compareByEntropy(const Pattern *lhs, const Pattern *rhs, void *) throw()
        {
            return comparison::increasing(lhs->entropy,rhs->entropy);
        }
        
        void Pattern:: SortByEntropy(List &patterns)
        {
            for(Pattern *p=patterns.head;p;p=p->next)
            {
                p->updateEntropy();
            }
            
            merging<Pattern>::sort(patterns, compareByEntropy, NULL);
            
            
        }
        
    }
    
}

#include "y/information/entropy.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        
        void Pattern:: updateEntropy() const throw()
        {
            const Serializable &self = *this;
            aliasing::_(entropy) = Information::Entropy::Of(self);
        }
    }
}
