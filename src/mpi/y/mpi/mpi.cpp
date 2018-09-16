#include "y/mpi/mpi.hpp"
#include "y/code/ilog2.hpp"

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
        memset( string_, 0, sizeof(string_) );
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
    }

    const char * mpi:: exception:: what() const throw()
    {
        return string_;
    }
}

namespace upsylon
{
    mpi::data_type:: data_type( const std::type_info &t, param_type v ) :
    type_spec(t),
    value(v)
    {
    }

    mpi::data_type:: ~data_type() throw() {}

    mpi:: data_type:: data_type( const data_type &other ) : type_spec(other), value(other.value) {}
    
}

namespace upsylon
{




    namespace
    {
        int  *   __mpi_argc = 0;
        char *** __mpi_argv = 0;
        int      __mpi_levl = 0;
        bool     __mpi_call = false;

        static inline void __cleanup() throw()
        {
            __mpi_argc=0;
            __mpi_argv=0;
            __mpi_levl=0;
            __mpi_call=false;
        }

        
    }

    mpi:: ~mpi() throw()
    {
        finalizeAndCleanUp();
    }

    mpi & mpi:: init( int * argc, char ***argv, int requestedThreadLevel )
    {
        if( exists() )
        {
            throw upsylon::exception("mpi.init( already exist )");
        }
        __mpi_argc = argc;
        __mpi_argv = argv;
        __mpi_levl = requestedThreadLevel;
        __mpi_call = true;
        try
        {
            mpi &MPI = instance();
            __cleanup();
            return MPI;
        }
        catch(...)
        {
            __cleanup();
            throw;
        }
    }

#define Y_MPI_CLEAN(FIELD) memset( (void *) &FIELD, 0, sizeof(FIELD) )

    void mpi:: finalizeAndCleanUp() throw()
    {
        (void)MPI_Finalize();
        Y_MPI_CLEAN(size);
        Y_MPI_CLEAN(rank);
        Y_MPI_CLEAN(last);
        Y_MPI_CLEAN(parallel);
        Y_MPI_CLEAN(isHead);
        Y_MPI_CLEAN(isTail);
        Y_MPI_CLEAN(isBulk);
        Y_MPI_CLEAN(threadLevel);
        Y_MPI_CLEAN(comTicks);
    }

    namespace
    {
        template <typename T>
        void __register( mpi::data_type::db &types, MPI_Datatype v )
        {
            const std::type_info & info = typeid(T);
            const mpi::data_type   dt(info,v);
            (void) types.insert(dt);
        }

    }
    
    mpi:: mpi() :
    size(0),
    rank(0),
    last(0),
    parallel(false),
    isHead(false),
    isTail(false),
    isBulk(false),
    threadLevel(-1),
    comTicks(0),
    processorName(),
    types(32,as_capacity)
    {
        //______________________________________________________________________
        //
        // System level init
        //______________________________________________________________________
        if(!__mpi_call) throw upsylon::exception("please call mpi.init(...)");
        Y_MPI_CHECK(MPI_Init_thread(__mpi_argc,__mpi_argv,__mpi_levl, (int *)&threadLevel));

        try
        {

            //__________________________________________________________________
            //
            // Get topology
            //__________________________________________________________________
            Y_MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD,(int*)&size));
            Y_MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD,(int*)&rank));
            (int  &)last          = size-1;
            (bool &)parallel      = (size>1);
            (bool &)isHead        = (0==rank);
            (bool &)isTail        = (last==rank);
            (bool &)isBulk        = (!isHead) && (!isTail);

            {
                char pname[MPI_MAX_PROCESSOR_NAME+1] = { 0 };
                int  psize=0;
                Y_MPI_CHECK(MPI_Get_processor_name(pname, &psize) );
                string tmp(pname,psize);
                tmp.swap_with( (string &)processorName );
            }

            {
                string tmp = vformat("<%d.%d> ",size,rank);
                tmp.swap_with( (string&)nodeName );
            }

            //__________________________________________________________________
            //
            // fill in database
            //__________________________________________________________________
            __register<char     >(types,MPI_CHAR);
            __register<short    >(types,MPI_SHORT);
            __register<int      >(types,MPI_INT);
            __register<long     >(types,MPI_LONG);
            __register<long long>(types,MPI_LONG_LONG);

            __register<unsigned char     >(types,MPI_UNSIGNED_CHAR);
            __register<unsigned short    >(types,MPI_UNSIGNED_SHORT);
            __register<unsigned int      >(types,MPI_UNSIGNED);
            __register<unsigned long     >(types,MPI_UNSIGNED_LONG);
            __register<unsigned long long>(types,MPI_UNSIGNED_LONG_LONG);

            __register<int8_t >(types,MPI_INT8_T);
            __register<int16_t>(types,MPI_INT16_T);
            __register<int32_t>(types,MPI_INT32_T);
            __register<int64_t>(types,MPI_INT64_T);

            __register<uint8_t >(types,MPI_UINT8_T);
            __register<uint16_t>(types,MPI_UINT16_T);
            __register<uint32_t>(types,MPI_UINT32_T);
            __register<uint64_t>(types,MPI_UINT64_T);

            __register<float >(types,MPI_FLOAT);
            __register<double>(types,MPI_DOUBLE);

            {
                const MPI_Datatype idt[4] = {MPI_INT8_T,MPI_INT16_T,MPI_INT32_T,MPI_INT64_T};
                __register<ptrdiff_t>(types,idt[ ilog2<sizeof(ptrdiff_t)>::value ]);
            }

            {
                const MPI_Datatype udt[4] = {MPI_UINT8_T,MPI_UINT16_T,MPI_UINT32_T,MPI_UINT64_T};
                __register<size_t>(types,udt[ ilog2<sizeof(ptrdiff_t)>::value ]);
            }
            
        }
        catch(...)
        {
            finalizeAndCleanUp();
            throw;
        }
    }

#define Y_MPI_THR(ID) case MPI_THREAD_##ID: return "MPI_THREAD_" #ID
    const char * mpi:: threadLevelText() const throw()
    {
        switch(threadLevel)
        {
                Y_MPI_THR(SINGLE);
                Y_MPI_THR(FUNNELED);
                Y_MPI_THR(SERIALIZED);
                Y_MPI_THR(MULTIPLE);

            default: break;
        }
        return "MPI_THREAD_UNKNOWN";
    }


    const MPI_Datatype & mpi:: get_data_type( const std::type_info &t ) const
    {
        const string     _(t.name());
        const data_type *p  = types.search(_);
        if(!p) throw upsylon::exception("mpi: unregistered type <%s>", *_);
        return p->value;
    }

}

#include "y/exceptions.hpp"
#include "y/memory/buffers.hpp"
#include <cstdarg>

namespace upsylon
{
    void mpi:: print0( FILE *fp, const char *format,...)
    {
        static const char fn[] = "mpi::print0: ";
        if(isHead)
        {
            assert(fp);
            va_list args;
            va_start(args, format);
            vfprintf(fp, format, args);
            va_end (args);
            if( ferror(fp) != 0 )
            {
                throw libc::exception( EIO, "%sformat='%s'", fn, format );
            }
        }
    }


    bool mpi:: write_to(FILE *fp, const string &text) const throw()
    {
        const size_t n = text.size();
        return n == fwrite( &text[0], 1, n, fp) ;
    }



    void mpi:: print( FILE *fp, const char *format, ... )
    {
        static const char fn[]  = "mpi::print: ";

        if(isHead)
        {
            assert(fp);
            assert(format);

            // write prolog for head
            if( !write_to(fp,nodeName)) throw libc::exception( EIO, "%swriting rank#0 name",fn);

            va_list args;
            va_start(args, format);
            vfprintf(fp, format, args);
            va_end (args);
            if( ferror(fp) != 0 )
            {
                throw libc::exception( EIO, "%sformat='%s'", fn, format );
            }

            // then receive strings
            for(int r=1;r<size;++r)
            {
                {
                    const string tmp = Recv<string>(r,io_tag);
                    if(!write_to(fp,tmp)) throw libc::exception( EIO, "%swriting rank#%d name", fn, r);
                }

                {
                    const string tmp = Recv<string>(r,io_tag);
                    if(!write_to(fp,tmp)) throw libc::exception( EIO, "%swriting content from rank#%d", fn, r);
                }
            }
        }
        else
        {
            size_t n = 64;
        TRY:
            {
                memory::buffer_of<char,memory::pooled> databuf( n );
                char  *buffer  = databuf();
                int    length  = int( databuf.length() );
                if( length < 0 )
                    throw libc::exception( ERANGE, "%s(...) memory overflow", fn);

                va_list ap;
                va_start( ap, format );
                const int  result  = vsnprintf( buffer, length, format, ap);
                const bool success = (result >= 0 && result < length); // WIN32 and GLIBC compatible
                va_end (ap);

                if( success )
                {
                    Send(nodeName,0,io_tag);
                    const string tmp(buffer,length_of(buffer));
                    Send(tmp,0,io_tag);
                    return;
                }
                n <<= 1;
            }
            goto TRY;
        }

    }


}

