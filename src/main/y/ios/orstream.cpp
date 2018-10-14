
#include "y/ios/orstream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        orstream:: ~orstream() throw()
        {
        }

#define Y_IOS_ORSTREAM() local_file(filename, writable | ( append ? 0 : truncate) )

        orstream:: orstream( const string &filename, bool append ) :
        Y_IOS_ORSTREAM()
        {
            if(append) unwind();
        }

        orstream:: orstream( const char *filename, bool append ) :
        Y_IOS_ORSTREAM()
        {
            if(append) unwind();
        }

        orstream:: orstream( ios::cstdout_t &_) : local_file(_) {}
        orstream:: orstream( ios::cstderr_t &_) : local_file(_) {}



        void orstream:: write(char C)
        {
            size_t done = 0;
            descriptor::put(handle,&C,1,done);
            if(done!=1) throw exception("orstream: couldn't write 1 char");
        }

        void orstream:: flush()
        {
        }

    }
}
