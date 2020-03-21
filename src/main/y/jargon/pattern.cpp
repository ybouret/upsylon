
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
        
        Pattern:: Pattern(const uint32_t u) throw() :
        CountedObject(),
        inode<Pattern>(),
        Serializable(),
        Vizible(),
        uuid(u),
        self(0)
        {
        }
        
        Pattern:: Pattern(const Pattern &p) throw() :
        CountedObject(),
        inode<Pattern>(),
        Serializable(),
        Vizible(),
        uuid(p.uuid),
        self(0)
        {
        }
        
        size_t Pattern:: emitUUID(ios::ostream &fp) const
        {
            size_t nw = 0;
            fp.emit_net(uuid,&nw);
            return nw;
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
        
        void Pattern:: PairwiseMerge(List &pat)
        {
            if(pat.size>1)
            {
                List tmp;
                tmp.push_back( pat.pop_front() );
                while(pat.size)
                {
                    const Pattern *lhs = tmp.tail;
                    const Pattern *rhs = pat.head;
                    Pattern       *mrg = Merge::Try(lhs, rhs);
                    if(mrg)
                    {
                        delete tmp.pop_back();
                        delete pat.pop_front();
                        tmp.push_back( mrg );
                    }
                    else
                    {
                        tmp.push_back( pat.pop_front() );
                    }
                }
                pat.swap_with(tmp);
            }
        }
        
    }
    
}
