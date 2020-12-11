
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
        writable(0),
        readable(0),
        invalid(0),
        rw(**data),
        ro(rw)
        {
            
        }

        void send_queue:: reset_metrics() throw()
        {
            invalid = 0;
            rw      = **data;
            ro      = **data;
            aliasing::_(readable) = 0;
            aliasing::_(writable) = 0;
        }
        

        void send_queue:: reset_() throw()
        {
            reset_metrics();
        }

        void send_queue:: defrag() throw()
        {
            assert(invalid>0);
            uint8_t *org = **data;
            {
                uint8_t *target = org;
                uint8_t *source = target+invalid; assert(ro==source);
                for(size_t i=0;i<readable;++i)
                {
                    *(target++) = *source;
                    *(source++) = 0;
                }
            }
            ro = org;
            rw = org + readable;
            aliasing::_(writable) += invalid;
            invalid = 0;
            assert(readable+writable==data->size);
        }

        void send_queue:: write1(const uint8_t code) throw()
        {
            assert(writable);

            *(rw++) = code;
            --aliasing::_(writable);
            ++aliasing::_(readable);
        }

        void send_queue:: write(char C)
        {
            if(writable<=0)
            {
                if(invalid>0)
                {
                    defrag();
                    assert(writable>0);
                    write1(C);
                }
                else
                {
                    push_back( rig(C) );
                }
            }
            else
            {
                write1(C);
            }
        }

        void send_queue:: flush() throw()
        {

        }

        void send_queue:: writeN(const void *buffer, const size_t buflen) throw()
        {
            assert(buflen<=writable);
            // direct copy
            memcpy(rw,buffer,buflen);
            rw                    += buflen;
            aliasing::_(writable) -= buflen;
            aliasing::_(readable) += buflen;
        }

        void send_queue:: output(const void *buffer, const size_t buflen)
        {
            assert(!(0==buffer&&buflen>0));
            if(buflen<=writable)
            {
                writeN(buffer,buflen);
            }
            else
            {
                assert(buflen>0);

            }
        }


        std::ostream &send_queue:: display(std::ostream &os) const
        {
            os << '[';
            os << invalid;
            os << '>';
            for(size_t i=0;i<readable;++i)
            {
                os << cchars::visible[ ro[i] ];
            }
            os << '<';
            os << writable;
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
            return false;
        }

    }
}
