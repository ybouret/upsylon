
#include "y/net/comm/send-queue.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"
#include "y/code/utils.hpp"
#include <cstring>

namespace upsylon
{
    namespace net
    {
        
#define Y_SENDQ_CHECK()       \
assert(ro== **data + invalid);\
assert(rw==ro+readable);      \
assert(invalid+readable+writable==data->size)
        
        send_queue:: ~send_queue() throw()
        {
            reset_metrics();
        }
        
        send_queue:: send_queue(comm_block *block) :
        comm_queue(block),
        writable(data->size),
        readable(0),
        invalid(0),
        rw(**data),
        ro(rw)
        {
            Y_SENDQ_CHECK();
        }
        
        void send_queue:: reset_metrics() throw()
        {
            invalid               = 0;
            rw                    = **data;
            ro                    = **data;
            aliasing::_(readable) = 0;
            aliasing::_(writable) = data->size;
            Y_SENDQ_CHECK();
        }
        
        
        void send_queue:: reset_() throw()
        {
            reset_metrics();
        }
        
        void send_queue:: defrag() throw()
        {
            Y_SENDQ_CHECK();
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
            Y_SENDQ_CHECK();
        }
        
        void send_queue:: write1(const uint8_t code) throw()
        {
            Y_SENDQ_CHECK();
            assert(writable);
            
            *(rw++) = code;
            --aliasing::_(writable);
            ++aliasing::_(readable);
            Y_SENDQ_CHECK();
        }
        
        void send_queue:: write(char C)
        {
            if(writable<=0)
            {
                if(invalid>0)
                {
                    // claim space
                    defrag();
                    assert(writable>0);
                    write1(C);
                }
                else
                {
                    // really no space!
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
        
        void send_queue:: writeN(const void *p, const size_t n) throw()
        {
            assert(n<=writable);
            // direct copy
            memcpy(rw,p,n);
            rw                    += n;
            aliasing::_(writable) -= n;
            aliasing::_(readable) += n;
        }
        
        void send_queue:: output(const void *buffer, const size_t buflen)
        {
            assert(!(0==buffer&&buflen>0));
            std::cerr << "send_queue output #" << buflen << "/" << writable << std::endl;
            if(buflen<=writable)
            {
                writeN(buffer,buflen);
            }
            else
            {
                assert(buflen>writable);
                // reclaim space
                if(invalid>0)
                    defrag();
                
                if(buflen<=writable)
                {
                    // full copy
                    writeN(buffer,buflen);
                }
                else
                {
                    // partial copy then store
                    const size_t w = writable;
                    const size_t n = buflen-w;
                    writeN(buffer,w); assert(0==writable);
                    push(static_cast<const uint8_t*>(buffer)+w,n);
                }
                
                
            }
        }
        
        
        std::ostream &send_queue:: display(std::ostream &os) const
        {
            os << '[';
            os << invalid;
            os << '<';
            for(size_t i=0;i<readable;++i)
            {
                os << cchars::visible[ ro[i] ];
            }
            os << '>';
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
