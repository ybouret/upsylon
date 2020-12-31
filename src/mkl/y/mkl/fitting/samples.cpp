
#include "y/mkl/fitting/samples.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace mkl
    {
        namespace fitting
        {

            void _samples:: throw_multiple_sample(const string &owner, const string &name)
            {
                throw exception("in fitting '%s': multiple '%s'", *owner, *name);
            }

            void _samples:: throw_missing_sample(const string &owner, const string &name)
            {
                throw exception("in fitting '%s': missing '%s'", *owner, *name);
            }
        }

    }

}

