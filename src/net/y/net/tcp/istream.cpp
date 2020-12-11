
#include "y/net/tcp/istream.hpp"

namespace upsylon
{
    namespace net
    {
        tcp_istream:: ~tcp_istream() throw()
        {
        }

        tcp_istream:: tcp_istream(const tcp_link &conn,
                                  comm_block     *cblk) :
        tcp_stream(conn),
        Q(cblk)
        {
        }

        bool tcp_istream:: query(char &C)
        {
            if( Q.size>0 || Q.download(*link)>0 )
            {
                assert(Q.size>0);
                C = char( Q.pop() );
                return true;
            }
            else
            {
                return 0;
            }
        }

        void tcp_istream:: store(char C)
        {
            Q.push_front( Q.rig(C) );
        }
    }

}
