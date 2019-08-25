
#include "y/oxide/field/info.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldInfo:: ~FieldInfo() throw()
        {
            (size_t &)ownedTypes = 0;
            (size_t &)linearSize = 0;
            releasePrivate();
        }
        
#define Y_OXIDE_FIELD_INFO_CTOR() \
name(id),                         \
ownedTypes(0),                    \
linearSize(nb),                   \
privateData(0),                   \
privateSize(0)
        
        FieldInfo:: FieldInfo(const string &id, const size_t nb) :
        Y_OXIDE_FIELD_INFO_CTOR()
        {
        }
        
        
        void *   FieldInfo:: acquirePrivate()
        {
            static memory::allocator &mgr = memory::global::instance();
            return (privateData = mgr.acquire(privateSize));
        }
        
        void FieldInfo:: releasePrivate() throw()
        {
            assert(ownedTypes==0);
            if(privateSize)
            {
                static memory::allocator &mgr = memory::global::location();
                mgr.release(privateData,privateSize);
            }
        }
        
        string FieldInfo:: subName( const Coord1D n ) const
        {
            return name + vformat("[%ld]", static_cast<long>(n) );
        }
        
    }
}

