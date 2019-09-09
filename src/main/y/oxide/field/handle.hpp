//! \file
#ifndef Y_FIELD_HANDLE_INCLUDED
#define Y_FIELD_HANDLE_INCLUDED 1

#include "y/oxide/field/type.hpp"
#include "y/ptr/arc.hpp"
#include <typeinfo>

namespace upsylon
{
    namespace Oxide
    {
        typedef arc_ptr<FieldType> FieldPointer; //!< alias for dynamic fields

        //! lightweight handle for fields
        class FieldHandle
        {
        public:
            const FieldPointer   field;  //!< holds the dynamic field
            const std::type_info &ftype; //!< holds the type info
            const void           *faddr; //!< holds the derived field address

            //! cleanup
            ~FieldHandle() throw();

            //! setup
            explicit FieldHandle(const FieldPointer   &f,
                                 const std::type_info &t,
                                 const void           *p) throw();

            //! copy
            FieldHandle(const FieldHandle &) throw();

            //! key for associative set: f->name
            const string & key() const throw();

        private:
            Y_DISABLE_ASSIGN(FieldHandle);
        };
        
    }
}


#endif


