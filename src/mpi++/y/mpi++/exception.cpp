
#include "y/mpi++/mpi.hpp"
#include "y/type/block/zset.hpp"

#include <cstdarg>
#include <cstring>

namespace upsylon
{
    mpi:: exception:: exception( int err, const char *fmt, ... ) throw() :
    upsylon::exception(),
    code( err ),
    string_()
    {
        va_list ap;
        va_start(ap,fmt);
        format(fmt,&ap);
        Y_BZSET_STATIC(string_);
        int errlen = 0;
        (void)MPI_Error_string(err,string_,&errlen);
    }
    
    mpi:: exception:: exception( const mpi::exception &e ) throw() :
    upsylon::exception(e),
    code( e.code ),
    string_()
    {
        memcpy( string_, e.string_, sizeof(string_) );
    }
    
    mpi:: exception:: ~exception() throw()
    {
        Y_BZSET_STATIC(string_);
    }
    
    const char * mpi:: exception:: what() const throw()
    {
        return string_;
    }
}


