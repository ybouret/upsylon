
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
            return (privateData = memory::global::instance().acquire(privateSize));
        }

        void FieldInfo:: releasePrivate() throw()
        {
            assert(ownedTypes==0);
            if(privateSize)
            {
                memory::global::location().release(privateData,privateSize);
            }
        }
        
    }
}

