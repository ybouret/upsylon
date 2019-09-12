//! \file
#ifndef Y_FIELD_SET_INCLUDED
#define Y_FIELD_SET_INCLUDED 1

#include "y/oxide/field/handle.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace Oxide
    {
        typedef set<string,FieldHandle> FieldsType; //!< alias

        //! data base for fields
        class Fields : public FieldsType
        {
        public:
            explicit Fields() throw(); //!< setup
            virtual ~Fields() throw(); //!< cleanup

            //! enroll a field pointer with the type_info of the fully qualified field
            void               __enroll( const FieldPointer &f,  const std::type_info &t, const void *p);

            //! look up the field handle using its name and fully qualified type_info
            const FieldHandle &__lookUp( const string       &id, const std::type_info &t) const;

            //! use type info to enroll any field
            template <typename FIELD> inline
            void enroll( FIELD *F )
            {
                const FieldPointer    f(F);
                const std::type_info &t = typeid(FIELD);
                __enroll(f,t,F);
            }

            //! use type info to match an enrolled  field
            template <typename FIELD> inline
            const FIELD &lookUp(const string &id ) const
            {
                const std::type_info &t = typeid(FIELD);
                const FieldHandle    &h = __lookUp(id,t);
                assert(h.faddr);
                return *static_cast<const FIELD *>(h.faddr);
            }

            //! test really owned
            bool owns( const Field &F ) const throw();

            //! get field type
            Field & operator[](const string &id);

            //! get field type, wrapper
            Field & operator[](const char *id);

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Fields);
            static const char Fn[];


        };



    }

}

#endif



