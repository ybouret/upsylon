#include "y/mpi/mpi.hpp"
#include "y/type/bzset.hpp"
#include <cstdarg>


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


namespace upsylon
{
    mpi::data_type:: data_type(const type_spec &t,
                               const_type       v ) :
    label(t),
    value(v)
    {
    }

    mpi::data_type:: ~data_type() throw() {}

    mpi:: data_type:: data_type( const data_type &other ) :
    label(other.label),
    value(other.value)
    {}

    const type_mark & mpi::data_type:: key() const throw() { return label; }


}

namespace upsylon
{
    mpi:: data_type_cache:: data_type_cache() throw() :
    type(MPI_DATATYPE_NULL),
    size(0),
    last(0),
    full(0)
    {
    }

    mpi:: data_type_cache:: ~data_type_cache() throw()
    {
        type = MPI_DATATYPE_NULL;
        size = 0;
        last = 0;
        full = 0;
    }

    void mpi:: data_type_cache:: comm( const size_t count) throw()
    {
        assert(size>0);
        full += ( last = count*size );
    }

    void mpi:: data_type_cache:: like( const data_type_cache &other) throw()
    {
        type = other.type;
        size = other.size;
    }

    void mpi:: data_type_cache:: none() throw()
    {
        last = 0;
    }

    void mpi:: data_type_cache:: zero() throw()
    {
        last = full = 0;
    }


}

