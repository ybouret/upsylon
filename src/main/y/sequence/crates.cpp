#include "y/sequence/crates.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace core
    {
        void crates:: reserve_failure()
        {
            throw exception("crates: no possible reserve!");
        }
    }
}
