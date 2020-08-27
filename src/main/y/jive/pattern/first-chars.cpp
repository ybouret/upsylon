
#include "y/jive/pattern/first-chars.hpp"
#include "y/code/base2.hpp"

namespace upsylon
{

    namespace Jive
    {
        FirstChars:: ~FirstChars() throw()
        {
        }

        FirstChars:: FirstChars() : fcOrdered(4,as_capacity)
        {
        }

        

        void FirstChars:: fulfill()
        {
            if(capacity()<256)
            {
                reserve(256-capacity());
            }
            
            for(unsigned i=0;i<256;++i)
            {
                insert( uint8_t(i) );
            }

        }


    }

}
