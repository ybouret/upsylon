
#include "y/memory/xslot.hpp"
#include <cstring>

namespace upsylon
{
    namespace memory
    {
        xslot_type:: ~xslot_type() throw()
        {
            assert(0==size);
            assert(0==data);
            assert(0==kill);
        }

        void xslot_type:: free() throw()
        {
            if(data)
            {
                assert(size>0);
                would_kill();
                memset(data,0,size);
            }
        }

        xslot_type:: xslot_type() throw() :
        size(0),
        data(0),
        kill(0)
        {
        }

        void xslot_type:: would_kill() throw()
        {
            if(kill)
            {
                assert(data!=0);
                assert(size>0);
                kill(data);
                kill = 0;
            }
        }

        bool xslot_type:: is_cplusplus() const throw()
        {
            return kill!=0;
        }



    }

}

