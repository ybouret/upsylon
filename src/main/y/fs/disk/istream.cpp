#include "y/fs/disk/istream.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace ios
    {

        disk_istream :: disk_istream(const readable_disk_file &src,
                                     const shared_disk_buffer &sdb,
                                     const offset_t            len) :
        istream(),
        disk_stream( src, sdb ),
        rdf( src ),
        count(0),
        limit(len),
        bound(limit>=0)
        {
        }

        disk_istream:: ~disk_istream() throw()
        {
        }


        bool disk_istream:: update(const size_t nr)
        {
            if(nr>0)
            {
                // something was read!
                pos   =  rdf->tell();
                count += nr;
                next  =  curr+nr; assert(next<=last);
                return true;
            }
            else
            {
                // got nothing
                return false;
            }
        }

        bool disk_istream:: load()
        {
            //__________________________________________________________________
            //
            // initialize
            //__________________________________________________________________
            curr = next = buf->entry;
            if(bound)
            {
                //______________________________________________________________
                //
                // bound state
                //______________________________________________________________
                assert(count<=limit);
                if(count>=limit)
                {
                    return false;
                }
                else
                {
                    const size_t to_read = static_cast<size_t>(limit-count);
                    rdf->seek(pos,from_set);
                    return update(rdf.get(curr,min_of(to_read,buf->bytes)));
                }
            }
            else
            {
                //______________________________________________________________
                //
                // unbound state
                //______________________________________________________________
                rdf->seek(pos,from_set);
                return update(rdf.get(curr,buf->bytes));
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
            --count;
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

