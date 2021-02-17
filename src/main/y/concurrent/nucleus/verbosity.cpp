#include "y/concurrent/nucleus/thread.hpp"
#include "y/string/env.hpp"

namespace upsylon
{

    namespace concurrent
    {

        namespace  nucleus
        {
            bool thread:: verbosity(const char *id)
            {
                return environment::flag(id);
            }

        }

    }

}

