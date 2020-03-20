
#include "y/jargon/pattern/logic.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Logical:: ~Logical() throw()
        {
        }
        
        Logical:: Logical(const uint32_t u) throw() :
        Pattern(u)
        {
        }
        
        Logical::Logical(const Logical &other) :
        Pattern(other),
        Operands(other)
        {
            
        }
        
        size_t Logical:: serialize(ios::ostream &fp) const
        {
            size_t n = emitUUID(fp);
            {
                size_t ns = 0;
                fp.emit_upack(size,&ns);
                n += ns;
            }
            for(const Pattern *op = head; op; op=op->next)
            {
                n += op->serialize(fp);
            }
            return n;
        }
        
        void Logical:: viz(ios::ostream &fp) const
        {
            for(const Pattern *op = head; op; op=op->next)
            {
                op->vizSave(fp);
                endl(vizJoin(fp,op));
            }
        }
        
       
        Logical *Logical:: NoMulti(Logical *p)
        {
            assert(p);
            Pattern::RemoveRedundant(*p);
            return p;
        }
        
        Pattern *Logical:: Compact(Logical *p)
        {
            assert(p);
            if(p->size==1)
            {
                Pattern *one = p->pop_back(); assert(0==p->size);
                delete   p;
                return   one;
            }
            else
            {
                return p;
            }
            
        }
    }
    
}

#include "y/ptr/auto.hpp"
#include "y/jargon/pattern/basic/single.hpp"
#include "y/jargon/pattern/basic/range.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Logical * Logical:: Feed( Logical *p, const string &s )
        {
            auto_ptr<Logical> guard(p);
            for(size_t i=0;i<s.size();++i)
            {
                p->push_back( Single::Create( s[i] ) );
            }
            return guard.yield();
        }
        
        void Logical:: add(const uint8_t c)
        {
            push_back( Single::Create(c) );
        }
        
        void Logical:: add(const uint8_t a, const uint8_t b)
        {
            push_back( Range::Create(a,b) );
        }
        
      
        
    }
    
}

