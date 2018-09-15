//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/string.hpp"
#include "y/exception.hpp"
#include "y/os/rt-clock.hpp"
#include <mpi.h>

namespace upsylon
{

#define Y_MPI_CHECK(CALL) do { const int err = CALL; if(MPI_SUCCESS!=err) throw mpi::exception(err,#CALL); } while(false)

    class mpi : public singleton<mpi>
    {
    public:
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

            virtual const char *what() const throw(); //!< Uses MPI_Error_string(...) to format the error code.

            const int code; //!< MPI error

        private:
            Y_DISABLE_ASSIGN(exception);
            static const size_t elen = MPI_MAX_ERROR_STRING + 1;
            static const size_t alen = Y_MEMALIGN(elen);
            char string_[alen];
        };

        //______________________________________________________________________
        //
        // data
        //______________________________________________________________________
        const int      size;
        const int      rank;
        const int      last;
        const bool     parallel;
        const bool     isHead;
        const bool     isTail;
        const bool     isBulk;
        const int      threadLevel;  //!< current thread level
        const uint64_t comTicks;     //!< cumulative communication ticks
        rt_clock       rtClock;      //!< internal clock
        static mpi & init( int * argc, char ***argv, int requestedThreadLevel );



        const char *threadLevelText() const throw();

    private:
        explicit mpi();
        virtual ~mpi() throw();
        friend class singleton<mpi>;
        void finalizeAndCleanUp() throw();

    public:
        static const  at_exit::longevity life_time = memory::pooled::life_time - 2;
    };

    //! MPI_Init
    /**
     - SINGLE
     - FUNNELED
     - SERIALIZED
     - MULTIPLE
     */
#define Y_MPI(LEVEL) mpi & MPI = mpi::init( &argc, &argv, MPI_THREAD_##LEVEL )

}

#endif

