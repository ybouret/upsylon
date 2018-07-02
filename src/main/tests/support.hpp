
#ifndef SUPPORT_INCLUDED
#define SUPPORT_INCLUDED 1

#include "y/string.hpp"
#include "y/alea.hpp"

using namespace upsylon;

namespace {


    struct support
    {

        template <typename T>
        static inline T get()
        {
            return alea.template full<T>();
        }

    };

    template <>
    inline string support:: get<string>()
    {
        const size_t len = 1+ alea.leq(15);
        string ans(len,as_capacity);
        for(size_t i=0;i<len;++i)
        {
            ans += alea.range<char>('a','z');
        }
        return ans;
    }
}

#endif

