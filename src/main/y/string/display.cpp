#include "y/string/display.hpp"


namespace upsylon
{

    std::ostream & string_display:: align(std::ostream &os,
                                          const char   *s,
                                          const size_t  n,
                                          const size_t  width )
    {
        assert(!(s==NULL&&n>0));
        for(size_t i=0;i<n;++i)
        {
            os << s[i];
        }
        for(size_t i=n;i<width;++i)
        {
            os << ' ';
        }
        return os;
    }

    std::ostream & string_display:: align( std::ostream &os, const char   *s, const size_t width )
    {
        return string_display:: align(os, s, length_of(s), width );
    }

    std::ostream & string_display:: align( std::ostream &os, const string &s, const size_t width )
    {
        return string_display:: align(os, *s, s.size(), width);
    }

}
