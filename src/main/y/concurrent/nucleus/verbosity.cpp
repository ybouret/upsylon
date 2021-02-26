#include "y/concurrent/nucleus/thread.hpp"
#include "y/string/env.hpp"
#include <iostream>

namespace upsylon
{

    namespace concurrent
    {

        namespace  nucleus
        {
            bool thread:: verbosity(const char *id)
            {
                assert(id);
                std::cerr << "thread::verbosity(" << id << ")" << std::endl;
                const bool ans =  environment::flag(id);
                std::cerr << "thread::verbosity(" << id << ")=" << ans << std::endl;
                return ans;
            }

        }

    }

}

