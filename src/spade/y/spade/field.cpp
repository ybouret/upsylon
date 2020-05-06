
#include "y/spade/field.hpp"

namespace upsylon {
    
    namespace Spade {
    
        
        namespace Kernel {
     
            Field:: ~Field() throw()
            {
                if(allocated)
                {
                    static memory::allocator &mgr = memory::global::location();
                    mgr.release(workspace,allocated);
                }
                assert(0==allocated);
                assert(0==workspace);
            }
            
            void *  Field:: allocate(const size_t request)
            {
                static memory::allocator &mgr = memory::global::instance();
                return ( workspace = mgr.acquire( (allocated=request) ) );
            }

            
#define Y_SPADE_FIELD_CTOR()\
Object(),\
name(id),\
objectType(ts),\
objectSize(bs)
            
            Field:: Field(const string    &id,
                          const type_spec &ts,
                          const size_t     bs) :
            Y_SPADE_FIELD_CTOR()
            {
                assert(bs>0);
            }
            
            Field:: Field(const char      *id,
                          const type_spec &ts,
                          const size_t     bs) :
            Y_SPADE_FIELD_CTOR()
            {
                assert(bs>0);
            }
            
        }
    
    }
    
}
