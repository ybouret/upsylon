
#include "y/ipso/fields.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ipso
    {
        fields:: ~fields() throw()
        {
        }

        fields:: fields( const size_t n ) : fields_set(n,as_capacity) {}

        void fields:: __add( field_info *F )
        {
            assert(F);
            const field_pointer p(F);
            if(!insert(F))
            {
                throw exception("multiple '%s' fields", *(F->name));
            }
        }

    }
}
