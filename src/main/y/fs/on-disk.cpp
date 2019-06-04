#include "y/fs/on-disk.hpp"


namespace upsylon
{
    namespace ios
    {
        on_disk:: ~on_disk() throw()
        {
        }

        on_disk:: on_disk( const string &filename, const unsigned mode) :
        local_file(filename,mode),
        access()
        {
        }


    }
}


