
#include "y/oxide/field/info.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldInfo:: ~FieldInfo() throw()
        {
            releasePrivate();
        }

        FieldInfo:: FieldInfo(const string &id ) :
        name(id),
        owned(0),
        privateData(0),
        privateSize(0)
        {
        }

        FieldInfo:: FieldInfo(const char *id ) :
        name(id),
        owned(0),
        privateData(0),
        privateSize(0)
        {
        }

        void   FieldInfo:: acquirePrivate()
        {
            privateData = memory::global::instance().acquire(privateSize);
        }

        void FieldInfo:: releasePrivate() throw()
        {
            assert(owned==0);
            if(privateSize)
            {
                memory::global::location().release(privateData,privateSize);
            }
        }
        
    }
}

