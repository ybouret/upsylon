#include "y/string/env.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/memory/pooled.hpp"
#include "y/exceptions.hpp"

#include <cerrno>
#include <cstring>

#if defined(Y_BSD)
#include <cstdlib>
extern char **environ;
#endif

#if defined(Y_WIN)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include "y/memory/buffers.hpp"
#endif


namespace upsylon {

    using namespace memory;

    namespace
    {

        class envmgr : public singleton<envmgr>
        {
        public:

            inline bool query( string &value, const string &name ) const
            {
                Y_LOCK(access);

#if defined(Y_BSD)
                const char *res = getenv( &name[0] );
                if( res ) {
                    value = res;
                    return true;
                } else {
                    value.clear();
                    return false;
                }
#endif

#if defined(Y_WIN)
                //std::cerr << "GetEnvVar '" << name << "'" << std::endl;
                value.clear();
                const DWORD res = ::GetEnvironmentVariable( &name[0], NULL, 0);
                const DWORD err = ::GetLastError();
                if( err == ERROR_ENVVAR_NOT_FOUND )
                {
                    //std::cerr << "Not Defined" << std::endl;
                    return false;
                }
                else
                {
                    //std::cerr << "[res1] = " << unsigned(res) << std::endl;
                    const size_t           len    = res-1;
                    const size_t           cnt    = res;
                    buffer_of<char,global> blk(cnt);
                    char                  *buf    = *blk;
                    const DWORD           res2    = ::GetEnvironmentVariable( &name[0], buf, res);
                    //std::cerr << "[res2] = " << unsigned(res2) << std::endl;
                    if(  res2 != len )
                    {
                        throw win32::exception( ::GetLastError(), "::GetEnvironmentVariable");
                    }
                    value.assign( buf, len );
                    //std::cerr << "='" << value << "'" << std::endl;
                    return true;
                }
#endif

            }


            inline void store( const string &name, const string &value ) const
            {
                Y_LOCK(access);
#if defined(Y_BSD)
                if( setenv( &name[0], &value[0], 1) < 0 ) {
                    throw libc::exception( errno, "setenv");
                }
#endif

#if defined(Y_WIN)
                Y_GIANT_LOCK();
                if( ! ::SetEnvironmentVariable( &name[0], &value[0] ) ) {
                    throw win32::exception( ::GetLastError(),  "::SetEnvironmentVariable");
                }
#endif
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(envmgr);
            friend class singleton<envmgr>;

            explicit envmgr() throw()
            {
            }

            virtual ~envmgr() throw()
            {

            }

            static const at_exit::longevity life_time = longevity_for::system_env;
        };

    }


    bool environment::get( string &value, const string &name )
    {
        return envmgr::instance().query( value, name );
    }

    bool environment::get( string &value, const char *name )
    {
        const string _(name);
        return get(value,_);
    }


    void environment::set( const string &name, const string &value)
    {
        envmgr::instance().store( name, value );
    }

    void environment::set( const char *name, const string &value)
    {
        const string _(name);
        set(_,value);
    }


    static void _parse_env( const char *str, const size_t len, void (*proc)( const string &, const string &, void *), void *args )
    {
        assert(str!=NULL);
        assert(len>0);
        size_t idx    = 1;
        bool   has_eq = false;
        while( idx < len ) {
            if( str[idx] == '=' ) {
                has_eq = true;
                break;
            }
            ++idx;
        }
        if( !has_eq )
            throw libc::exception( EINVAL, "invalid system environment string");

        string name( str, idx );
        ++idx;
        string value( str+idx, len-idx );
        proc( name, value, args );
    }


    void environment:: get( void (*proc)( const string &, const string &, void *), void *args )
    {

        Y_LOCK(envmgr::access);
        assert( proc );

#if defined(Y_BSD)
        char **env_reg = environ;
        while( *env_reg ) {
            const char *str = *env_reg;
            _parse_env( str, strlen(str), proc, args);
            ++env_reg;
        }
#endif


#if defined(Y_WIN)
        LPTCH es = 0;
        {
            Y_GIANT_LOCK();
            es = ::GetEnvironmentStrings();
            if( !es )
                throw win32::exception( ::GetLastError(), "::GetEnvironmentStrings()" );
        }

        try {

            const char *str = (const char *)es;
            while( *str ) {
                size_t len = std::strlen( str );
                _parse_env( str, len, proc, args );
                str += len;
                ++str;
            }

            Y_GIANT_LOCK();
            ::FreeEnvironmentStrings( es );
        }
        catch(...) {
            Y_GIANT_LOCK();
            ::FreeEnvironmentStrings( es );
            throw;
        }

#endif
    }

}

#include "y/string/convert.hpp"

namespace upsylon
{

    static inline bool is_blank( char C ) throw()
    {
        switch(C)
        {
            case ' ':
            case '\t':
                return true;

            default: break;
        }
        return true;
    }

    static inline bool env_get_clean( string &content, const string &name )
    {
        if( environment::get(content,name) )
        {
            content.clean(is_blank);
            return true;
        }
        else
        {
            return false;
        }

    }

    template <>
    bool environment:: check<bool>( bool &value, const string &name )
    {
        string content;
        if( env_get_clean(content,name) )
        {
            string_convert::to_lower(content);
            if( content == "false" || content == "0" )
            {
                value = false;
            }
            else if( content == "true" || content == "1" )
            {
                value = true;
            }
            else
            {
                throw exception("environment::check<bool>: invalid '%s'='%s' )", *name, *content);
            }

            return true;
        }
        else
        {
            return false;
        }
    }


#define Y_ENV_CHECK_FOR(TYPE) \
template <>\
bool environment:: check<TYPE>( TYPE &value, const string &name )\
{\
string content;\
if( env_get_clean(content,name) )\
{\
value = string_convert::to<TYPE>(content,"environment::check<" #TYPE ">");\
return true;\
}\
else\
{\
return false;\
}\
}
    
    Y_ENV_CHECK_FOR(unit_t)
    Y_ENV_CHECK_FOR(size_t)
    Y_ENV_CHECK_FOR(float)
    Y_ENV_CHECK_FOR(double)



}



