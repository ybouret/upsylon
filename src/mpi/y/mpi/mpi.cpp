#include "y/mpi/mpi.hpp"

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




    namespace
    {
        int  *   __argc = 0;
        char *** __argv = 0;
        int      __levl = 0;
        bool     __call = false;

        static inline void __cleanup() throw()
        {
            __argc=0;
            __argv=0;
            __levl=0;
            __call=false;
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
        __argc = argc;
        __argv = argv;
        __levl = requestedThreadLevel;
        __call = true;
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
    rtClock()
    {
        if(!__call) throw upsylon::exception("please call mpi.init(...)");
        Y_MPI_CHECK(MPI_Init_thread(__argc,__argv,__levl, (int *)&threadLevel));
        try
        {
            Y_MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD,(int*)&size));
            Y_MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD,(int*)&rank));
            (int  &)last          = size-1;
            (bool &)parallel      = (size>1);
            (bool &)isHead        = (0==rank);
            (bool &)isTail        = (last==rank);
            (bool &)isBulk        = (!isHead) && (!isTail);
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


}

