
#include "y/string/temporary-name.hpp"
#include "y/type/utils.hpp"
#include "y/randomized/bits.hpp"
namespace upsylon
{
    namespace
    {
        static const char file_name_chars[] =
        {
            "abcdefghijklmnopqrstuvwxyz"
            "ABCDEFGHIJKLMNOPQRSTUCWXYZ"
            "0123456789"
            "_"
        };
    }

    string temporary_name(const size_t length)
    {
        static randomized::bits &ran = randomized::bits::crypto();
        static const char        nch = length_of(file_name_chars);
        const size_t n = max_of<size_t>(1,length);
        string       ans(n+4,as_capacity);
        for(size_t i=0;i<n;++i)
        {
            ans += file_name_chars[ ran.lt(nch) ];
        }
        ans += ".tmp";
        assert(4+n==ans.size());
        return ans;
    }

}
