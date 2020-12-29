

#include "y/mkl/fitting/variable/replica.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {
            replica_variable:: ~replica_variable() throw()
            {
            }

            size_t  replica_variable:: get_index() const throw()
            {
                return source->get_index();
            }

        }
    }
}
