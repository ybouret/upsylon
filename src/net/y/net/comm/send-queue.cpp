
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

        send_queue:: send_queue(comm_block *block) :
        comm_queue(block),
        origin( **data ),
        offset( 0 ),
        current( origin ),
        written( 0 ),
        beginning( origin ),
        available( data->size )
        {
            
        }

        void send_queue:: resetData() throw()
        {
            offset                 = 0;
            current                = origin;
            aliasing::_(written)   = 0;
            beginning              = origin;
            aliasing::_(available) = data->size;
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
                beginning              -= offset;
                aliasing::_(available) += offset;
                current   =  origin;
                offset    =  0;
                pack();
            }
        }

        void send_queue:: transfer(size_t n) throw()
        {
            assert(n<=size);
            assert(n<=available);
            while(n-- > 0 )
            {
                *(beginning++) = pop();
                --aliasing::_(available);
                ++aliasing::_(written);
            }
        }

        
        void send_queue:: update(const size_t n) throw()
        {
            assert(n<=written);
            current              += n;
            aliasing::_(written) -= n;
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
            if(available)
            {
                *(beginning++) = C;
                --aliasing::_(available);
                ++aliasing::_(written);
            }
            else
            {
                push_back( rig(C) );
            }
        }

        void send_queue:: flush() throw()
        {
            pack();
        }

        void send_queue:: output(const void *buffer, const size_t buflen)
        {
            assert( !(NULL==buffer&&buflen>0));
            const uint8_t *p = static_cast<const uint8_t *>(buffer);
            size_t         n = buflen;
            while(available&&n)
            {
                *(beginning++) = *(p++);
                --aliasing::_(available);
                ++aliasing::_(written);
                --n;
            }
            while(n-- > 0)
            {
                push_back( rig(*(p++) ));
            }
        }


        std::ostream &send_queue:: display(std::ostream &os) const
        {
            os << '[';
            os << offset;
            os << '>';
            for(size_t i=0;i<written;++i)
            {
                os << cchars::visible[ current[i] ];
            }
            os << '<';
            os << available;
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
