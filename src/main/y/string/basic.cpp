#include "y/string/basic.hpp"
#include "y/memory/pooled.hpp"

#include "y/code/utils.hpp"

#include "y/ios/ostream.hpp"
#include <iostream>

namespace upsylon
{
    namespace core
    {

        memory::allocator & string_allocator_instance()
        {
            return memory::pooled::instance();
        }

        memory::allocator & string_allocator_location() throw()
        {
            assert(memory::pooled::exists());
            return memory::pooled::location();
        }


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

        template <>
        bool string<char>:: compact() throw()
        {
            static  memory::pooled &mgr = memory::pooled::location();

            if(mgr.compact_as(addr_,items,bytes,size_+1))
            {
                maxi_ = items - 1;
                return true;
            }
            else
            {
                return false;
            }
        }


    }
}

