
#include "y/ios/irstream.hpp"

namespace upsylon
{
    namespace ios
    {

        irstream:: ~irstream() throw() {}

        irstream:: irstream(const string &filename,const offset_t shift) :
        local_file(filename,readable)
        {
            (void)seek(shift,from_set);
        }


        irstream:: irstream(const char *filename,const offset_t shift) :
        local_file(filename,readable)
        {
            (void)seek(shift,from_set);
        }

        bool irstream:: query(char &C)
        {
            size_t done = 0;
            descriptor::get( handle, &C, 1, done);
            return 1 == done;
        }

        void irstream:: store(char)
        {
            seek(-1,from_cur);
        }



    }
}
