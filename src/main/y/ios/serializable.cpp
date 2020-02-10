
#include "y/ios/serializable.hpp"
#include "y/ios/null-ostream.hpp"
#include "y/ios/ohstream.hpp"

namespace upsylon {

    namespace ios {

        serializable:: serializable() throw()
        {
        }

        serializable:: ~serializable() throw()
        {
        }

        size_t serializable:: serialize_length() const
        {
            ios::null_ostream dev_null;
            return serialize(dev_null);
        }

        size_t serializable:: serialize_className( ios::ostream &fp ) const
        {
            return fp.emit_block( className() );
        }

        size_t serializable:: serialize_className_length() const
        {
            ios::null_ostream dev_null;
            return serialize_className(dev_null);
        }

        size_t serializable:: serialize_class( ios::ostream &fp ) const
        {
            size_t len = serialize_className(fp);
            return len + serialize(fp);
        }

        size_t serializable:: serialize_class_length() const
        {
            ios::null_ostream dev_null;
            return serialize_class(dev_null);
        }

        size_t serializable:: hash_className_with( hashing::function &H ) const
        {
            ios::ohstream fp(H);
            return serialize_className(fp);
        }

        size_t serializable:: hash_with( hashing::function &H ) const
        {
            ios::ohstream fp(H);
            return serialize(fp);
        }

        size_t  serializable:: hash_class_with( hashing::function &H) const
        {
            ios::ohstream fp(H);
            return serialize_class(fp);
        }

        digest serializable:: md(hashing::function &H) const
        {
            H.set();
            (void) hash_with(H);
            return H.md();
        }


    }
}

#include "y/ios/osstream.hpp"
#include "y/ios/ocstream.hpp"
#include "y/exception.hpp"
#include "y/codec/base64.hpp"

namespace upsylon {

    namespace ios {

        string serializable::to_binary() const
        {
            static const char excpfmt[] = "serializable.to_binary(sizes mismatch@level-%d)";

            const  size_t toWrite = serialize_length();
            string ans( toWrite, as_capacity, false );
            {
                ios::osstream fp(ans);
                const size_t  written = serialize(fp);
                if(written!=toWrite) throw exception(excpfmt,1);
            }
            if(ans.size()!=toWrite)  throw exception(excpfmt,2);
            return ans;
        }

        string serializable:: to_base64() const
        {
            const string ans = to_binary();
            ios::base64::encoder b64;
            return b64.to_string(ans);
        }

        size_t serializable:: save_to(const string &binaryFile, const bool append) const
        {
            ios::ocstream fp(binaryFile,append);
            return serialize(fp);
        }

        size_t serializable:: save_to(const char *binaryFile, const bool append) const
        {
            const string _(binaryFile);
            return save_to(_,append);
        }
    }
}

#include "y/string/convert.hpp"

namespace upsylon {

    namespace ios {

        string serializable:: to_visible() const
        {
            const  string ans = to_binary();
            return string_convert::to_visible(ans);
        }

        string serializable:: to_printable() const
        {
            const  string ans = to_binary();
            return string_convert::to_printable(ans);
        }

    }

}
