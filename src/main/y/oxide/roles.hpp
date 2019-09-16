//! \file

#ifndef Y_OXIDE_ROLES_INCLUDED
#define Y_OXIDE_ROLES_INCLUDED 1

#include "y/os/platform.hpp"

namespace upsylon
{
    namespace Oxide
    {
        struct Role
        {
            enum Type
            {
                IsControlling,
                IsSubordinate
            };
        };


        typedef int2type<Role::IsControlling> ControllingType;
        typedef int2type<Role::IsSubordinate> SubordinateType;
        
        extern const ControllingType Controlling;
        extern const SubordinateType Subordinate;
    }
}

#endif

