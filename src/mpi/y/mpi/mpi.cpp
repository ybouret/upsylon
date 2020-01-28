#include "y/mpi/mpi.hpp"

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
        Y_MPI_CLEAN(fullCommTicks);
        Y_MPI_CLEAN(lastCommTicks);
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
    fullCommTicks(0),
    lastCommTicks(0),
    processorName(),
    nodeName(),
    send_pack(),
    recv_pack(),
    send_(),
    recv_(),
    types(32,as_capacity),
    bytes(),
    dtidx(1),
    send( send_ ),
    recv( recv_ )
    {
        //______________________________________________________________________
        //
        // System level init
        //______________________________________________________________________
        if(!__mpi_call) throw upsylon::exception("please call mpi.init(...)");
        Y_MPI_CHECK(MPI_Init_thread(__mpi_argc,__mpi_argv,__mpi_levl, (int *)&threadLevel));

        try
        {
            setup_global();
            setup_dtypes();
            setup_caches();
        }
        catch(...)
        {
            finalizeAndCleanUp();
            throw;
        }
    }

    void mpi:: displayTypes( FILE *fp ) const
    {
        if(0==rank)
        {
            {
                //data_type_hasher H;
                fprintf(fp,"<MPI::DataTypes count=\"%u\">\n", unsigned( types.size() ));
                for(data_type::db::iterator i=types.begin();i!=types.end();++i)
                {
                    const data_type    &t = *i;
                    fprintf(fp,"\t<%s>:\n", *(t.label->name()));
                }
                fprintf(fp,"<MPI::DataTypes>\n");
            }

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
        data_type::db   &db = types;
        const data_type *p  = db.search(t);
        if(!p) throw upsylon::exception("mpi: unregistered type <%s>", t.name() );
        return p->value;
    }

}





