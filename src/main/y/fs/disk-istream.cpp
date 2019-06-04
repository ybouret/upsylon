#include "y/fs/disk-istream.hpp"
#include "y/exception.hpp"

namespace upsylon
{
    namespace ios
    {

        disk_istream :: disk_istream(const readable_disk_file &src,
                                     const shared_disk_buffer &sdb ) :
        istream(),
        rdf( src ),
        pos( rdf->tell() ),
        buf( sdb ),
        curr( buf->entry ),
        next( curr ),
        last( curr + buf->bytes )
        {
            assert( !available() );
        }

        disk_istream:: ~disk_istream() throw()
        {
        }

        size_t disk_istream:: available() const throw()
        {
            assert(next>=curr);
            return static_cast<size_t>(next-curr);
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
                const size_t n = available();
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

        void disk_istream:: defrag() throw()
        {
            const size_t n = available();
            memmove(buf->entry, curr, n);
            curr = buf->entry;
            next = curr+n;
            assert(available()==n);
        }

        bool disk_istream:: query(char &C)
        {
            if(!available() && !load() )
            {
                return false;
            }
            else
            {
                assert( available() > 0 );
                C = *(curr++);
                return true;
            }
        }

    }
}

