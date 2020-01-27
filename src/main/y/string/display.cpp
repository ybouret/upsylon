#include "y/string/display.hpp"


namespace upsylon
{

    std::ostream & string_display:: align(std::ostream &os,
                                          const char   *s,
                                          const size_t  n,
                                          const size_t  width,
                                          const char   *prefix,
                                          const char   *suffix)
    {
        assert(!(s==NULL&&n>0));
        if(prefix) os << prefix;
        for(size_t i=0;i<n;++i)
        {
            os << s[i];
        }
        if(suffix) os << suffix;
        for(size_t i=n;i<width;++i)
        {
            os << ' ';
        }
        return os;
    }

    std::ostream & string_display:: align(std::ostream & os,
                                          const char    *s,
                                          const size_t   width,
                                          const char    *prefix,
                                          const char    *suffix )
    {
        return string_display:: align(os, s, length_of(s), width,prefix,suffix);
    }

    std::ostream & string_display:: align(std::ostream & os,
                                          const string & s,
                                          const size_t   width,
                                          const char    *prefix,
                                          const char    *suffix )
    {
        return string_display:: align(os, *s, s.size(), width,prefix,suffix);
    }

}
