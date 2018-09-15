#include "y/mpi/mpi.hpp"

namespace upsylon
{
    mpi:: ~mpi() throw()
    {
        MPI_Finalize();
    }

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

    mpi & mpi:: init( int * argc, char ***argv, int requestedThreadLevel )
    {
        if( exists() )
        {
            throw exception("mpi.init( already exist )");
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

    mpi:: mpi()
    {
        if(!__call) throw exception("please call mpi.init(...)");
    }

}

