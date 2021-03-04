#include "y/gfx/types.hpp"
#include "y/exceptions.hpp"

namespace upsylon
{
    namespace graphic
    {
        unit_t checking:: geqz(const unit_t value, const char *name)
        {
            assert(name);
            if(value<0) throw exception("%s<0",name);
            return value;
        }
        
        const char checking:: height[] = "height";
        const char checking:: width[]  = "width";
    }
}
