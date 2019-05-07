#include "y/codec/base64-name.hpp"
#include "y/codec/base64.hpp"

namespace upsylon
{

    namespace core
    {
        void base64_name_ops:: format(char *field, const size_t bytes, uint64_t qword) throw()
        {
            assert(field);
            assert(bytes);
            memset(field,0,bytes);
            size_t pos = 0;
            while(qword)
            {
                assert(pos<bytes);
                field[pos++] = ios::base64::encode_url[ qword & 0x3f ];
                qword >>= 6;
            }
        }
    }
}
