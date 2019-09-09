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
        typedef arc_ptr<FieldType> FieldPointer;

        class FieldHandle
        {
        public:
            const FieldPointer   field;
            const std::type_info &ftype;
            const void           *faddr;

            ~FieldHandle() throw();
            explicit FieldHandle( const FieldPointer &f, const std::type_info &t, const void *p) throw();
            FieldHandle(const FieldHandle &) throw();

            const string & key() const throw();

        private:
            Y_DISABLE_ASSIGN(FieldHandle);
        };
        
    }
}


#endif


