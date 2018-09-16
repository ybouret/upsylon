//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/exception.hpp"
#include "y/os/rt-clock.hpp"
#include "y/type/spec.hpp"
#include "y/associative/set.hpp"

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
        //! data type wrapper
        //______________________________________________________________________
        class data_type : public type_spec
        {
        public:
            typedef const MPI_Datatype                      value_type;
            typedef type_traits<value_type>::parameter_type param_type;
            typedef set<string,data_type>                   db;

            value_type value;

            explicit data_type( const std::type_info &t, param_type v );
            virtual ~data_type() throw();
            data_type(const data_type &other);
            
        private:
            Y_DISABLE_ASSIGN(data_type);
        };

        //______________________________________________________________________
        //
        // data and initialization
        //______________________________________________________________________
        const int        size;
        const int        rank;
        const int        last;
        const bool       parallel;
        const bool       isHead;
        const bool       isTail;
        const bool       isBulk;
        const int        threadLevel;   //!< current thread level
        uint64_t         comTicks;      //!< cumulative communication ticks
        const string     processorName; //!< the processor name
        const string     nodeName;      //!< size.rank
        static mpi & init( int * argc, char ***argv, int requestedThreadLevel );


        //! return human readable thread level
        const char *threadLevelText() const throw();

        //! query database
        const MPI_Datatype & get_data_type( const std::type_info &t ) const;

        //! template wrapper
        template<typename T> inline
        const MPI_Datatype & get_data_type_for() const
        {
            return get_data_type( typeid(T) );
        }

        //______________________________________________________________________
        //
        // point to point
        //______________________________________________________________________

        //! MPI_Send
        inline void Send(const void        *buffer,
                         const size_t       count,
                         const MPI_Datatype type,
                         const int          target,
                         const int          tag)
        {
            assert(!(0==buffer&&count>0));
            const uint64_t mark = rt_clock::ticks();
            Y_MPI_CHECK(MPI_Send(buffer, count, type, target, tag, MPI_COMM_WORLD) );
            comTicks += rt_clock::ticks() - mark;
        }


        //! Send integral type, to be specialized
        template <typename T> inline
        void Send( const T &x, const int target, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            Send( &x, 1, _, target, tag);
        }

        //! MPI_Recv
        inline void Recv(void              *buffer,
                         const size_t       count,
                         const MPI_Datatype type,
                         const int          source,
                         const int          tag)
        {
            assert(!(0==buffer&&count>0));
            const uint64_t mark = rt_clock::ticks();
            MPI_Status status;
            Y_MPI_CHECK(MPI_Recv(buffer, count, type, source, tag, MPI_COMM_WORLD, &status) );
            comTicks += rt_clock::ticks() - mark;
        }

        //! Recv integral type, to be specialized
        template <typename T>
        T Recv( const int source, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            T x(0);
            Recv( &x, 1, _, source, tag);
            return x;
        }

        static const int io_tag = 7;
        
        //! in order print message
        void print( FILE *fp, const char *fmt,... ) Y_PRINTF_CHECK(3,4);

        //! print only on node0
        void print0( FILE *fp, const char *fmt,...) Y_PRINTF_CHECK(3,4);

    private:
        data_type::db types;

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


    template <> inline
    void mpi:: Send<string>( const string &s, const int target, const int tag)
    {
        const size_t sz = s.size();
        Send(sz,target,tag);
        if(sz>0)
        {
            Send(*s,sz,MPI_CHAR,target,tag);
        }
    }

    template <> inline
    string mpi:: Recv<string>( const int source, const int tag )
    {
        const size_t sz = Recv<size_t>(source,tag);
        if(sz>0)
        {
            string ans(sz,as_capacity);
            Recv( *ans, sz, MPI_CHAR, source, tag );
            ans.force(sz);
            return ans;
        }
        else
        {
            return string();
        }
    }

}

#endif

