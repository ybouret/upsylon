#include "y/net/comm/queue.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace net
    {

        comm_queue:: ~comm_queue() throw()
        {
        }

        comm_queue:: comm_queue(const size_t bs) :
        comm_bytes(),
        data(bs)
        {
            reserve( data.length() );
        }



        void comm_queue:: reset() throw()
        {
            clear();
            reset_();
            aliasing::_(data).ldz();
        }




    }

}


