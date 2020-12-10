
#include "y/net/comm/send-queue.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"
#include <cstring>

namespace upsylon
{
    namespace net
    {

        send_queue:: ~send_queue() throw()
        {

        }

        send_queue:: send_queue(const size_t bs) :
        comm_queue(bs),
        origin( aliasing::_(data).as<uint8_t>() ),
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
                {
                    uint8_t *target = origin;
                    uint8_t *source = (uint8_t *)current;
                    for(size_t i=written;i>0;--i)
                    {
                        (*target++) = *source;
                        (*source++) = 0;
                    }
                }
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

        void send_queue:: write(char C)
        {
            push_back( rig(C) );
        }

        void send_queue:: flush() throw()
        {
            pack();
        }

        void send_queue:: output(const void *buffer, const size_t buflen)
        {
            push(buffer,buflen);
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
            const comm_queue &self = *this;
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
