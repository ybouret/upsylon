#include "y/associative/prefix/stump.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    template <> const char * prefix_ops:: code2text<int8_t>( const int8_t c ) throw()
    {
        return cchars::visible[ uint8_t(c) ] ;
    }
    
}
