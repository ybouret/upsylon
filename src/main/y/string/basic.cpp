#include "y/string/basic.hpp"
#include <iostream>

namespace upsylon
{
    namespace core
    {
        template <>
        void core::string<char>::std_display(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << addr_[i];
        }

    }
}

