
#include "y/ios/orstream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        orstream:: ~orstream() throw()
        {
        }

#define Y_IOS_ORSTREAM0() local_file(filename, writable | ( append ? 0 : truncate) )
#define Y_IOS_ORSTREAM() \
iobuf(),                 \
curr(iobuf.data),        \
last(curr+iobuf.size)



        orstream:: orstream( const string &filename, bool append ) :
        Y_IOS_ORSTREAM0(),
        Y_IOS_ORSTREAM()
        {
            if(append) unwind();
        }

        orstream:: orstream( const char *filename, bool append ) :
        Y_IOS_ORSTREAM0(),
        Y_IOS_ORSTREAM()
        {
            if(append) unwind();
        }

        orstream:: orstream( ios::cstdout_t &_) :
        local_file(_),
        Y_IOS_ORSTREAM()
        {}
        orstream:: orstream( ios::cstderr_t &_) :
        local_file(_),
        Y_IOS_ORSTREAM()
        {}



        void orstream:: write(char C)
        {
            assert(curr<last);
            *(curr++) = C;
            if(curr>last)
            {
                flush();
            }
#if 0
            size_t done = 0;
            descriptor::put(handle,&C,1,done);
            if(done!=1) throw exception("orstream: couldn't write 1 char");
#endif
        }

        void orstream:: flush()
        {
            
        }

    }
}
