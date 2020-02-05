#include "y/ios/serialized.hpp"
#include "y/ios/osstream.hpp"

namespace upsylon {

    namespace ios {

        bool serialized:: are_same_binary(const serializable &lhs, const serializable &rhs)
        {
            if( &lhs == &rhs )
            {
                return true;
            }
            else
            {
                const size_t lhs_len = lhs.serialize_length();
                const size_t rhs_len = rhs.serialize_length();
                if(lhs_len!=rhs_len)
                {
                    return false;
                }
                else
                {
                    string L(lhs_len,as_capacity,false); { ios::osstream fp(L); lhs.serialize(fp); }
                    string R(rhs_len,as_capacity,false); { ios::osstream fp(R); rhs.serialize(fp); }
                    return 0 == string::compare(L,R);
                }
            }
        }
        
    }

}
