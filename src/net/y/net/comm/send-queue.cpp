
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
        
        bool send_queue:: sending() const throw()
        {
            return size>0 || readable>0;
        }
        
        size_t send_queue:: to_send() const throw()
        {
            return size+readable;
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
            Y_SENDQ_CHECK();
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
            Y_SENDQ_CHECK();
        }
        
        void send_queue:: flush() throw()
        {
            if(invalid>0) defrag();
        }
        
        void send_queue:: writeN(const void *p, const size_t n) throw()
        {
            assert(n<=writable);
            Y_SENDQ_CHECK();
            // direct copy
            memcpy(rw,p,n);
            rw                    += n;
            aliasing::_(writable) -= n;
            aliasing::_(readable) += n;
            Y_SENDQ_CHECK();
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
                    assert(buflen>writable);
                    // partial copy then store
                    const size_t w = writable;
                    if(w)
                    {
                        const size_t n = buflen-w;
                        writeN(buffer,w); assert(0==writable);
                        push(static_cast<const uint8_t*>(buffer)+w,n);
                    }
                    else
                    {
                        // really no space, store all!
                        push(buffer,buflen);
                    }
                }
            }
        }
        
        void send_queue:: remove(const size_t n) throw()
        {
            assert(n<=readable);
            Y_SENDQ_CHECK();

            // adjust buffer
            if(n>=readable)
            {
                reset_metrics();
            }
            else
            {
                ro                    += n;
                aliasing::_(readable) -= n;
                invalid               += n;
                Y_SENDQ_CHECK();
                if(invalid>0) defrag();
            }
            size_t count = min_of(writable,size);
            while( count-- > 0)
            {
                *(rw++) = pop();
                --aliasing::_(writable);
                ++aliasing::_(readable);
            }
            Y_SENDQ_CHECK();
            
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
        
        size_t send_queue:: uploaded(void *buffer, const size_t buflen) throw()
        {
            assert(!(NULL==buffer&&buflen>0));
            const size_t ns = min_of(buflen,readable);
            memcpy(buffer,ro,ns);
            remove(ns);
            return ns;
        }
        
    }
    
}

#include "y/net/tcp/client.hpp"

namespace upsylon
{
    namespace net
    {
        
        size_t send_queue:: uploaded(tcp_client &client)
        {
            Y_NET_VERBOSE(std::cerr << "[network.send_queue-->" << *client << " #" << readable << "... ]" << std::endl);
            const size_t ns = client.send(ro,readable);
            if(ns)
            {
                Y_NET_VERBOSE(std::cerr << "[network.send_queue-->" << *client << " #" << ns << "/" << readable << "]" << std::endl);
                remove(ns);
                return ns;
            }
            else
            {
                return 0;
            }
            
        }
        
    }
}
