
//! \file

#ifndef Y_MPIXX_INCLUDED
#define Y_MPIXX_INCLUDED 1

#include "y/string.hpp"
#include "y/exception.hpp"
#include "y/concurrent/singleton.hpp"
#include "y/associative/suffix/storage.hpp"
#include "y/ios/cfile.hpp"
#include "y/hashing/mph.hpp"
#include "y/sequence/vector.hpp"
#include "y/sequence/crates.hpp"
#include "y/type/rtti.hpp"

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
        static const int                 io_tag = 7; //!< default channel
        typedef hashing::minimal_perfect mphash;     //!< datatype->index

        //______________________________________________________________________
        //
        //! dedicated error handling
        //______________________________________________________________________
        class exception : public upsylon::exception
        {
        public:
            explicit exception(int err, const char *fmt,... ) throw(); //!< initialize
            exception(const exception &) throw();                      //!< copy
            virtual            ~exception()  throw();                  //!< destructor
            virtual const char *what() const throw();                  //!< Uses MPI_Error_string(...) to format the error code.
            const int           code;                                  //!< MPI error
        private:
            Y_DISABLE_ASSIGN(exception);
            static const size_t elen = MPI_MAX_ERROR_STRING + 1;
            static const size_t alen = Y_MEMORY_ALIGN(elen);
            char string_[alen];
        };

       

        //______________________________________________________________________
        //
        //! base type to trace full/last amount of operations
        //______________________________________________________________________
        class commTracer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            commTracer() throw();  //!< setup
            ~commTracer() throw();  //!< cleanup
            commTracer(const commTracer &) throw(); //!< copy
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void operator()(const uint64_t delta) throw(); //!< full += (last=delta)
            void reset()                          throw(); //!< last=full=0

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const uint64_t last; //!< last count
            const uint64_t full; //!< cumulative count

        private:
            Y_DISABLE_ASSIGN(commTracer);
        };

        //______________________________________________________________________
        //
        //! couple of tracers to follow send/recv
        //______________________________________________________________________
        class commFlux_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            commFlux_()                  throw(); //!< setup
            commFlux_(const commFlux_ &) throw(); //!< copy
            virtual ~commFlux_()         throw(); //!< cleanup
            void reset()                 throw(); //!< reset all

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            commTracer send; //!< send tracer
            commTracer recv; //!< recv tracer
        private:
            Y_DISABLE_ASSIGN(commFlux_);
        };

        //______________________________________________________________________
        //
        //! tracers and their run time type information
        //______________________________________________________________________
        class commFlux : public commFlux_
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit commFlux(const rtti &) throw(); //!< setup
            virtual ~commFlux() throw();             //!< cleanup
            commFlux(const commFlux &) throw();      //!< copy

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const rtti &info;                        //!< from MPI_Datatype
        private:
            Y_DISABLE_ASSIGN(commFlux);
        };


        
        //______________________________________________________________________
        //
        //! system_type to MPI_Datatype
        //______________________________________________________________________
        class system_type
        {
        public:
            typedef suffix_storage<system_type> store; //!< alias
            
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            system_type(const MPI_Datatype, commFlux &)  throw(); //!< setup
            system_type(const system_type &) throw();             //!< copy
            ~system_type() throw();                               //!< cleanup
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const MPI_Datatype type;  //!< associated type
            commFlux          &flux;  //!< for data I/O
            
        private:
            Y_DISABLE_ASSIGN(system_type);
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
        mutable commFlux_  commTicks;     //!< tracking time
        mutable commFlux_  commBytes;     //!< tracking time
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
        
        
        //! Send some data, commSend.bytes is updated
        template <typename T> inline
        void Send(const T       *buffer,
                  const size_t   count,
                  const int      dest) const
        {
            static const system_type   &st = systemTypeFor<T>();
            static const MPI_Datatype   dt = st.type;
            static commTracer          &cs = st.flux.send;

            Send(buffer,count,dt,dest,io_tag,MPI_COMM_WORLD);
            commBytes.send(count*sizeof(T));
            cs(count);
        }
        
        
        
        
        //! Send one datum
        template <typename T>
        void Send(const T       &args,
                  const int      dest) const
        {
            Send(&args,1,dest);
        }

        //! portable send size
        void SendSize(const size_t   args,
                      const int      dest) const;
        
        
        //______________________________________________________________________
        //
        // Recv
        //______________________________________________________________________
        
        //! MPI_Recv, commSend.ticks is updated
        void Recv(void              *buffer,
                  const size_t       count,
                  const MPI_Datatype datatype,
                  const int          source,
                  const int          tag,
                  const MPI_Comm     comm,
                  MPI_Status        &status) const;
       
        
        
        //! Recv some data, commSend.bytes is update
        template <typename T> inline
        void Recv(T             *buffer,
                  const size_t   count,
                  const int      source) const
        {
            static const system_type   &st = systemTypeFor<T>();
            static const MPI_Datatype   dt = st.type;
            static commTracer          &cr = st.flux.recv;

            MPI_Status   status;
            Recv(buffer,count,dt,source,io_tag,MPI_COMM_WORLD,status);
            commBytes.recv(count*sizeof(T));
            cr(count);
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
        
        //! MPI_Sendrecv, ticks are update
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
            static const system_type   &s_info = systemTypeFor<T>();
            static const MPI_Datatype   s_type = s_info.type;
            static commTracer          &s_flux = s_info.flux.send;

            static const system_type   &r_info = systemTypeFor<U>();
            static const MPI_Datatype   r_type = r_info.type;
            static commTracer          &r_flux = r_info.flux.recv;

            {
                MPI_Status   status;
                Sendrecv(sendbuf, sendcount, s_type, dest,   io_tag,
                         recvbuf, recvcount, r_type, source, io_tag, MPI_COMM_WORLD, status);
            }

            commBytes.send(sendcount*sizeof(T)); s_flux(sendcount);
            commBytes.recv(recvcount*sizeof(U)); r_flux(recvcount);

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
        //! MPI_Barrier(MPI_COMM_WORLD), update times
        void Barrier() const;
        
        //! MPI_Bcast, update timings
        void Bcast(void *buffer, const size_t count, MPI_Datatype datatype, const int root, MPI_Comm comm) const;
        
        //! generic Bcast
        template <typename T>
        void Bcast(T *buffer, const size_t count, const int root) const
        {
            static const system_type   &st = systemTypeFor<T>();
            static const MPI_Datatype   dt = st.type;

            Bcast(buffer,count,dt,root,MPI_COMM_WORLD);
            if(parallel)
            {
                static commTracer &cs = st.flux.send;
                static commTracer &cr = st.flux.recv;
                if(root==rank)
                {
                    const size_t multi = count * last;
                    commBytes.send(multi*sizeof(T));
                    cs(multi);
                }
                else
                {
                    commBytes.recv(count*sizeof(T));
                    cr(count);
                }
            }
        }
        
        //! one datum Bcast
        template <typename T>
        void Bcast( T &args, const int root) const
        {
            Bcast( &args, 1, root );
        }
        
        //! Bcast sizes
        void BcastSize(size_t &args, const int root) const;
        
        //! MPI_Reduce, update timings
        void Reduce(const void*        send_data,
                    void *             recv_data,
                    const size_t       count,
                    const MPI_Datatype datatype,
                    const MPI_Op       op,
                    const int          root,
                    const MPI_Comm     comm) const;
        
        //! generic reduction, update timings
        template <typename T> inline
        void Reduce(const T     *sendbuffer,
                    T           *recvbuffer,
                    const size_t count,
                    const MPI_Op op,
                    const int    root) const
        {
            static const system_type   &st = systemTypeFor<T>();
            static const MPI_Datatype   dt = st.type;
            Reduce(sendbuffer,recvbuffer,count,dt,op,root,MPI_COMM_WORLD);
            if(parallel)
            {
                static commTracer &cs = st.flux.send;
                static commTracer &cr = st.flux.recv;
                if(root==rank)
                {
                    const size_t multi = count * last;
                    commBytes.send(multi*sizeof(T));
                    cs(multi);
                }
                else
                {
                    commBytes.recv(count*sizeof(T));
                    cr(count);
                }
            }
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
            static const system_type   &st = systemTypeFor<T>();
            static const MPI_Datatype   dt = st.type;
            static commTracer          &cs = st.flux.send;
            static commTracer          &cr = st.flux.recv;
            Allreduce(sendbuffer,recvbuffer,count,dt,op,MPI_COMM_WORLD);
            const size_t items = count * last;
            const size_t bytes = items*sizeof(T);
            commBytes.send(bytes);
            commBytes.recv(bytes);
            cs(items);
            cr(items);
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
        //! convert type_info into a unique internal data_type
        const system_type & systemTypeFor( const std::type_info & ) const;
        
        //! wrapper to get a data_type
        template <typename T> inline
        const system_type & systemTypeFor() const { return systemTypeFor( typeid(T) ); }

        //! info only
        void displayTypes() const;

        //! print only on head node
        void Printf0(FILE *,const char *fmt,...) const Y_PRINTF_CHECK(3,4);
        
        //! print with node name
        void Printf(FILE  *,const char *fmt,...) const Y_PRINTF_CHECK(3,4);
        
        typedef void (*sequentialProc)(const mpi &, void *);     //!< sequential,low-level procdure
        void sequential(sequentialProc proc, void *args ) const; //!< ensure proc is executed in-order

        //! sequential code wrapper to call 'code(MPI);'
        template <typename FUNC> inline
        void sequential( FUNC &code ) const
        {
            sequential( callSequential<FUNC>, (void *) &code );
        }
        
        void recvACK(const int) const; //!< recv ACK from another
        void recvSYN()          const; //!< from rank=0
        void sendACK()          const; //!< to rank=0
        void sendSYN(const int) const; //!< send SYN to another

        void flush( const ios::cstdout_t & ) const throw(); //!< flush stdout and std::cout
        void flush( const ios::cstderr_t & ) const throw(); //!< flush stderr and std::cerr

        void   reset()  const throw(); //!< reset all comms
        string report() const;         //!< build I/O report

    private:
        Y_DISABLE_COPY_AND_ASSIGN(mpi);

        virtual ~mpi() throw();
        explicit mpi();
        void finalize() throw(); //!< MPI_Finalize()
        void build_types(); //!< build the database of primary types
        
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
        Y_SINGLETON_DECL_WITH(object::life_time-8,mpi); //!< setup
        unsigned index_of(const MPI_Datatype) const;    //!< get the index of the type from dataHash

        const mphash              dataHash; //!< MPI_Datatype => index
        const crates<commFlux>    ioFluxes; //!< I/O status per type
        const system_type::store  sysTypes; //!< system type  => MPI_Datatype

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

    //! inline head-only code
#define Y_MPI_HEAD(CODE) do { if(MPI.head) do { CODE; } while(false); } while(false)

    //! inline in-order all nodes code
#define Y_MPI_NODE(CODE) do {\
if(MPI.head) { do {CODE;} while(false); for(int r=1;r<MPI.size;++r) { MPI.sendSYN(r); MPI.recvACK(r); } }\
else         { MPI.recvSYN(); do { CODE; } while(false); MPI.sendACK(); } } while(false)

}

#endif
