
#include "y/spade/field.hpp"
#include "y/memory/allocator/global.hpp"

namespace upsylon {
    
    namespace Spade {
        
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

#if 0
        void *  Field:: allocate(const size_t request)
        {
            assert(0==allocated);
            static memory::allocator &mgr = memory::global::instance();
            return ( workspace = mgr.acquire( (allocated=request) ) );
        }
#endif
        
        void   Field:: allocate( memory::embed emb[], const size_t num )
        {
            static memory::allocator &mgr = memory::global::location();
            assert(0==allocated);
            workspace = memory::embed::create(emb, num,mgr,allocated);
        }
        
#define Y_SPADE_FIELD_CTOR()\
Object(),       \
name(id),       \
objectType(ts), \
objectSize(bs), \
io(NULL),       \
workspace(0),   \
allocated(0)
        
        
        Field:: Field(const string         &id,
                      const std::type_info &ts,
                      const size_t          bs) :
        Y_SPADE_FIELD_CTOR()
        {
            assert(bs>0);
        }
        
        Field:: Field(const char           *id,
                      const std::type_info &ts,
                      const size_t          bs) :
        Y_SPADE_FIELD_CTOR()
        {
            assert(bs>0);
        }
        
        size_t Field:: objectBytes() const throw()
        {
            return objectSize * metrics().items;
        }
        
        size_t Field:: localMemory() const throw()
        {
            return allocated;
        }
        
        double Field:: virtualBits() const throw()
        {
            double ratio = double(localMemory()) / objectBytes();
            ratio = floor(1000*ratio+0.5)/1000;
            return ratio;
        }
        
        
        string Field:: Suffix(const Coord1D c)
        {
            return vformat("[%ld]", static_cast<long>(c) );
        }
        
        size_t Field:: objects() const throw()
        {
            return metrics().items;
        }
        
        
        const void * Field:: objectAt(const size_t indx) const throw()
        {
            return getObjectAt(indx);
        }
        
        void * Field:: objectAt(const size_t indx) throw()
        {
            return (void*) getObjectAt(indx);
        }
        
        const string & Field:: key() const throw()
        {
            return name;
        }
    }
    
    
}
