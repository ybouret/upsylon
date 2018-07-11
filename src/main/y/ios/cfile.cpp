
#include "y/ios/cfile.hpp"
#include "y/exceptions.hpp"
#include <cerrno>
namespace upsylon
{
    namespace ios
    {
        cfile:: ~cfile() throw()
        {
            assert(fp);
            if(!persistent)
            {
                (void)fclose(fp);
            }
            fp = 0;
        }

        FILE * cfile:: operator*() throw()
        {
            assert(fp);
            return fp;
        }

        static inline
        const char * __cfile_flag( const cfile::open_mode m ) throw()
        {
            switch(m)
            {
                case cfile::open_read: return "rb";
                case cfile::open_write: return "wb";
                case cfile::open_append: return "ab";
            }
        }

        static inline
        FILE * __open_file( const string &filename, const cfile::open_mode m )
        {
            Y_GIANT_LOCK();
            const char *name = *filename;
            const char *flag = __cfile_flag(m);
            FILE *fp = fopen(name,flag);
            if(!fp)
            {
                throw libc::exception(errno,"fopen(%s,%s)",name,flag);
            }
            return fp;
        }

        cfile:: cfile(const string   &filename,
                      const open_mode m) :
        fp( __open_file(filename,m) ),
        persistent(false)
        {
        }

        cfile:: cfile(const char     *filename,
                      const open_mode m) :
        fp(0),
        persistent(false)
        {
            const string fn(filename);
            fp = __open_file(fn,m);
        }

        const cstdin_t  cstdin  = {};
        const cstdout_t cstdout = {};
        const cstderr_t cstderr = {};

        cfile:: cfile( const cstdin_t & ) :
        fp( stdin ),
        persistent(true)
        {
            if(!fp) throw exception("stdin is closed");
        }

        cfile:: cfile( const cstdout_t & ) :
        fp( stdout ),
        persistent(true)
        {
            if(!fp) throw exception("stdout is closed");
        }

        cfile:: cfile( const cstderr_t & ) :
        fp( stderr ),
        persistent(true)
        {
            if(!fp) throw exception("stderr is closed");
        }
    }

}

