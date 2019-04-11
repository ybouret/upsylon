
#include "y/ios/serializable.hpp"
#include "y/ios/null-ostream.hpp"
#include "y/ios/ohstream.hpp"

namespace upsylon
{
    namespace ios
    {
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



    }
}
