//! \file
#ifndef Y_FIELD_SET_INCLUDED
#define Y_FIELD_SET_INCLUDED 1

#include "y/oxide/field/handle.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace Oxide
    {
        typedef set<string,FieldHandle> Fields;

        struct __Fields
        {
            static  void Register( Fields &db, const FieldPointer &f, const type_spec &t );

            template <typename FIELD>
            static inline void Register( Fields &db, FIELD *F )
            {
                const FieldPointer f(F);
                const type_spec    t = typeid(FIELD);
                Register(db,f,t);
            }
        };



    }

}

#endif



