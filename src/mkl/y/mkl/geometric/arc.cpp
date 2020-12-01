
#include "y/mkl/geometric/arc.hpp"
#include "y/type/spec.hpp"
#include "y/exception.hpp"

namespace upsylon
{

    namespace mkl
    {

        namespace Geometric
        {

            Arc_:: Arc_() throw() : Object() {}

            Arc_:: ~Arc_() throw() {}

            void Arc_:: nodeInsertFailure(const std::type_info &tid) const
            {
                const char *id = *type_name_for(tid);
                throw exception("Arc<%s> cannot insert NEW node...",id);
            }


        }

    }

}


