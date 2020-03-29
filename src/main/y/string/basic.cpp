#include "y/string/basic.hpp"
#include "y/memory/pooled.hpp"

#include "y/code/utils.hpp"

#include "y/ios/ostream.hpp"
#include "y/ios/istream.hpp"
#include "y/exception.hpp"

#include <iostream>

namespace upsylon {

    namespace core {

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
            for(size_t i=0;i<size_;++i) os << cchars::visible[ uint8_t(addr_[i]) ];
            return os;
        }

        template <>
        std::ostream & string<char>:: display_printable(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << cchars::printable[ uint8_t(addr_[i]) ];
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
            return fp.write_block(addr_,size_);
        }

        template <>
        string<char> string<char>:: read(ios::istream &fp, size_t *shift, const string &which)
        {
            static const char fn[] = "string::read";
            size_t nr    = 0;
            size_t chars = 0;
            if(!fp.query_upack(chars,&nr)) throw exception("%s(missing #chars for '%s')",fn,*which);
            string ans(chars,as_capacity,true);
            const size_t nc = fp.try_get(ans.addr_,chars);
            if(nc!=chars) throw exception("%s(missing chars for '%s')",fn,*which);
            
            ios::gist::assign(shift,nr+chars);
            ans.size_ = chars;
            return ans;
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

namespace upsylon     {

    namespace core {

        template <>
        const char string<ptrdiff_t>:: CLASS_NAME[] = "lstring";

        template <>
        const char * string<ptrdiff_t>:: className() const throw()
        {
            return CLASS_NAME;
        }

        template <>
        size_t string<ptrdiff_t>:: serialize(ios::ostream &fp) const
        {
            size_t ans = fp.write_upack(size_);
            
            //Y_OSTREAM_ADD_TO(ans, fp.emit_upack, size_);
            for(size_t i=0;i<size_;++i)
            {
                ans += fp.write_nbo(addr_[i]);
            }
            return ans;
        }

        template <>
        std::ostream & string<ptrdiff_t>::std_display(std::ostream &os) const
        {
            for(size_t i=0;i<size_;++i) os << '[' << addr_[i] << ']';
            return os;
        }

    }

}
