
#include "y/net/tcp/ostream.hpp"

namespace upsylon
{
    namespace net
    {

        tcp_ostream:: ~tcp_ostream() throw()
        {
        }


        tcp_ostream:: tcp_ostream(const tcp_link &conn,
                                  comm_block     *cblk) :
        tcp_stream( conn ),
        Q(cblk)
        {
        }

        void tcp_ostream:: flush()
        { 
        }

        void tcp_ostream:: write(char C)
        {
            Q.write(C);
            if(Q.writable<=0)
            {
                flush();
            }
        }

        void tcp_ostream:: output(const void  *buffer,
                                  const size_t buflen)
        {
            Q.output(buffer,buflen);
            if(Q.writable<=0)
            {
                flush();
            }
        }
    }

}

