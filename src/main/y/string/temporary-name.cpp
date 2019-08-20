
#include "y/string/temporary-name.hpp"
#include "y/type/utils.hpp"
#include "y/randomized/bits.hpp"
#include "y/memory/pooled.hpp"
#include "y/sequence/vector.hpp"

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

    string temporary_name:: create(const size_t length)
    {
        static randomized::bits &ran = randomized::bits::crypto();
        static const size_t      nch = length_of(file_name_chars);
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

    void temporary_name:: create(sequence<string> &names,
                                 const size_t      count,
                                 const size_t      length)
    {
        vector<string,memory::pooled> local_names(count,as_capacity);
        for(size_t i=1;i<=count;++i)
        {
        CREATE1:
            string name = create(length);
            for(size_t j=local_names.size();j>0;--j)
            {
                if(local_names[j]==name) goto CREATE1;
            }
            local_names.push_back_(name);
        }
        for(size_t i=1;i<=count;++i)
        {
            names.push_back( local_names[i] );
        }
    }

}
