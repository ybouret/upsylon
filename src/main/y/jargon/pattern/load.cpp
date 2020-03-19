

#include "y/jargon/pattern/basic/all.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static const char fn[] = "Jargon::Pattern::Load";
        
        
        Pattern * Pattern:: Load(ios::istream &fp)
        {
            
            uint32_t id = 0;
            if( !fp.query_net(id) ) throw exception("%s(missing UUID)",fn);
            
            switch(id)
            {
                case Any1  ::UUID: return Any1::Create();
                    
                case Single::UUID: {
                    uint8_t code = 0;
                    if(!fp.query_net(code))
                        throw exception("%s(missing Single.code)",fn);
                    return Single::Create(code);
                }
                    
                case Range::UUID: {
                    uint8_t lower=0, upper=0;
                    if(!fp.query_net(lower)) throw exception("%s(missing Range.lower)",fn);
                    if(!fp.query_net(upper)) throw exception("%s(missing Range.upper)",fn);
                    return Range::Create(lower,upper);
                }
                default:
                    break;
                    
            }
            throw exception("%s(unknown <%s>)",fn,fourcc_(id));
            
        }
        
    }
    
}
