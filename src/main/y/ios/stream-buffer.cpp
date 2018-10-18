#include "y/ios/stream-buffer.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace ios
    {
        stream_buffer:: ~stream_buffer() throw()
        {
            memory::global::location().release( *(void **)&head, wlen);
            head = 0;
            curr = 0;
            last = 0;
            (size_t &)maxi = 0;
        }

        stream_buffer:: stream_buffer(const size_t n) :
        maxi( n ),
        wlen( memory::align(max_of<size_t>(n,1)) ),
        head( static_cast<uint8_t *>(memory::global::instance().acquire(wlen)) ),
        curr(head),
        last(curr)
        {

        }

        const void * stream_buffer:: ro()     const throw() { return curr; }
        size_t       stream_buffer:: length() const throw() { return last-curr; }

        bool stream_buffer:: query( char & C) throw()
        {
            if(curr<last)
            {
                C = *(curr++);
                return true;
            }
            else
            {
                return false;
            }
        }

    }

}

