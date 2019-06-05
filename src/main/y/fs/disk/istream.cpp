#include "y/fs/disk/istream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {

        disk_istream :: disk_istream(const readable_disk_file &src,
                                     const shared_disk_buffer &sdb ) :
        istream(),
        disk_stream( src, sdb ),
        rdf( src )
        {
        }

        disk_istream:: ~disk_istream() throw()
        {
        }



        bool disk_istream:: load()
        {
            curr = next = buf->entry;
            rdf->seek(pos,from_set);
            const size_t nr = rdf.get(curr,buf->bytes);
            pos = rdf->tell();
            if(nr>0)
            {
                next = curr+nr; assert(next<=last);
                return true;
            }
            else
            {
                return false;
            }
        }

        void disk_istream:: push1()
        {
            if(curr<=buf->entry)
            {
                if(next>=last)
                {
                    throw exception("disk_istream.store: full buffer");
                }
                const size_t n = used();
                memmove(curr+1,curr,n);
                ++curr;
                ++next;
            }
        }

        void disk_istream:: store(char C)
        {
            push1();
            *(--curr) = C;
        }

        
        bool disk_istream:: query(char &C)
        {
            if(!used() && !load() )
            {
                return false;
            }
            else
            {
                assert( used() > 0 );
                C = *(curr++);
                return true;
            }
        }

    }
}

