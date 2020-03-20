
#include "y/jargon/pattern.hpp"
#include "y/type/block/zset.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        Pattern:: ~Pattern() throw()
        {
            _bzset(uuid);
            self = 0;
        }
        
        Pattern:: Pattern(const uint32_t u) throw() :
        uuid(u),
        self(0)
        {
        }
        
        Pattern:: Pattern(const Pattern &p) throw() :
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
        
        
        void Pattern:: test( Source &source, Token &content ) const
        {
            content.erase();
            
            while( source.alive() )
            {
                Token token(source.cache());
                if( match(token,source) )
                {
                    std::cerr << "<" << token << ">";
                    content.merge_back(token);
                }
                else
                {
                    assert(source.alive());
                    assert(0==token.size);
                    assert(source.bufferSize());
                    content.push_back(source.get());
                    std::cerr << *content.tail;
                }
            }
            
        }
        
        bool Pattern:: strong() const throw()
        {
            return !feeble();
        }
        
        const char *Pattern:: vizStyle() const throw()
        {
            return feeble() ? "dashed" : "solid";
        }
        
        
        void Pattern::RemoveRedundant( Pattern::List &patterns )
        {
            Pattern::List tmp;
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

    }
    
}
