#include "y/string/io.hpp"

namespace  upsylon
{

    void string_io:: save_binary(ios::ostream &fp,
                                 const string &s)
    {
        fp.emit_upack<size_t>(s.size());
        fp << s;
    }

    string string_io:: load_binary(ios::istream &fp)
    {
        size_t n = fp.read_upack<size_t>();
        string ans(n,as_capacity);
        while(n-->0)
        {
            char C = 0;
            fp.input(&C,1);
            ans << C;
        }
        return ans;
    }

}
