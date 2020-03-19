
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
                    content << token;
                }
                else
                {
                    assert(source.alive());
                    assert(0==token.size);
                    assert(source.buffered());
                    content << source.get();
                    std::cerr << *content.tail;
                }
            }
        }

    }
    
}
