#include "y/fs/disk-ostream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {
        disk_ostream:: ~disk_ostream() throw()
        {}

        disk_ostream:: disk_ostream(const writable_disk_file &src,
                                    const shared_disk_buffer &sdb ) :
        ostream(),
        disk_stream(src,sdb),
        wdf(src)
        {
        }

        void disk_ostream:: flush()
        {
            const size_t to_write = used();
            if(to_write)
            {
                wdf->seek(pos,from_set);
                const size_t written = wdf.put(curr, used() );
                pos  = wdf->tell();
                if(written<=0)
                {
                    throw exception("couldn't write disk_ostream");
                }
                curr += written;
                if(curr>=next)
                {
                    curr = next = buf->entry;
                }
                else
                {
                    defrag();
                }
            }
        }

        void disk_ostream:: write(char C)
        {
            if(next>=last)
            {
                flush();
            }

            *(next++) = C;
            

        }



    }

}
