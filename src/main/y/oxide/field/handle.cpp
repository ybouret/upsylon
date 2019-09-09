#include "y/oxide/field/handle.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldHandle:: ~FieldHandle() throw()
        {
        }

        FieldHandle:: FieldHandle( const FieldPointer &f, const type_spec &t ) throw() :
        field(f),
        ftype(t)
        {
        }

        FieldHandle:: FieldHandle(const FieldHandle &_) throw() :
        field( _.field ),
        ftype( _.ftype )
        {

        }

        const string & FieldHandle:: key() const throw()
        {
            return field->name;
        }


    }
}
