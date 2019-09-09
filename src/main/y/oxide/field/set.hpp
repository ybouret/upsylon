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
            static void               Enroll( Fields       &db, const FieldPointer &f,  const std::type_info &t, const void *p);
            static const FieldHandle &LookUp( const Fields &db, const string       &id, const std::type_info &t);

            template <typename FIELD> static inline
            void Enroll( Fields &db, FIELD *F )
            {
                const FieldPointer   f(F);
                const std::type_info &t = typeid(FIELD);
                Enroll(db,f,t,F);
            }
            
            template <typename FIELD> static inline
            const FIELD &LookUp(const Fields &db, const string &id )
            {
                const std::type_info &t = typeid(FIELD);
                const FieldHandle    &h = LookUp(db,id,t);
                assert(h.faddr);
                return *static_cast<const FIELD *>(h.faddr);
            }



        };



    }

}

#endif



