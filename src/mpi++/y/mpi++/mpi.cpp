#include "y/mpi++/mpi.hpp"
#include "y/type/aliasing.hpp"
#include "y/type/block/zset.hpp"
#include "y/string/env.hpp"
#include "y/parops.hpp"

namespace upsylon {
    
    int mpi:: nextRank() const throw()
    {
        return parops::rank_next(size,rank);
    }
    
    int mpi:: prevRank() const throw()
    {
        return parops::rank_prev(size,rank);
    }

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
        finalize();
    }
    
    mpi & mpi:: init(int *      argc,
                     char ***   argv,
                     const int  requestedThreadLevel,
                     const bool fromEnvironment)
    {
        if( exists() )
        {
            throw upsylon::exception("mpi.init( already exist )");
        }
        __mpi_argc = argc;
        __mpi_argv = argv;
        __mpi_levl = requestedThreadLevel;
        if(fromEnvironment)
        {
            const string threadLevel;
            if( environment::get( aliasing::_(threadLevel), "MPI_THREAD_LEVEL") )
            {
#define Y_MPI_CHOOSE(LEVEL) do{ if( #LEVEL == threadLevel) { __mpi_levl = MPI_THREAD_##LEVEL; goto CALL; } } while(false)
                Y_MPI_CHOOSE(SINGLE);
                Y_MPI_CHOOSE(FUNNELED);
                Y_MPI_CHOOSE(SERIALIZED);
                Y_MPI_CHOOSE(MULTIPLE);
            }
            __mpi_levl = MPI_THREAD_SINGLE;            
        }
    CALL:
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
    
    void mpi:: finalize() throw()
    {
        (void)MPI_Finalize();
        _bzset(size);
        _bzset(rank);
        _bzset(threadLevel);
    }
    
    
    
    
    mpi:: mpi() :
    size(-1),
    rank(-1),
    last(-1),
    parallel(false),
    head(false),
    tail(false),
    bulk(0),
    commSend(),
    commRecv(),
    processorName(),
    nodeName(),
    threadLevel(-1),
    types()
    {
        //______________________________________________________________________
        //
        //
        // System level init
        //
        //______________________________________________________________________
        if(!__mpi_call) throw upsylon::exception("please call mpi.init(...)");
        Y_MPI_CHECK(MPI_Init_thread(__mpi_argc,__mpi_argv,__mpi_levl, & aliasing::_(threadLevel)) );
        
        //______________________________________________________________________
        //
        //
        // from now, finalize upon failure
        //
        //______________________________________________________________________
        try
        {
            
            //__________________________________________________________________
            //
            // size and rank
            //__________________________________________________________________
            Y_MPI_CHECK(MPI_Comm_size(MPI_COMM_WORLD,&aliasing::_(size) ));
            Y_MPI_CHECK(MPI_Comm_rank(MPI_COMM_WORLD,&aliasing::_(rank) ));
            aliasing::_(last)     = size-1;
            aliasing::_(parallel) = (size>1);
            aliasing::_(head)     = (0==rank);
            aliasing::_(tail)     = (last==rank);
            aliasing::_(bulk)     = (!head) && (!tail);
            
            //__________________________________________________________________
            //
            // processor/node name
            //__________________________________________________________________
            {
                char pname[MPI_MAX_PROCESSOR_NAME+1] = { 0 };
                int  psize=0;
                Y_MPI_CHECK(MPI_Get_processor_name(pname,&psize));
                string tmp(pname,psize);
                tmp.swap_with( aliasing::_(processorName) );
            }
            
            {
                int sz = 10; int tp = 1;
                while(size>=sz)
                {
                    ++tp;
                    sz *= 10;
                }
                const string fmt = vformat(".%%%dd.%%0%dd",tp,tp);
                string tmp = processorName + vformat(*fmt,size,rank);
                tmp.swap_with( aliasing::_(nodeName) );

            }
            
            //__________________________________________________________________
            //
            // data_types
            //__________________________________________________________________
            build_data_types();
            
            
            
        }
        catch(...)
        {
            finalize();
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

