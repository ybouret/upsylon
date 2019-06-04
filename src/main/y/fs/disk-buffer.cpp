#include "y/fs/disk-buffer.hpp"

namespace upsylon
{
    namespace ios
    {

        disk_buffer_:: ~disk_buffer_() throw()
        {
            (size_t&)bytes = 0;
        }

        disk_buffer_:: disk_buffer_() throw() :
        entry(0),
        bytes(0),
        allocated(0)
        {
        }


    }

}

