#include "y/oxide/field/handle.hpp"

namespace upsylon
{
    namespace Oxide
    {
        FieldHandle:: ~FieldHandle() throw()
        {
        }

        FieldHandle:: FieldHandle(const FieldPointer   &f,
                                  const std::type_info &t,
                                  const void           *p) throw() :
        field(f),
        ftype(t),
        faddr(p)
        {
            assert(faddr!=NULL);
        }

        FieldHandle:: FieldHandle(const FieldHandle &_) throw() :
        field( _.field ),
        ftype( _.ftype ),
        faddr( _.faddr )
        {

        }

        const string & FieldHandle:: key() const throw()
        {
            return field->name;
        }


    }
}
