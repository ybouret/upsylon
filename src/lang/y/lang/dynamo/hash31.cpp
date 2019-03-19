
#include "y/lang/dynamo/hash31.hpp"


namespace upsylon
{
    namespace Lang
    {
        DynamoHash31:: DynamoHash31() throw() :
        h()
        {
        }

        DynamoHash31:: ~DynamoHash31() throw()
        {
        }

        int32_t DynamoHash31:: operator()(const memory::ro_buffer &buf) throw()
        {
            static const int32_t mask = 0x7fffffff;

            int32_t ans = 0;

            h.set();
            h.run( buf.ro(), buf.length() );
            h.get(&ans,sizeof(ans));

            ans &= mask;
            assert(ans>=0);
            return ans;
            
        }
    }

}

