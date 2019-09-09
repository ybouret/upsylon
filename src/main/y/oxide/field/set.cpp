#include "y/oxide/field/set.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace Oxide
    {
        void __Fields:: Enroll(Fields               &db,
                               const FieldPointer   &f,
                               const std::type_info &t,
                               const void           *p)
        {
            const FieldHandle h(f,t,p);
            if( !db.insert(h) )
            {
                throw exception("Oxide::Multiple Field '%s', requested type <%s>", * h.key(), t.name());
            }
        }

        const FieldHandle &__Fields:: LookUp( const Fields &db, const string       &id, const std::type_info &t)
        {
            const FieldHandle *h = db.search(id);
            if(!h) throw exception("Oxide::No Field '%s'",*id);

            if(h->ftype != t )
            {
                throw exception("Oxide:: %s<%s> type mismatch <%s>", *id, h->ftype.name(), t.name() );
            }
            return *h;
        }
    }

}


