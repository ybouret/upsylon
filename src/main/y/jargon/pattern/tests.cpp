#include "y/jargon/pattern.hpp"
#include "y/ios/imstream.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
 
    namespace Jargon {
        
        bool Pattern:: checkIO() const
        {
            const string      bin = to_binary();
            ios::imstream     fp(bin);
            auto_ptr<Pattern> loaded = Load(fp);
            return loaded->alike(this);
        }
        
        
        void Pattern:: test( Source &source, Token &content ) const
        {
            content.erase();
            
            while( source.isAlive() )
            {
                Token token(source.cache());
                if( match(token,source) && token.size>0 )
                {
                    std::cerr << "<" << token << ">";
                    content.merge_back(token);
                }
                else
                {
                    assert(source.isAlive());
                    assert(0==token.size);
                    assert(source.IO().size);
                    content.push_back(source.get());
                    std::cerr << *content.tail;
                }
            }
            std::cerr << std::endl;
            
        }
        
    }

}

