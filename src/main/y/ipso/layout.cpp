
#include "y/ipso/layout.hpp"

namespace upsylon
{
    namespace ipso
    {
        layout:: ~layout() throw()
        {
        }

        layout:: layout(const size_t dims) throw() :
        dimensions(dims),
        items(0)
        {
        }

        layout:: layout(const layout &other) throw() :
        dimensions(other.dimensions),
        items(other.items)
        {
        }
        
    }
}
