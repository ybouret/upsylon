#include "y/string/basic.hpp"
#include <iostream>
#include "y/code/utils.hpp"
#include "y/ios/ostream.hpp"

namespace upsylon
{
    namespace core
    {
        template <>
        std::ostream & string<char>::std_display(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << addr_[i];
            return os;
        }


        template <>
        std::ostream & string<char>:: display_visible(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << visible_char[ uint8_t(addr_[i]) ];
            return os;
        }

        template <>
        std::ostream & string<char>:: display_printable(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << printable_char[ uint8_t(addr_[i]) ];
            return os;
        }

        template <>
        const char string<char>:: CLASS_NAME[] = "string";
        
        template <>
        const char * string<char>:: className() const throw()
        {
            return CLASS_NAME;
        }

        template <>
        size_t string<char>:: serialize(ios::ostream &fp) const
        {
            return fp.emit_block(addr_,size_);
        }



    }
}

