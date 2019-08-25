
#include "y/oxide/field/info.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldInfo:: ~FieldInfo() throw()
        {
            releasePrivate();
        }

        FieldInfo:: FieldInfo(const string &id, const size_t nb) :
        name(id),
        ownedTypes(0),
        linearSize(nb),
        privateData(0),
        privateSize(0)
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
        
    }
}

