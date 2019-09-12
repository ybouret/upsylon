//! \file

#ifndef Y_OXIDE_FIELD_MPI_INCLUDED
#define Y_OXIDE_FIELD_MPI_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/workspace.hpp"
#include "y/oxide/algo/optimal-mapping.hpp"

namespace upsylon
{
    namespace Oxide
    {

        template <typename COORD>
        class ParallelContext
        {
        public:
            typedef Layout<COORD> LayoutType;

            const LayoutType      full;     //!< full layout
            const vector<COORD>   mappings; //!< possible mappings
            const COORD           optimal;  //!< optimal mapping

            explicit ParallelContext(const mpi        &MPI,
                                     const LayoutType &userFull ) :
            full(userFull),
            mappings(),
            optimal( OptimalMapping::Find(full,MPI.size) )
            {
                full.buildMappings((vector<COORD> &)mappings,MPI.size);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParallelContext);
        };

#define Y_PARWKSP_DECL(TYPE)  typedef typename     WorkspaceType::TYPE TYPE
#define Y_PARWKSP_IMPL(VALUE) static  const size_t VALUE = WorkspaceType::VALUE

        template <typename COORD>
        class ParallelWorkspace : public Workspace<COORD>
        {
        public:
            static const size_t tag = 3;
            typedef Workspace<COORD>                     WorkspaceType;
            Y_PARWKSP_DECL(LayoutType);
            Y_PARWKSP_DECL(coord);
            Y_PARWKSP_DECL(const_coord);
            Y_PARWKSP_DECL(AsyncIO);
            Y_PARWKSP_IMPL(Dimensions);
            Y_PARWKSP_IMPL(Orientations);


            mpi &MPI;
            explicit ParallelWorkspace(mpi             &usrMPI,
                                       const LayoutType &full,
                                       const_coord       localSizes,
                                       const_coord      &PBC,
                                       const size_t      ng) :
            WorkspaceType(full,localSizes,usrMPI.rank,PBC,ng),
            MPI(usrMPI)
            {
                const int lsize = int(Layouts<COORD>::size);
                if(  lsize != MPI.size )
                {
                    throw exception("Oxide::ParalleWorkspace(invalid |localSizes|=%d/MPI.size=%d)",lsize,MPI.size);
                }
            }

            virtual ~ParallelWorkspace() throw()
            {

            }

            inline void asyncExchange(const ActiveFields  &fields)
            {
                for(size_t orientation=0;orientation<Orientations;++orientation)
                {
                    rings(fields,orientation);
                }
            }
            
            
           
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParallelWorkspace);
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


