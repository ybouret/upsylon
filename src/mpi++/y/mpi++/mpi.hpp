
//! \file

#ifndef Y_MPIXX_INCLUDED
#define Y_MPIXX_INCLUDED 1

#include "y/string.hpp"
#include "y/exception.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/associative/suffix-tree.hpp"

#include <cstdio>
#include <typeinfo>

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
        //! wrapping MPI_Datatype to build a persistent database
        //______________________________________________________________________
        class data_type
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            data_type(const MPI_Datatype, const unsigned) throw(); //!< setup
            data_type(const data_type &) throw();                //!< copy
            ~data_type() throw();                                //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const MPI_Datatype uuid; //!< unique
            const unsigned     size; //!< associated size
            
        private:
            Y_DISABLE_ASSIGN(data_type);
        };
        
        //______________________________________________________________________
        //
        //! tracing
        //______________________________________________________________________
        class comm_tracer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit comm_tracer() throw(); //!< setup
            virtual ~comm_tracer() throw(); //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void operator()(const uint64_t delta) throw(); //!< full += (last=delta)
            void reset() throw(); //!< last=full=0
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            uint64_t last;         //!< last count
            uint64_t full;         //!< cumulative count
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_tracer);
        };
        
        //______________________________________________________________________
        //
        //! tracing comms
        //______________________________________________________________________
        class comm_ticks : public comm_tracer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit comm_ticks()  throw(); //!< setup
            virtual ~comm_ticks() throw();  //!< cleanup
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_ticks);
        };
        
        //______________________________________________________________________
        //
        //! tracing calls
        //______________________________________________________________________
        class comm_data : public comm_tracer
        {
        public:
            explicit comm_data()  throw();
            virtual ~comm_data() throw();
            MPI_Datatype type; //!< last type
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_data);
        };
        
        //! full comm info
        class comm_info
        {
        public:
            comm_info() throw();      //!< setup
            ~comm_info() throw();     //!< cleanup
            void reset_all() throw(); //!< reset all members
            comm_ticks ticks;         //!< ticks
            comm_data  data;          //!< data
        private:
            Y_DISABLE_COPY_AND_ASSIGN(comm_info);
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
        const int          size;          //!< MPI_COMM_WORLD size
        const int          rank;          //!< MPI_COMM_WORLD rank
        const int          last;          //!< size-1
        const bool         parallel;      //!< size>1
        const bool         head;          //!< 0==rank
        const bool         tail;          //!< last==rank
        const bool         bulk;          //!< !head && !tail
        mutable comm_info  commSend;      //!< tracking time
        mutable comm_info  commRecv;      //!< tracking time
        const string       processorName; //!< the processor name
        const string       nodeName;      //!< size.rank
        const int          threadLevel;   //!< current thread level
        
        //______________________________________________________________________
        //
        //
        // MPI methods: point to point
        //
        //______________________________________________________________________
        
        //______________________________________________________________________
        //
        // Send
        //______________________________________________________________________
        
        //! MPI_Send, update commSend.ticks only
        void Send(const void        *buffer,
                  const size_t       count,
                  const MPI_Datatype datatype,
                  const int          dest,
                  const int          tag,
                  const MPI_Comm     comm) const;
        
        
        //! Send some data, commSend.data is updated
        template <typename T> inline
        void Send(const T      *buffer,
                  const size_t   count,
                  const int      dest) const
        {
            static const data_type   & _        = data_type_for<T>();
            static const MPI_Datatype  datatype = _.uuid;
            static const size_t        datasize = _.size;
            Send(buffer,count,datatype,dest,io_tag,MPI_COMM_WORLD);
            commSend.data.type = datatype;
            commSend.data( datasize*count );
        }
        
        
        
        
        //! Send one datum
        template <typename T>
        void Send(const T       &args,
                  const int      dest) const
        {
            Send(&args,1,dest);
        }
        
        //! Send one datum
        
        
        //! portable send size
        void SendSize(const size_t   args,
                      const int      dest) const;
        
        
        //______________________________________________________________________
        //
        // Recv
        //______________________________________________________________________
        
        //! MPI_Recv
        void Recv(void              *buffer,
                  const size_t       count,
                  const MPI_Datatype datatype,
                  const int          source,
                  const int          tag,
                  const MPI_Comm     comm,
                  MPI_Status        &status) const;
       
        
        
        //! Recv some data
        template <typename T> inline
        void Recv(T             *buffer,
                  const size_t   count,
                  const int      source) const
        {
            static const data_type   & _        = data_type_for<T>();
            static const MPI_Datatype  datatype = _.uuid;
            static const size_t        datasize = _.size;
            MPI_Status status;
            Recv(buffer,count,datatype,source,io_tag,MPI_COMM_WORLD,status);
            commRecv.data.type = datatype;
            commRecv.data( datasize*count );
        }
        
        //! Recv one datum
        template <typename T>
        T Recv(const int source) const
        {
            T ans(0);
            Recv(&ans,1,source);
            return ans;
        }
       
        //! Recv a size, portable way
        size_t RecvSize(const int source) const;
        
        //______________________________________________________________________
        //
        // Sendrecv
        //______________________________________________________________________
        
        //! MPI_Sendrecv
        void Sendrecv(const void        *sendbuf,
                      const size_t       sendcount,
                      const MPI_Datatype sendtype,
                      const int          dest,
                      int                sendtag,
                      void              *recvbuf,
                      const size_t       recvcount,
                      const MPI_Datatype recvtype,
                      const int          source,
                      const int          recvtag,
                      const MPI_Comm     comm,
                      MPI_Status         &status) const;
      
        //! generic Sendrecv, with data count
        template <typename T,typename U> inline
        void Sendrecv(const T     *sendbuf,
                      const size_t sendcount,
                      const int    dest,
                      U           *recvbuf,
                      const size_t recvcount,
                      const int    source) const
        {
            static const data_type   & s_info = data_type_for<T>();
            static const MPI_Datatype  s_type = s_info.uuid;
            static const size_t        s_size = s_info.size;
            static const data_type   & r_info = data_type_for<U>();
            static const MPI_Datatype  r_type = r_info.uuid;
            static const size_t        r_size = r_info.size;
            MPI_Status status;
            Sendrecv(sendbuf, sendcount, s_type, dest, io_tag,
                     recvbuf, recvcount, r_type, source, io_tag, MPI_COMM_WORLD, status);
            commRecv.data.type = r_type; commRecv.data( r_size*recvcount );
            commSend.data.type = s_type; commSend.data( s_size*sendcount );
        }
        
        //! Sendrecv 1 args
        template <typename T, typename U> inline
        U Sendrecv(const T &args, const int dest, const int source) const
        {
            U temp(0);
            Sendrecv(&args,1,dest,&temp,1,source);
            return temp;
        }
        
        //! exchanging same kind of data
        template <typename T> inline
        void Exch(const T     *sendbuf,
                  const size_t sendcount,
                  const int    dest,
                  T           *recvbuf,
                  const size_t recvcount,
                  const int    source) const
        {
            Sendrecv<T,T>(sendbuf,sendcount,dest,recvbuf,recvcount,source);
        }
        
        //! exchande 1 args
        template <typename T> inline
        T Exch(const T &args, const int dest, const int source) const
        {
            return Sendrecv<T,T>(args,dest,source);
        }
        
        //! exchanging sizes
        size_t ExchSizes(const size_t args,const int dest,const int source) const;
        
        
        //______________________________________________________________________
        //
        //
        // MPI methods: collective
        //
        //______________________________________________________________________
        //! MPI_Barrier(MPI_COMM_WORLD)
        void Barrier() const;
        
        //! MPI_Bcast
        void Bcast(void *buffer, const size_t count, MPI_Datatype datatype, const int root, MPI_Comm comm) const;
        
        //! generic Bcast
        template <typename T>
        void Bcast(T *buffer, const size_t count, const int root) const
        {
            static const data_type   & _        = data_type_for<T>();
            static const MPI_Datatype  datatype = _.uuid;
            static const size_t        datasize = _.size;
            Bcast(buffer,count,datatype,root,MPI_COMM_WORLD);
            const uint64_t bytes = count * datasize;
            commRecv.data.type = datatype; commRecv.data( bytes );
            commSend.data.type = datatype; commSend.data( bytes );
        }
        
        //! one datum Bcast
        template <typename T>
        void Bcast( T &args, const int root) const
        {
            Bcast( &args, 1, root );
        }
        
        //! Bcast sizes
        void BcastSize(size_t &args, const int root) const;
        
        //! MPI_Reduce
        void Reduce(const void*        send_data,
                    void *             recv_data,
                    const size_t       count,
                    const MPI_Datatype datatype,
                    const MPI_Op       op,
                    const int          root,
                    const MPI_Comm     comm) const;
        
        //! generic reduction
        template <typename T> inline
        void Reduce(const T     *sendbuffer,
                    T           *recvbuffer,
                    const size_t count,
                    const MPI_Op op,
                    const int    root) const
        {
            static const data_type   & _        = data_type_for<T>();
            static const MPI_Datatype  datatype = _.uuid;
            static const size_t        datasize = _.size;
            Reduce(sendbuffer,recvbuffer,count,datatype,op,root,MPI_COMM_WORLD);
            const uint64_t bytes = count * datasize;
            commRecv.data.type = datatype; commRecv.data( bytes );
            commSend.data.type = datatype; commSend.data( bytes );
        }
        
        //! generic 1 datum reduction
        template <typename T> inline
        T Reduce(const T &args,const MPI_Op op, const int root) const
        {
            T ans(0);
            Reduce(&args,&ans,1,op,root);
            return ans;
        }
        
        //! generic template reduction
        template < template <class> class VERTEX, typename T> inline
        VERTEX<T> Reduce2( const VERTEX<T> &vertex, const MPI_Op op, const int root) const
        {
            VERTEX<T> v;
            Reduce((const T *)&vertex,(T *)&v,sizeof(VERTEX<T>)/sizeof(T),op,root);
            return v;
        }
        
        //! reduction
        void Allreduce(const void*        send_data,
                       void *             recv_data,
                       const size_t       count,
                       const MPI_Datatype datatype,
                       const MPI_Op       op,
                       const MPI_Comm     comm) const;
        
        //! generic all-reduction
        template <typename T> inline
        void Allreduce(const T     *sendbuffer,
                       T           *recvbuffer,
                       const size_t count,
                       const MPI_Op op) const
        {
            static const data_type   & _        = data_type_for<T>();
            static const MPI_Datatype  datatype = _.uuid;
            static const size_t        datasize = _.size;
            Allreduce(sendbuffer,recvbuffer,count,datatype,op,MPI_COMM_WORLD);
            const uint64_t bytes = count * datasize;
            commRecv.data.type = datatype; commRecv.data( bytes );
            commSend.data.type = datatype; commSend.data( bytes );
        }
        
        //! generic 1 datum all-reduction
        template <typename T> inline
        T Allreduce(const T &args,const MPI_Op op) const
        {
            T ans(0);
            Allreduce(&args,&ans,1,op);
            return ans;
        }
        
        //! generic template reduction
        template < template <class> class VERTEX, typename T> inline
        VERTEX<T> Allreduce2( const VERTEX<T> &vertex, const MPI_Op op) const
        {
            VERTEX<T> v;
            Allreduce((const T *)&vertex,(T *)&v,sizeof(VERTEX<T>)/sizeof(T),op);
            return v;
        }
        
        
        //______________________________________________________________________
        //
        //
        // higher level methods
        //
        //______________________________________________________________________
        //! convert type_info into a unique internal dta_type
        const data_type & data_type_for( const std::type_info & ) const;
        
        //! wrapper to get a data_type
        template <typename T> inline
        const data_type & data_type_for() const { return data_type_for( typeid(T) ); }
        
        //! print only on head node
        void Printf0(FILE *,const char *fmt,...) const Y_PRINTF_CHECK(3,4);
        
        //! print with node name
        void Printf(FILE  *,const char *fmt,...) const Y_PRINTF_CHECK(3,4);
        
        typedef void (*sequentialProc)(const mpi &, void *);
        void sequential(sequentialProc proc, void *args ) const;
        
        template <typename FUNC> inline
        void sequential( FUNC &code ) const
        {
            sequential( callSequential<FUNC>, (void *) &code );
        }
        
        void recvACK(const int) const; //!< recv ACK from another
        void recvSYN()          const; //!< from rank=0
        void sendACK()          const; //!< to rank=0
        void sendSYN(const int) const; //!< sedn SYN to another
        
    private:
        Y_DISABLE_COPY_AND_ASSIGN(mpi);
        virtual ~mpi() throw();
        explicit mpi();
        friend class singleton<mpi>;
        void finalize() throw(); //!< MPI_Finalize()
        void build_data_types(); //!< build the database of primary types
      
        
        suffix_tree<data_type> types; //!< persistent database of types+sizes
        
        template <typename FUNC> static inline
        void callSequential(const mpi &self, void *args)
        {
            assert(args);
            FUNC &code = *(FUNC *)args;
            code(self);
        }
        
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
    
    //! MPI_Init using "MPI_THREAD_LEVEL" as env, default is single
#define Y_MPI_ENV()  mpi & MPI = mpi::init( &argc, &argv,-1,true)
    
    //! specialized string sending
    template <> void mpi::Send<string>(const string  &str,
                                        const int      dest) const;
    
    //! specialized string receiving
    template <> string mpi::Recv<string>(const int source) const;
 
    //! specialized string sendrecv
    template <> string mpi::Exch<string>(const string &str, const int dest, const int source) const;

    //! specialized string Bcast
    template <> void mpi::Bcast<string>(string &str, const int root) const;
    
#define Y_MPI_SEQUENTIAL(CODE) do {\
if(MPI.head) { do {CODE;} while(false); for(int r=1;r<MPI.size;++r) { MPI.sendSYN(r); MPI.recvACK(r); } }\
else         { MPI.recvSYN(); do { CODE; } while(false); MPI.sendACK(); } } while(false)

}

#endif
