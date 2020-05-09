
//! \file

#ifndef Y_MPIXX_INCLUDED
#define Y_MPIXX_INCLUDED 1

#include "y/exception.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/object.hpp"
#include <cstdio>

//! avoid C++ from OpenMPI
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>


namespace upsylon
{
    //! check a MPI function retuns MPI_SUCCESS
#define Y_MPI_CHECK(CALL) do { const int err = (CALL); if(MPI_SUCCESS!=err) throw mpi::exception(err,#CALL); } while(false)
    
    //! MPI wrapping
    class mpi : public singleton<mpi>
    {
    public:
        //______________________________________________________________________
        //
        //
        // types and definitions
        //
        //______________________________________________________________________
        
        static const int io_tag = 7; //!< default channel
        
        //______________________________________________________________________
        //
        //! dedicated error handling
        //______________________________________________________________________
        class exception : public upsylon::exception
        {
        public:
            explicit exception( int err, const char *fmt,... ) throw(); //!< initialize
            exception( const exception & ) throw();                     //!< copy
            virtual ~exception() throw();                               //!< destructor
            virtual const char *what() const throw();                   //!< Uses MPI_Error_string(...) to format the error code.
            const int code;                                             //!< MPI error
        private:
            Y_DISABLE_ASSIGN(exception);
            static const size_t elen = MPI_MAX_ERROR_STRING + 1;
            static const size_t alen = Y_MEMALIGN(elen);
            char string_[alen];
        };

        //______________________________________________________________________
        //
        //
        // static methods
        //
        //______________________________________________________________________
        
        //! true initialization function
        static mpi & init(int    *   argc,
                          char ***   argv,
                          const int  requestedThreadLevel,
                          const bool fromEnvironment);
       
        
        //______________________________________________________________________
        //
        //
        // methods
        //
        //______________________________________________________________________
        
        //! return human readable thread level
        const char *threadLevelText() const throw();

        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________
        const int threadLevel;
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(mpi);
        virtual ~mpi() throw();
        explicit mpi();
        friend class singleton<mpi>;
        void finalize() throw();
        
    public:
        //______________________________________________________________________
        //
        //
        // helpers
        //
        //______________________________________________________________________
        
        //! life-time of the singleton
        static const  at_exit::longevity life_time = memory::global::life_time - 4;
        
    };
    
    //! MPI_Init
    /**
     - SINGLE
     - FUNNELED
     - SERIALIZED
     - MULTIPLE
     */
#define Y_MPI(LEVEL) mpi & MPI = mpi::init( &argc, &argv, MPI_THREAD_##LEVEL, false)
#define Y_MPI_ENV()  mpi & MPI = mpi::init( &argc, &argv,-1,true)

}

#endif
