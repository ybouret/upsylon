#include "y/oxide/field/set.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {
        void __Fields:: Register(Fields             &db,
                                 const FieldPointer &f,
                                 const type_spec    &t )
        {
            const FieldHandle h(f,t);
            if( !db.insert(h) )
            {
                throw exception("Oxide::Multiple Field '%s' of type <%s>", * h.key(), *t.key());
            }
        }
    }

}


