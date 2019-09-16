//! \file

#ifndef Y_MPI_INCLUDED
#define Y_MPI_INCLUDED 1

#include "y/exception.hpp"
#include "y/type/spec.hpp"
#include "y/associative/set.hpp"
#include "y/associative/map.hpp"
#include "y/ios/upack.hpp"
#include "y/ios/ovstream.hpp"
#include "y/hashing/type-info.hpp"
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
        //! data type wrapper
        //______________________________________________________________________
        class data_type //: public type_spec
        {
        public:

            //------------------------------------------------------------------
            // types
            //------------------------------------------------------------------
            typedef hashing::type_info_hasher<>             db_hasher;
            typedef const MPI_Datatype                      value_type;  //!< value type
            typedef type_traits<value_type>::parameter_type param_type;  //!< for passing parameters
            typedef set<std::type_info,data_type,db_hasher> db;          //!< database

            //------------------------------------------------------------------
            // methods
            //------------------------------------------------------------------
            explicit data_type(const std::type_info &, const size_t, param_type); //!< initialize
            virtual ~data_type() throw();                                //!< destructor
            data_type(const data_type &other);                           //!< copy

            const std::type_info &label;                                 //!< system identifier
            const size_t          bytes;
            value_type            value;                                 //!< wrapped value

            const std::type_info & key() const throw();


        private:
            Y_DISABLE_ASSIGN(data_type);
        };

        //!
        class data_type_hasher
        {
        public:
            data_type_hasher() throw();
            ~data_type_hasher() throw();
            size_t operator()( const MPI_Datatype & ) throw();

        private:
            Y_DISABLE_COPY_AND_ASSIGN(data_type_hasher);
        };

        typedef map<MPI_Datatype,size_t,data_type_hasher> data_type_sizes;


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
        uint64_t         fullCommTicks; //!< cumulative communication ticks
        uint64_t         lastCommTicks; //!< ticks for last operation
        uint64_t         fullCommBytes; //!< cumulative communication bytes
        uint64_t         lastCommBytes; //!< bytes for last operation
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

        //______________________________________________________________________
        //
        // utilities
        //______________________________________________________________________

        //! next rank in one dimension
        template <typename T>
        static inline T getNext(T the_rank, const T the_size) throw()
        {
            return (++the_rank>=the_size?0:the_rank);
        }

        //! prev rank in one dimension
        template <typename T>
        static inline T getPrev(T the_rank, T the_size) throw()
        {
            return (the_rank<=0?--the_size:--the_rank);
        }


        
        //______________________________________________________________________
        //
        // point to point communication
        //______________________________________________________________________

        //! MPI_Send
        void Send(const void        *buffer,
                  const size_t       count,
                  const MPI_Datatype type,
                  const int          target,
                  const int          tag);


        //! Send integral type, to be specialized
        template <typename T> inline
        void Send( const T &x, const int target, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            Send( &x, 1, _, target, tag);
        }

        //! Send array of integral types, known size
        template <typename T> inline
        void SendAll( const array<T> &arr, const int target, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            const  size_t             n = arr.size();
            if(n>0)
            {
                Send(*arr,n,_,target,tag );
            }
        }

        //! MPI_Recv
        void Recv(void              *buffer,
                  const size_t       count,
                  const MPI_Datatype type,
                  const int          source,
                  const int          tag);

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
        void RecvAll( array<T> &arr, const int source, const int tag )
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            const size_t              n = arr.size();
            if(n>0)
            {
                Recv(*arr,n,_,source,tag);
            }
        }

        //! SendRecv
        void SendRecv(const void        *sendbuf,
                      const size_t       sendcount,
                      const MPI_Datatype sendtype,
                      const int          target,
                      const int          sendtag,
                      void              *recvbuf,
                      const size_t       recvcount,
                      const MPI_Datatype recvtype,
                      const int          source,
                      const int          recvtag);

        //! integral send/recv
        template <typename T,typename U>
        inline void SendRecv(const T &x, const int target, const int sendtag,
                             U       &y, const int source, const int recvtag)
        {
            static const MPI_Datatype _t = get_data_type_for<T>();
            static const MPI_Datatype _u = get_data_type_for<U>();
            Sendrecv(&x,1,_t,target, sendtag,
                     &y,1,_u,source, recvtag);
        }

        //! integral send/recv
        template <typename T>
        inline T  SendRecv(const T &x,
                           const int target, const int sendtag,
                           const int source, const int recvtag)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            T y(0);
            SendRecv(&x,1,_,target, sendtag,
                     &y,1,_,source, recvtag);
            return y;
        }
        
        //! arrays send/recv
        template <typename T, typename U>
        inline void SendRecvAll(const array<T> &x, const int target, const int sendtag,
                                array<U>       &y, const int source, const int recvtag)
        {
            static const MPI_Datatype _t = get_data_type_for<T>();
            static const MPI_Datatype _u = get_data_type_for<U>();
            SendRecv(*x, x.size(), _t, target, sendtag,
                     *y, y.size(), _u, source, recvtag);
        }

        //______________________________________________________________________
        //
        // collective
        //______________________________________________________________________

        //! generic wrapper
        void Bcast(void              *buffer,
                   const size_t       count,
                   const MPI_Datatype type,
                   const int          root);

        //! integral type wrapper
        template <typename T>
        inline void Bcast(T        &x,
                          const int root)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            Bcast(&x,1,_,root);
        }

        //! array of integral types
        template <typename T>
        inline void Bcast(T           *arr,
                          const size_t num,
                          const int    root)
        {
            static const MPI_Datatype _ = get_data_type_for<T>();
            Bcast(arr,num,_,root);
        }

        //! array of integral broadcast
        template <typename T>
        inline void BcastAll(array<T> &arr,
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
        void Reduce(const void  * send_data,
                    void        * recv_data,
                    const size_t  count,
                    MPI_Datatype datatype,
                    MPI_Op       op,
                    const int    root );
        
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
        void Allreduce(const void  * send_data,
                       void        * recv_data,
                       const size_t  count,
                       MPI_Datatype datatype,
                       MPI_Op       op);
        
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
        double getCommMilliseconds();
        
        //______________________________________________________________________
        //
        // I/O helpers
        //______________________________________________________________________

        //! in order print message
        void print( FILE *fp, const char *fmt,... ) Y_PRINTF_CHECK(3,4);

        //! print only on node0
        void print0( FILE *fp, const char *fmt,...) Y_PRINTF_CHECK(3,4);

        //! flush, effective only on node0
        void flush( FILE *fp );

        
        //! portable size send
        void   SendSize( const size_t value, const int target, const int tag);

        //! portable size receive
        size_t RecvSize( const int source, const int tag);

        //! portable SendRecv of sizes
        size_t SendRecvSizes(const size_t value,
                             const int target, const int sendtag,
                             const int source, const int recvtag);

        //______________________________________________________________________
        //
        // handling sequences
        //______________________________________________________________________

        //! send a range of iterated items
        template <typename ITERATOR> inline
        void SendRange( ITERATOR i, size_t n, const int target, const int tag)
        {
            SendSize(n,target,tag);
            while(n-->0)
            {
                Send(*i,target,tag);
                ++i;
            }
        }

        //! send a full sequence
        template <typename SEQUENCE> inline
        void SendSequence( const SEQUENCE &seq, const int target, const int tag)
        {
            typename SEQUENCE::const_iterator i = seq.begin();
            SendRange(i, seq.size(), target,tag);
        }

        //! recv a sequence, push_back
        template <typename SEQUENCE> inline
        void RecvSequence( SEQUENCE &seq, const int source, const int tag )
        {
            size_t n = RecvSize(source,tag);
            while(n-->0)
            {
                const typename SEQUENCE::const_type tmp = Recv<typename SEQUENCE::mutable_type>(source,tag);
                seq.push_back(tmp);
            }
        }

        //______________________________________________________________________
        //
        // handling variable sized sequences
        //______________________________________________________________________

        ios::upack_size send_pack; //!< used to send packed size of items
        ios::upack_size recv_pack; //!< used to recv packed size of items


        typedef array<uint8_t> vBytes; //!< variable number of bytes interface
        typedef ios::ovstream  vBlock; //!< variable block  implementation

        //! send variable number of bytes
        void vSend(const comm_mode mode, const vBytes &v, const int target, const int tag);

        //! recv variable number of bytes
        void vRecv(const comm_mode mode, vBlock       &v, const int source, const int tag);

        //! send/recv variable number of bytes
        void vSendRecv(const comm_mode mode,
                       const vBytes   &sendBytes, const int target, const int sendtag,
                       vBlock         &recvBytes, const int source, const int recvtag);


        //______________________________________________________________________
        //
        // point to point communication
        //______________________________________________________________________

        //! MPI_Barrier(MPI_COMM_WORLD)
        void Barrier();

        void displayTypes( FILE *fp ) const;

    private:
        data_type::db   types;
        data_type_sizes bytes;

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
        const size_t sz = s.size();
        SendSize(sz,target,tag);
        if(sz>0)
        {
            Send(*s,sz,MPI_CHAR,target,tag);
        }
    }

    //! recv a string, size+data
    template <> inline
    string mpi:: Recv<string>( const int source, const int tag )
    {
        const size_t sz = RecvSize(source,tag);
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

