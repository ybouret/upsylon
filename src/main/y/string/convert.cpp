#include "y/string/convert.hpp"
#include "y/exceptions.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{

    static const char default_name[] = "variable";

    static inline
    size_t __to_size_dec( const string &data, const char *id)
    {
        assert(id);
        return 0;
    }


    static inline
    size_t __to_size_hex( const string &data, const char *id)
    {
        assert(id);
        size_t ans = 0;
        for(size_t i=2;i<data.size();++i)
        {
        }
        return ans;
    }


    template <>
    size_t string_convert::to<size_t>(const string &data, const char *name)
    {
        static const char fn[] = "string_convert::to<size_t>: ";
        const char       *id   = name ? name : default_name;
        if(data.size()<=0)
        {
            throw exception("%sempty %s string",fn,id);
        }

        if(data.size()>2&&data[0]=='0'&&data[1]=='x')
        {
            return __to_size_hex(data,name);
        }
        else
        {
            return __to_size_dec(data,name);
        }
    }
}
