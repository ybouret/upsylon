//! \file

#ifndef Y_OXIDE_MPI_DOMAIN_INCLUDED
#define Y_OXIDE_MPI_DOMAIN_INCLUDED 1

#include "y/oxide/mpi/parallel.hpp"
#include "y/oxide/workspace.hpp"

namespace upsylon
{
    namespace Oxide
    {


        //======================================================================
        //
        //! forward declaration from Workspace
#define Y_DOMAIN_DECL(TYPE)  typedef typename     WorkspaceType::TYPE TYPE
        //
        //! forward definition from Workspace
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
            static const int            tag=3;          //!< session tag
            typedef Workspace<COORD>    WorkspaceType;  //!< alias
            Y_DOMAIN_DECL(LayoutType);                  //!< alias
            Y_DOMAIN_DECL(Loop);                        //!< alias
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
            mpi                          &MPI;       //!< keep the reference

            //==================================================================
            //
            // C++ setup
            //
            //==================================================================
            //! setup
            explicit Domain(mpi                   &_MPI,
                            const LayoutType      &fullLayout,
                            const_coord            localSizes,
                            const_coord            boundaries,
                            const size_t           ghostsZone) :
            WorkspaceType(fullLayout,localSizes,_MPI.rank,boundaries,ghostsZone),
            MPI(_MPI)
            {
                const int lsize = int(Coord::Product(this->sizes));
                if(  lsize != MPI.size )
                {
                    throw exception("Oxide::Domain(invalid |localSizes|=%d/MPI.size=%d)",lsize,MPI.size);
                }
            }

            //! cleanup
            virtual ~Domain() throw() {}

            //==================================================================
            //
            // communication
            //
            //==================================================================

            //------------------------------------------------------------------
            //
            //! full async exchange session
            //
            //------------------------------------------------------------------
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


        //======================================================================
        //
        //
        //! simplified way to get a functional domain
        //
        //
        //======================================================================
        template <typename COORD>
        class _Domain : public Parallel<COORD>, public Workspace<COORD>
        {
        public:
            typedef Workspace<COORD>     WorkspaceType; //!< alias
            Y_DOMAIN_DECL(LayoutType);                  //!< alias
            Y_DOMAIN_DECL(const_coord);                 //!< alias

            //! setup using optimal mapping
            inline explicit _Domain(mpi              &_MPI,
                                    const LayoutType &fullLayout,
                                    const_coord      &boundaries,
                                    const size_t      ghostsZone) :
            Parallel<COORD>(_MPI,fullLayout,boundaries),
            WorkspaceType(_MPI,fullLayout,this->optimal,boundaries,ghostsZone)
            {
            }

            //! cleanup
            inline virtual ~_Domain() throw() {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(_Domain);
        };



    }
}

#endif

