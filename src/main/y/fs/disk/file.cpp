#include "y/fs/disk/file.hpp"


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

        on_disk:: on_disk( const char *filename, const unsigned mode) :
        local_file(filename,mode),
        access()
        {
        }

        on_disk:: on_disk( const cstdin_t &_) :
        local_file(_),
        access()
        {
        }

        on_disk:: on_disk( const cstdout_t &_) :
        local_file(_),
        access()
        {
        }

        on_disk:: on_disk( const cstderr_t &_) :
        local_file(_),
        access()
        {
        }



    }
}




namespace upsylon
{
    namespace ios
    {
        disk_file:: ~disk_file() throw()
        {
        }

        disk_file:: disk_file( const disk_file &other ) throw() :
        on_disk::pointer(other)
        {}

        disk_file:: disk_file( const string &filename, const unsigned mode ) :
        on_disk::pointer( new on_disk(filename,mode) )
        {
        }

        disk_file:: disk_file( const char *filename, const unsigned mode ) :
        on_disk::pointer( new on_disk(filename,mode) )
        {
        }

        disk_file:: disk_file(const cstdin_t &_) :
        on_disk::pointer( new on_disk(_) )
        {
        }

        disk_file:: disk_file(const cstdout_t &_) :
        on_disk::pointer( new on_disk(_) )
        {
        }

        disk_file:: disk_file(const cstderr_t &_) :
        on_disk::pointer( new on_disk(_) )
        {
        }



        descriptor::type & disk_file:: fd()
        {
            on_disk & self = (**this);
            return self.handle;
        }


    }
}

namespace upsylon
{
    namespace ios
    {
        rw_disk_file:: ~rw_disk_file() throw()
        {
        }

        rw_disk_file:: rw_disk_file( const string &filename ) :
        disk_file(filename,ios::readable|ios::writable)
        {
            
        }

        rw_disk_file:: rw_disk_file( const char *filename ) :
        disk_file(filename,ios::readable|ios::writable)
        {

        }

        rw_disk_file:: rw_disk_file( const rw_disk_file &other ) throw() :
        disk_file( other )
        {
        }

        size_t rw_disk_file:: get( void *data, const size_t size )
        {
            size_t done = 0;
            descriptor::get( fd(), data, size, done);
            return done;
        }

        size_t  rw_disk_file:: put(const void *data, const size_t size)
        {
            size_t done = 0;
            descriptor::put( fd(), data, size, done);
            return done;
        }
    }

}

namespace upsylon
{
    namespace ios
    {
        readable_disk_file:: ~readable_disk_file() throw()
        {}

        readable_disk_file:: readable_disk_file(const string &filename) :
        disk_file(filename,ios::readable)
        {
        }

        readable_disk_file:: readable_disk_file(const char *filename) :
        disk_file(filename,ios::readable)
        {
        }

        readable_disk_file:: readable_disk_file( const cstdin_t &_ ) :
        disk_file(_)
        {

        }

        readable_disk_file:: readable_disk_file( const readable_disk_file &other ) throw() :
        disk_file(other)
        {
        }

        readable_disk_file:: readable_disk_file(const rw_disk_file &other) throw() :
        disk_file( other )
        {
        }


        size_t readable_disk_file:: get( void *data, const size_t size )
        {
            size_t done = 0;
            descriptor::get( fd(), data, size, done);
            return done;
        }

    }
}

#include "y/exceptions.hpp"
#include <cerrno>

namespace upsylon
{
    namespace ios
    {
        writable_disk_file:: ~writable_disk_file() throw()
        {
        }

        writable_disk_file:: writable_disk_file(const string &filename, const bool append ) :
        disk_file(filename, ios::writable | ( append ? 0 : ios::truncate) )
        {
            if(append)
            {
                (*this)->unwind();
            }
        }

        writable_disk_file:: writable_disk_file(const char *filename, const bool append ) :
        disk_file(filename, ios::writable | ( append ? 0 : ios::truncate) )
        {
            if(append)
            {
                (*this)->unwind();
            }
        }

        writable_disk_file:: writable_disk_file( const cstderr_t &_ ) :
        disk_file(_)
        {
        }

        writable_disk_file:: writable_disk_file( const cstdout_t &_ ) :
        disk_file(_)
        {
        }

        size_t  writable_disk_file:: put(const void *data, const size_t size)
        {
            size_t done = 0;
            descriptor::put( fd(), data, size, done);
            return done;
        }

        void   writable_disk_file:: put_all(const void *data, const size_t size)
        {
            const uint8_t *buff = (const uint8_t *)data;
            size_t         todo = size;
            while(todo>0)
            {
                const size_t nw = put(buff,todo);
                if(nw<=0) throw libc::exception( EIO, "writable_disk_file.put_all()" );
                buff += nw;
                todo -= nw;
            }
        }

        writable_disk_file:: writable_disk_file(const writable_disk_file &other) throw() :
        disk_file(other)
        {
        }

        writable_disk_file:: writable_disk_file(const rw_disk_file &other) throw() :
        disk_file(other)
        {
        }


    }
}

#include "y/memory/cblock.hpp"

namespace upsylon
{
    namespace ios
    {
        void disk_file:: copy( const string &target, const string &source, bool append)
        {
            memory::cblock_of<char> blk( BUFSIZ );
            char                   *buf = blk.data;
            const size_t            len = blk.size;

            readable_disk_file src(source);
            writable_disk_file tgt(target,append);
            while(true)
            {
                const size_t nr = src.get(buf,len);
                if(nr<=0) break;
                tgt.put_all(buf,len);
            }
        }

    }

}

