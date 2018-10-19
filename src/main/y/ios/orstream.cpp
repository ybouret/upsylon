
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




        orstream:: orstream( const string &filename, bool append ) :
        Y_IOS_ORSTREAM0()
        {
            if(append) unwind();
        }

        orstream:: orstream( const char *filename, bool append ) :
        Y_IOS_ORSTREAM0()
        {
            if(append) unwind();
        }

        orstream:: orstream( ios::cstdout_t &_) :
        local_file(_)
        {}

        orstream:: orstream( ios::cstderr_t &_) :
        local_file(_)
        {}



        void orstream:: write(char C)
        {
            size_t done = 0;
            descriptor::put(handle,&C,1,done);
            if(1!=done) throw exception("orstream::write(unable)");
        }

        void orstream:: flush()
        {

        }

    }
}
