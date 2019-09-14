//! \file

#ifndef Y_OXIDE_FIELD_MPI_INCLUDED
#define Y_OXIDE_FIELD_MPI_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/workspace.hpp"
#include "y/oxide/field/divide.hpp"

namespace upsylon
{
    namespace Oxide
    {

        template <typename COORD>
        class Parallel
        {
        public:
            typedef Layout<COORD> LayoutType;
            typedef vector<COORD> MappingsType;
            
            const MappingsType mappings;
            const COORD        optimal;
            
            inline explicit Parallel(const mpi        &MPI,
                                     const LayoutType &full,
                                     const COORD      &pbc) :
            mappings(),
            optimal( Divide::Find(full,MPI.size,pbc, (MappingsType *)&mappings) )
            {
                
            }
            
            inline virtual ~Parallel() throw()
            {
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parallel);
        };
        
        //======================================================================
        //
        //! forward declaration
#define Y_DOMAIN_DECL(TYPE)  typedef typename     WorkspaceType::TYPE TYPE
        //
        //! forward definition
#define Y_DOMAIN_IMPL(VALUE) static  const size_t VALUE = WorkspaceType::VALUE
        //
        //! build a MPI domain, which is a workspace with comms..
        //
        //======================================================================

        template <typename COORD>
        class Domain : public Workspace<COORD>
        {
        public:
            //==================================================================
            //
            // types and definitions
            //
            //==================================================================
            static const size_t         defaultTag = 3; //!< default mpi tag
            typedef Workspace<COORD>    WorkspaceType;  //!< alias
            Y_DOMAIN_DECL(LayoutType);                  //!< alias
            Y_DOMAIN_DECL(coord);                       //!< alias
            Y_DOMAIN_DECL(const_coord);                 //!< alias
            Y_DOMAIN_DECL(AsyncIO);                     //!< alias
            Y_DOMAIN_IMPL(Dimensions);                  //!< alias
            Y_DOMAIN_IMPL(Orientations);                //!< alias
            
            //==================================================================
            //
            // members
            //
            //==================================================================

            mpi &MPI;      //!< keep the reference
            const int tag; //!< session tag
            
            //==================================================================
            //
            // C++ setup
            //
            //==================================================================
            //! setup
            explicit Domain(mpi             &usrMPI,
                            const LayoutType &full,
                            const_coord       localSizes,
                            const_coord      &PBC,
                            const size_t      ng,
                            const int         sessionTag = defaultTag) :
            WorkspaceType(full,localSizes,usrMPI.rank,PBC,ng),
            MPI(usrMPI),
            tag(sessionTag)
            {
                const int lsize = int(Coord::Product(this->sizes));
                if(  lsize != MPI.size )
                {
                    throw exception("Oxide::ParalleWorkspace(invalid |localSizes|=%d/MPI.size=%d)",lsize,MPI.size);
                }
            }
            
            //! cleanup
            virtual ~Domain() throw() {}
        
            //==================================================================
            //
            // communication
            //
            //==================================================================
            
            //! full exchange session
            /**
             for each orientation, two waves are created
             */
            inline void asyncExchange(const ActiveFields  &fields)
            {
                for(size_t orientation=0;orientation<Orientations;++orientation)
                {
                    rings(fields,orientation);
                }
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Domain);
            inline void rings(const ActiveFields  &fields,
                              const size_t         orientation)
            {
                ring(Conn::Forward, fields,  orientation);
                ring(Conn::Reverse, fields,  orientation);
            }
            
            inline void ring(const Conn::Way      way,
                             const ActiveFields  &fields,
                             const size_t         orientation)
            {
                assert(orientation<Orientations);
                AsyncIO aio;
                this->asyncProlog(aio,fields,way,orientation);
                switch(aio.comm)
                {
                        //------------------------------------------------------
                        // send only
                        //------------------------------------------------------
                    case GhostsComm::Send:
                        assert(aio.send); assert(!aio.recv);
                        MPI.vSend(aio.mode,this->sendBlock,aio.send->rank,tag);
                        break;
                        
                        //------------------------------------------------------
                        // recv only
                        //------------------------------------------------------
                    case GhostsComm::Recv:
                        assert(aio.recv); assert(!aio.send);
                        MPI.vRecv(aio.mode,this->recvBlock,aio.recv->rank,tag);
                        break;
                        
                        //------------------------------------------------------
                        // sendrecv only
                        //------------------------------------------------------
                    case GhostsComm::Both:
                        assert(aio.recv); assert(aio.send);
                        MPI.vSendRecv(aio.mode,
                                      this->sendBlock, aio.send->rank, tag,
                                      this->recvBlock, aio.recv->rank, tag);
                        break;
                        
                    default:
                        break;
                }
                this->asyncEpilog(aio,fields);
            }
        };
        
    }
}

#endif


