

#include "y/jargon/pattern/basic/all.hpp"
#include "y/jargon/pattern/logic/all.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"
#include "y/ptr/auto.hpp"

namespace upsylon {
    
    namespace Jargon {
        
        static const char fn[] = "Jargon::Pattern::Load";
        
        
        Pattern * LoadLogical( Logical *p, ios::istream &fp)
        {
            assert(p);
            auto_ptr<Pattern> guard(p);
            size_t count = fp.read_upack<size_t>();
            while(count-- > 0)
            {
                p->push_back( Pattern::Load(fp) );
            }
            return guard.yield();
        }
        
        Pattern * Pattern:: Load(ios::istream &fp)
        {
            
            uint32_t id = 0;
            if( !fp.query_net(id) ) throw exception("%s(missing UUID)",fn);
            
            switch(id)
            {
                    //----------------------------------------------------------
                    // basic
                    //----------------------------------------------------------

                case Any1  ::UUID: return Any1::Create();
                    
                case Single::UUID: {
                    uint8_t code = 0;
                    if(!fp.query_net(code))
                        throw exception("%s(missing Single.code)",fn);
                    return Single::Create(code);
                }
                    
                case Excluded::UUID: {
                    uint8_t code = 0;
                    if(!fp.query_net(code))
                        throw exception("%s(missing Excluded.code)",fn);
                    return Excluded::Create(code);
                }
                    
                case Range::UUID: {
                    uint8_t lower=0, upper=0;
                    if(!fp.query_net(lower)) throw exception("%s(missing Range.lower)",fn);
                    if(!fp.query_net(upper)) throw exception("%s(missing Range.upper)",fn);
                    return Range::Create(lower,upper);
                }
                    
                    //----------------------------------------------------------
                    // logic
                    //----------------------------------------------------------
                case AND::UUID: return LoadLogical(AND::Create(),fp);
                case OR:: UUID: return LoadLogical(OR:: Create(),fp );

                    
                    
                default:
                    break;
                    
            }
            throw exception("%s(unknown <%s>)",fn,fourcc_(id));
            
        }
        
    }
    
}
