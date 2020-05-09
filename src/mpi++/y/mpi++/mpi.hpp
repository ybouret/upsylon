
//! \file

#ifndef Y_MPIXX_INCLUDED
#define Y_MPIXX_INCLUDED 1

#include "y/string.hpp"
#include "y/exception.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/associative/suffix-tree.hpp"

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
        //! wrapping MPI_Datatype
        //______________________________________________________________________
        class data_type
        {
        public:
            const MPI_Datatype uuid;
            const unsigned     size;
            
            data_type(const MPI_Datatype, const size_t) throw();
            data_type(const data_type &) throw();
            ~data_type() throw();
        private:
            Y_DISABLE_ASSIGN(data_type);
        };
        
        //______________________________________________________________________
        //
        //! tracing comms
        //______________________________________________________________________
        class comm_ticks
        {
        public:
            comm_ticks()  throw();
            ~comm_ticks() throw();
            uint64_t last;
            uint64_t full;
            void operator()(const uint64_t delta) throw(); //! full += (last=delta)
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_ticks);
        };
        
        //______________________________________________________________________
        //
        //! tracing calls
        //______________________________________________________________________
        class data_trace
        {
        public:
            data_trace()  throw();
            ~data_trace() throw();
            MPI_Datatype type; //!< last type
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_trace);
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
        // helper methods
        //
        //______________________________________________________________________
        
       
        const char *threadLevelText() const throw(); //!< return human readable thread level
        int         nextRank() const throw();        //!< next rank
        int         prevRank() const throw();        //!< prev rank
        
        //______________________________________________________________________
        //
        //
        // members
        //
        //______________________________________________________________________
        const int     size;          //!< MPI_COMM_WORLD size
        const int     rank;          //!< MPI_COMM_WORLD rank
        const int     last;          //!< size-1
        const bool    parallel;      //!< size>1
        const bool    head;          //!< 0==rank
        const bool    tail;          //!< last==rank
        const bool    bulk;          //!< !head && !tail
        comm_ticks    commTicks;     //!< tracking time
        const string  processorName; //!< the processor name
        const string  nodeName;      //!< size.rank
        const int     threadLevel;   //!< current thread level
        
        //______________________________________________________________________
        //
        //
        // MPI methods
        //
        //______________________________________________________________________
        
        //______________________________________________________________________
        //
        //
        // higher level methods
        //
        //______________________________________________________________________
        const data_type & data_type_for( const std::type_info & ) const;
        
        template <typename T> inline
        const data_type & data_type_for() const { return data_type_for( typeid(T) ); }
        
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(mpi);
        virtual ~mpi() throw();
        explicit mpi();
        friend class singleton<mpi>;
        void finalize() throw(); //!< MPI_Finalize()
        void build_data_types(); //!< build the database of primary types

        
        suffix_tree<data_type> types; //!< persistent database of types+sizes
        
        
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
