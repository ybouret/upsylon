
#include "y/gfx/image/named-format.hpp"
#include "y/jive/regexp.hpp"
#include "y/string/convert.hpp"

namespace upsylon
{
    namespace graphic
    {

        namespace image
        {
            named_format:: ~named_format() throw()
            {
            }

            static inline
            string rx_to_format_rx(const char *rx)
            {
                string tmp = rx;
                string_convert::make_lower(tmp);
                return '(' + tmp + ')' + '&';
            }

            named_format:: named_format(const char *id, const char *rx) :
            format(),
            name(id),
            extension_lowercase_regexp( rx_to_format_rx(rx) ),
            extension_compiled_pattern( Jive::RegExp(extension_lowercase_regexp,NULL) )
            {

            }

            bool named_format:: matches(const string &ext) const
            {
                Jive::Token  token;
                Jive::Source source( Jive::Module::OpenData(ext) );
                if( extension_compiled_pattern->accept(token,source) )
                {
                    return source.is_broken();
                }
                else
                {
                    return false;
                }
            }


            const string &  named_format::key() const throw()
            {
                return name;
            }

        }
    }
}
