
#include "y/mkl/fitting/samples.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            void _samples:: throw_multiple_sample(const string &name)
            {
                throw exception("samples: multiple '%s'", *name);
            }
        }

    }

}

