#include "y/string/basic.hpp"
#include <iostream>
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace core
    {
        template <>
        std::ostream & core::string<char>::std_display(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << addr_[i];
            return os;
        }


        template <>
        std::ostream & core::string<char>:: display_visible(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << visible_char[ uint8_t(addr_[i]) ];
            return os;
        }

        template <>
        std::ostream & core::string<char>:: display_printable(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << printable_char[ uint8_t(addr_[i]) ];
            return os;
        }
    }
}

