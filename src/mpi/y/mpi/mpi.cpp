#include "y/mpi/mpi.hpp"
#include "y/type/bzset.hpp"

#include <cstdarg>
#include <cerrno>

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
    nodeName(),
    types(32,as_capacity),
    send_pack(),
    recv_pack()
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
#define Y_MPI_REG_CASE(PFX,SFX,COM_TYPE) case (sizeof(PFX##SFX)): __register< PFX##SFX >(types,COM_TYPE); break

#define Y_MPI_REG(PFX,STD_TYPE,COM_TYPE)            \
do { __register<STD_TYPE>(types,COM_TYPE);          \
if(sizeof(STD_TYPE)==usr_size) usr_type = COM_TYPE; \
switch(sizeof(STD_TYPE))\
{\
Y_MPI_REG_CASE(PFX,8_t, COM_TYPE);\
Y_MPI_REG_CASE(PFX,16_t,COM_TYPE);\
Y_MPI_REG_CASE(PFX,32_t,COM_TYPE);\
Y_MPI_REG_CASE(PFX,64_t,COM_TYPE);\
default: break;\
}\
} while(false)
            {
                static const size_t usr_size = sizeof(ptrdiff_t);
                MPI_Datatype        usr_type = MPI_DATATYPE_NULL;

                Y_MPI_REG(int,char,      MPI_CHAR);
                Y_MPI_REG(int,short,     MPI_SHORT);
                Y_MPI_REG(int,int,       MPI_INT);
                Y_MPI_REG(int,long,      MPI_LONG);
                Y_MPI_REG(int,long long, MPI_LONG_LONG);

                if(MPI_DATATYPE_NULL==usr_type) throw upsylon::exception("couldn't find mpi matching type for ptrdiff_t");
                __register<ptrdiff_t>(types,usr_type);
            }

            {
                static const size_t usr_size = sizeof(size_t);
                MPI_Datatype        usr_type = MPI_DATATYPE_NULL;

                Y_MPI_REG(uint,unsigned char,      MPI_UNSIGNED_CHAR);
                Y_MPI_REG(uint,unsigned short,     MPI_UNSIGNED_SHORT);
                Y_MPI_REG(uint,unsigned int,       MPI_UNSIGNED);
                Y_MPI_REG(uint,unsigned long,      MPI_UNSIGNED_LONG);
                Y_MPI_REG(uint,unsigned long long, MPI_UNSIGNED_LONG_LONG);

                if(MPI_DATATYPE_NULL==usr_type) throw upsylon::exception("couldn't find mpi matching type for size_t");
                __register<size_t>(types,usr_type);
            }

            __register<float >(types,MPI_FLOAT);
            __register<double>(types,MPI_DOUBLE);



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
                char  *buffer  = *databuf;
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

#include "y/os/rt-clock.hpp"

namespace upsylon
{
    double mpi:: getCommMilliseconds()
    {
        rt_clock     clk;
        const double thisTime = clk( comTicks );
        const double maxiTime = Allreduce(thisTime,MPI_MAX);
        return maxiTime * 1000.0;
    }

    void mpi::Send(const void        *buffer,
                   const size_t       count,
                   const MPI_Datatype type,
                   const int          target,
                   const int          tag)
    {
        assert(!(0==buffer&&count>0));
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Send((void*)buffer, int(count), type, target, tag, MPI_COMM_WORLD) );
        comTicks += rt_clock::ticks() - mark;
    }

    void mpi:: Recv(void              *buffer,
                    const size_t       count,
                    const MPI_Datatype type,
                    const int          source,
                    const int          tag)
    {
        assert(!(0==buffer&&count>0));
        const uint64_t mark = rt_clock::ticks();
        MPI_Status     status;
        Y_MPI_CHECK(MPI_Recv(buffer, int(count), type, source, tag, MPI_COMM_WORLD, &status) );
        comTicks += rt_clock::ticks() - mark;
    }

    void mpi:: Bcast(void              *buffer,
                     const size_t       count,
                     const MPI_Datatype type,
               const int          root)
    {
        assert(!(0==buffer&&count>0));
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Bcast(buffer,int(count),type,root,MPI_COMM_WORLD));
        comTicks += rt_clock::ticks() - mark;
    }

    void mpi:: Reduce(const void  * send_data,
                      void        * recv_data,
                      const size_t  count,
                      MPI_Datatype datatype,
                      MPI_Op       op,
                      const int    root )
    {
        assert(!(0==send_data&&count>0));
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Reduce(send_data, recv_data,count,datatype, op, root, MPI_COMM_WORLD));
        comTicks += rt_clock::ticks() - mark;
    }

    void mpi:: Allreduce(const void  * send_data,
                         void        * recv_data,
                         const size_t  count,
                         MPI_Datatype datatype,
                         MPI_Op       op)
    {
        assert(!(0==send_data&&count>0));
        const uint64_t mark = rt_clock::ticks();
        Y_MPI_CHECK(MPI_Allreduce(send_data, recv_data,count,datatype, op, MPI_COMM_WORLD));
        comTicks += rt_clock::ticks() - mark;
    }

    void mpi:: SendRecv(const void        *sendbuf,
                        const size_t       sendcount,
                        const MPI_Datatype sendtype,
                        const int          target,
                        const int          sendtag,
                        void              *recvbuf,
                        const size_t       recvcount,
                        const MPI_Datatype recvtype,
                        const int          source,
                        const int          recvtag)
    {
        const uint64_t mark = rt_clock::ticks();
        MPI_Status status;
        Y_MPI_CHECK(MPI_Sendrecv((void*)sendbuf, int(sendcount), sendtype, target, sendtag,
                                 recvbuf,        int(recvcount), recvtype, source, recvtag,
                                 MPI_COMM_WORLD, &status) );
        comTicks += rt_clock::ticks() - mark;
    }
    
}



namespace upsylon
{
    void mpi::SendSize(const size_t     value,
                       const int        target,
                       const int        tag)
    {
        send_pack(value);
        Send(send_pack.ro(), send_pack.requested, MPI_BYTE, target, tag);
    }

    size_t mpi:: RecvSize(const int        source,
                          const int        tag)
    {
        Recv(recv_pack.get().rw(),recv_pack.requested, MPI_BYTE, source, tag);
        return recv_pack();
    }

    size_t mpi:: SendRecvSizes(const size_t value,
                               const int target, const int sendtag,
                               const int source, const int recvtag)
    {
        send_pack(value);
        SendRecv(send_pack.ro(),       send_pack.requested, MPI_BYTE, target, sendtag,
                 recv_pack.get().rw(), recv_pack.requested, MPI_BYTE, source, recvtag);
        return recv_pack();
    }

}
