
#include "y/ios/cfile.hpp"
#include "y/exceptions.hpp"
#include "y/string.hpp"
#include "y/type/aliasing.hpp"
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
                case cfile::open_read :  return "rb";
                case cfile::open_write : return "wb";
                case cfile::open_append: return "ab";
            }
            return 0;
        }

        const char standard_input[]  = ":STDIN:";
        const char standard_error[]  = ":STDERR:";
        const char standard_output[] = ":STDOUT:";


#define Y_CFILE_GET(channel) \
static inline FILE * __get_##channel() {\
FILE *fp = channel; if(!fp) throw exception("cfile(%s is closed!)", #channel);\
return fp; }

        Y_CFILE_GET(stdin)
        Y_CFILE_GET(stderr)
        Y_CFILE_GET(stdout)


        static void throw_invalid_flag( const char *name, const char *flag )
        {
            throw exception("fopen(%s,INVALID %s)",name,flag);
        }

        static inline
        FILE * __open_file(const string          &filename,
                           const cfile::open_mode m,
                           const bool            &persistent)
        {
            Y_GIANT_LOCK();
            const char *name = *filename;
            const char *flag = __cfile_flag(m);

            if( standard_input == filename )
            {
                if( cfile::open_read != m ) throw_invalid_flag(name,flag);
                aliasing::_(persistent) = true;
                return __get_stdin();
            }

            if( standard_error == filename )
            {
                if( cfile::open_read == m ) throw_invalid_flag(name,flag);
                aliasing::_(persistent) = true;
                return __get_stderr();
            }

            if( standard_output == filename )
            {
                if( cfile::open_read == m ) throw_invalid_flag(name,flag);
                aliasing::_(persistent) = true;
                return __get_stdout();
            }

            // regular file
            FILE *fp = fopen(name,flag);
            if(!fp)
            {
                throw libc::exception(errno,"fopen(%s,%s)",name,flag);
            }
            return fp;
        }

        cfile:: cfile(const string   &filename,
                      const open_mode m) :
        persistent(false),
        fp( __open_file(filename,m,persistent) )
        {
        }

        cfile:: cfile(const char     *filename,
                      const open_mode m) :
        persistent(false),
        fp(0)
        {
            const string fn(filename);
            fp = __open_file(fn,m,persistent);
        }

        const cstdin_t  cstdin  = {};
        const cstdout_t cstdout = {};
        const cstderr_t cstderr = {};

        cfile:: cfile( const cstdin_t & ) :
        persistent(true),
        fp( __get_stdin() )
        {
        }

        cfile:: cfile( const cstdout_t & ) :
        persistent(true),
        fp( __get_stdout() )
        {
        }

        cfile:: cfile( const cstderr_t & ) :
        persistent(true),
        fp( __get_stderr() )
        {
        }
    }

}

