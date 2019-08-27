//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/exception.hpp"
#include "y/os/rt-clock.hpp"
#include "y/type/spec.hpp"
#include "y/associative/set.hpp"
#include "y/sequence/array.hpp"
#include "y/type/ints-chkbnd.hpp"

#include <cstdio>

//! avoir C++ from OpenMPI
#define OMPI_SKIP_MPICXX 1
#include <mpi.h>


namespace upsylon
{

    //! check a MPI function retuns MPI_SUCCESS
#define Y_MPI_CHECK(CALL) do { const int err = CALL; if(MPI_SUCCESS!=err) throw mpi::exception(err,#CALL); } while(false)

    //! MPI wrapping
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
        //! data type wrapper
        //______________________________________________________________________
        class data_type : public type_spec
        {
        public:
            //------------------------------------------------------------------
            // types
            //------------------------------------------------------------------
            typedef const MPI_Datatype                      value_type;  //!< value type
            typedef type_traits<value_type>::parameter_type param_type;  //!< for passing parameters
            typedef set<string,data_type>                   db;          //!< database

            //------------------------------------------------------------------
            // methods
            //------------------------------------------------------------------
            explicit data_type( const std::type_info &t, param_type v ); //!< initialize
            virtual ~data_type() throw();                                //!< desctructor
            data_type(const data_type &other);                           //!< copy

            value_type value;                                            //!< wrapped value

        private:
            Y_DISABLE_ASSIGN(data_type);
        };

        //______________________________________________________________________
        //
        // data and initialization
        //______________________________________________________________________
        const int        size;          //!< MPI_COMM_WOLRD size
        const int        rank;          //!< MPI_COMM_WORLD rank
        const int        last;          //!< rank-1
        const bool       parallel;      //!< size>1
        const bool       isHead;        //!< 0==rank
        const bool       isTail;        //!< last==rank
        const bool       isBulk;        //!< !(isHead) && !(isTail)
        const int        threadLevel;   //!< current thread level
        uint64_t         comTicks;      //!< cumulative communication ticks
        const string     processorName; //!< the processor name
        const string     nodeName;      //!< size.rank

        //! true initialization function
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

        //! internal default channel
        static const int io_tag = 7;

        //______________________________________________________________________
        //
        // point to point communication
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
            Y_MPI_CHECK(MPI_Send((void*)buffer, int(count), type, target, tag, MPI_COMM_WORLD) );
            comTicks += rt_clock::ticks() - mark;
        }


        //! Send integral type, to be specialized
        template <typename T> inline
        void Send( const T &x, const int target, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            Send( &x, 1, _, target, tag);
        }

        //! Send array of integral types
        template <typename T> inline
        void Send( const array<T> &arr, const int target, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            const  size_t             n = arr.size();
            if(n>0)
            {
                Send(*arr,n,_,target,tag );
            }
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
            MPI_Status     status;
            Y_MPI_CHECK(MPI_Recv(buffer, int(count), type, source, tag, MPI_COMM_WORLD, &status) );
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

        //! Recv array of integral types
        template <typename T> inline
        void Recv( array<T> &arr, const int source, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            const size_t              n = arr.size();
            if(n>0)
            {
                Recv(*arr,n,_,source,tag);
            }
        }

        //! Sendrecv
        inline void Sendrecv(const void        *sendbuf,
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

        //! integral send/recv
        template <typename T,typename U>
        inline void Sendrecv(const T &x, const int target, const int sendtag,
                             U       &y, const int source, const int recvtag)
        {
            static const MPI_Datatype _t = get_data_type_for<T>();
            static const MPI_Datatype _u = get_data_type_for<U>();
            Sendrecv(&x,1,_t,target, sendtag,
                     &y,1,_u,source, recvtag);
        }

        //! integral send/recv
        template <typename T>
        inline T  Sendrecv(const T &x,
                           const int target, const int sendtag,
                           const int source, const int recvtag)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            T y(0);
            Sendrecv(&x,1,_,target, sendtag,
                     &y,1,_,source, recvtag);
            return y;
        }
        
        //! arrays send/recv
        template <typename T, typename U>
        inline void SendRecv(const array<T> &x, const int target, const int sendtag,
                             array<U>       &y, const int source, const int recvtag)
        {
            static const MPI_Datatype _t = get_data_type_for<T>();
            static const MPI_Datatype _u = get_data_type_for<U>();
            Sendrecv(*x, x.size(), _t, target, sendtag,
                     *y, y.size(), _u, source, recvtag);
        }

        //______________________________________________________________________
        //
        // collective
        //______________________________________________________________________

        //! generic wrapper
        inline void Bcast(void              *buffer,
                          const size_t       count,
                          const MPI_Datatype type,
                          const int          root)
        {
            assert(!(0==buffer&&count>0));
            const uint64_t mark = rt_clock::ticks();
            Y_MPI_CHECK(MPI_Bcast(buffer,int(count),type,root,MPI_COMM_WORLD));
            comTicks += rt_clock::ticks() - mark;
        }

        //! integral type wrapper
        template <typename T>
        inline void Bcast(T        &x,
                          const int root)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            Bcast(&x,1,_,root);
        }


        //! array of integral broadcast
        template <typename T>
        inline void Bcast(array<T> &arr,
                          const int root)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            const size_t              n = arr.size();
            if(n>0)
            {
                Bcast(*arr,n,_,root);
            }
        }

        //! Reduce operation
        inline void Reduce(const void  * send_data,
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
        
        //! Reduce wrapper
        template <typename T>
        inline T Reduce(const T &x, MPI_Op op, const int root)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            T ans(0);
            Reduce( &x, &ans, 1, _, op, root);
            return ans;
        }
        
        //! Allreduce operation
        inline void Allreduce(const void  * send_data,
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
        
        //! Allreduce wrapper
        template <typename T>
        inline T Allreduce(const T &x, MPI_Op op)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            T ans(0);
            Allreduce( &x, &ans, 1, _, op);
            return ans;
        }
        
        //! helper to get commTime in ms
        double getCommMilliseconds()
        {
            rt_clock     clk;
            const double thisTime = clk( comTicks );
            const double maxiTime = Allreduce(thisTime,MPI_MAX);
            return maxiTime * 1000.0;
        }
        
        //______________________________________________________________________
        //
        // I/O helpers
        //______________________________________________________________________

        //! in order print message
        void print( FILE *fp, const char *fmt,... ) Y_PRINTF_CHECK(3,4);

        //! print only on node0
        void print0( FILE *fp, const char *fmt,...) Y_PRINTF_CHECK(3,4);

        //! check that the size fits in a 32 bit value (should...)
        static  uint32_t size_to_uint32( const size_t sz );

    private:
        data_type::db types;

        explicit mpi();
        virtual ~mpi() throw();
        friend class singleton<mpi>;


        void finalizeAndCleanUp() throw();
        bool write_to(FILE *fp, const string &text) const throw();

    public:
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
#define Y_MPI(LEVEL) mpi & MPI = mpi::init( &argc, &argv, MPI_THREAD_##LEVEL )




    //! send a string: size+data
    template <> inline
    void mpi:: Send<string>( const string &s, const int target, const int tag)
    {
        const uint32_t sz = size_to_uint32(s.size());
        Send(sz,target,tag);
        if(sz>0)
        {
            Send(*s,sz,MPI_CHAR,target,tag);
        }
    }

    //! recv a string, size+data
    template <> inline
    string mpi:: Recv<string>( const int source, const int tag )
    {
        const size_t sz = Recv<uint32_t>(source,tag);
        if(sz>0)
        {
            string ans(sz,as_capacity,true);
            Recv( *ans, sz, MPI_CHAR, source, tag );
            return ans;
        }
        else
        {
            return string();
        }
    }

}

#endif

