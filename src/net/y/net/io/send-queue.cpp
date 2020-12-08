
#include "y/net/io/send-queue.hpp"
#include "y/type/utils.hpp"
#include "y/code/utils.hpp"

namespace upsylon
{
    namespace net
    {

        send_queue:: ~send_queue() throw()
        {

        }

        send_queue:: send_queue(const size_t bs) :
        io_queue(bs),
        origin( data.as<uint8_t>() ),
        offset( 0 ),
        current( origin ),
        written( 0 ),
        beginning( origin ),
        available( data.length() )
        {
            
        }

        void send_queue:: resetData() throw()
        {
            offset    = 0;
            current   = origin;
            written   = 0;
            beginning = origin;
            available = data.length();
        }


        void send_queue:: pack() throw()
        {
            if(size<=available)
            {
                transfer(size);
            }
            else
            {
                defrag();
                transfer(min_of(size,available));
            }
        }

        void send_queue:: defrag() throw()
        {
            if(offset>0)
            {
                memmove(origin,current,written);
                beginning -= offset;
                available += offset;
                current   =  origin;
                offset    =  0;
            }
        }

        void send_queue:: transfer(size_t n) throw()
        {
            assert(n<=size);
            assert(n<=available);
            while(n-- > 0 )
            {
                *(beginning++) = pop();
                --available;
                ++written;
            }
        }

        size_t send_queue:: packed() const throw()
        {
            return written;
        }


        void send_queue:: update(const size_t n) throw()
        {
            assert(n<=written);
            current += n;
            written -= n;
            if(written>0)
            {
                offset  += n;
            }
            else
            {
                resetData();
            }
        }

        void send_queue:: reset_() throw()
        {
            resetData();
        }



        std::ostream &send_queue:: display(std::ostream &os) const
        {
            os << '[';
            os << '+' << offset;
            os << '|';
            for(size_t i=0;i<written;++i)
            {
                os << cchars::visible[ current[i] ];
            }
            os << '|';
            os << '+' << available;
            os << ']';
            const io_queue &self = *this;
            os << '[' << self << ']';
            return os;
        }

        std::ostream & operator<<(std::ostream &os,const send_queue &Q)
        {
            return Q.display(os);
        }


    }

}

#include "y/net/tcp/client.hpp"

namespace upsylon
{
    namespace net
    {

        bool send_queue:: uploaded(tcp_client &client)
        {
            pack();
            if(written)
            {
                update( client.send(current,written) );
            }
            return size<=0 && written<=0;
        }

    }
}
