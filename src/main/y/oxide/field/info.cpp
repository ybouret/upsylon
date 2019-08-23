
#include "y/oxide/field/info.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldInfo:: ~FieldInfo() throw()
        {
        }

        FieldInfo:: FieldInfo(const string &id ) :
        name(id)
        {
        }

        FieldInfo:: FieldInfo(const char *id ) :
        name(id)
        {
        }
        
    }
}

