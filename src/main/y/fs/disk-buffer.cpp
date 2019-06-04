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


        disk_stream:: disk_stream( const disk_file &df, const shared_disk_buffer &sdb ) :
        pos( ((disk_file &)df)->tell() ),
        buf( sdb )
        {
        }

        disk_stream:: ~disk_stream() throw()
        {
        }


    }

}

