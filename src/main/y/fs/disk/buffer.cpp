#include "y/fs/disk/buffer.hpp"
#include "y/code/round.hpp"
#include "y/type/block/zset.hpp"
#include <cstring>

namespace upsylon
{
    namespace ios
    {

        disk_buffer_:: ~disk_buffer_() throw()
        {
            _bzset(bytes);
        }

        disk_buffer_:: disk_buffer_() throw() :
        entry(0),
        bytes(0),
        allocated(0)
        {
        }

        size_t disk_buffer_:: check(size_t n) throw()
        {
            if(n<=0) n=1;
            return Y_ROUND32(n);
        }

        void disk_buffer_:: clear() throw()
        {
            memset(entry,0,allocated);
        }


        disk_stream:: disk_stream( const disk_file &df, const shared_disk_buffer &sdb ) :
        pos( ((disk_file &)df)->tell() ),
        buf( sdb ),
        curr(buf->entry),
        next(curr),
        last(curr+buf->bytes)
        {
        }

        disk_stream:: ~disk_stream() throw()
        {
        }


        size_t disk_stream:: used() const throw()
        {
            return static_cast<size_t>(next-curr);
        }

        void disk_stream:: defrag() throw()
        {
            const size_t n = used();
            memmove(buf->entry, curr, n);
            curr = buf->entry;
            next = curr+n;
            assert(used()==n);
        }

    }

}

