//! \file

#ifndef Y_OXIDE_FIELD_MPI_INCLUDED
#define Y_OXIDE_FIELD_MPI_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/workspaces.hpp"
#include "y/oxide/field/divide.hpp"
#include "y/oxide/partition.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
        //======================================================================
        //
        //
        //! information about parallel layouts
        //
        //
        //======================================================================
        template <typename COORD> class Parallel
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD> LayoutType;       //!< alias
            typedef vector<COORD> MappingsType;     //!< will store mappings
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const MappingsType mappings;            //!< all possible mappings
            const COORD        optimal;             //!< optimal mappings
            
            //! create possible mappings and pick optimal
            inline explicit Parallel(const mpi        &MPI,
                                     const LayoutType &full,
                                     const COORD      &pbc,
                                     const bool        computeMappings=true) :
            mappings(), optimal( Divide::Find(full,MPI.size,pbc, (computeMappings) ? (MappingsType *)&mappings : 0 ) )
            {
                if( Coord::Product(optimal) <= 0 ) throw exception("No available mapping for MPI.size=%d", MPI.size );
            }
            
            //! cleanup
            inline virtual ~Parallel() throw() { bzset_(optimal); }
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parallel);
        };
        
    }
    
}

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
            typedef Partition<COORD>    PartitionType;  //!< alias
            
            //==================================================================
            //
            // members
            //
            //==================================================================
            mpi                          &MPI;       //!< keep the reference
            const auto_ptr<PartitionType> partition; //!< not allocated by default
            
            //==================================================================
            //
            // C++ setup
            //
            //==================================================================
            //! setup
            explicit Domain(mpi              &_MPI,
                            const LayoutType &fullLayout,
                            const_coord       localSizes,
                            const_coord       boundaries,
                            const size_t      ghostZone) :
            WorkspaceType(fullLayout,localSizes,_MPI.rank,boundaries,ghostZone),
            MPI(_MPI),
            partition(0)
            {
                const int lsize = int(Coord::Product(this->sizes));
                if(  lsize != MPI.size )
                {
                    throw exception("Oxide::Domain(invalid |localSizes|=%d/MPI.size=%d)",lsize,MPI.size);
                }
            }
            
            //! setup full domain, no boudaries, no ghosts
            explicit Domain(mpi              & _MPI,
                            const LayoutType &fullLayout,
                            const_coord       localSizes) :
            WorkspaceType(fullLayout,getOne(),0,getZero(),0),
            MPI(_MPI),
            partition( new PartitionType(fullLayout,localSizes,MPI.size) )
            {
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
            
            //------------------------------------------------------------------
            //
            // send a parent workspace to some local workspaces
            //
            //------------------------------------------------------------------
            
            //! send from node 0 to other
            static inline void Scatter(mpi           &MPI,
                                       const Domain *parentAddr,
                                       const string  &id,
                                       WorkspaceType &child )
            {
                
                Field            &target = child[id];
                if(parentAddr)
                {
                    const Domain        &parent = *parentAddr; assert(parent.partition.is_valid());
                    const PartitionType &part   =  *parent.partition;
                    assert(child.rank==0);
                    assert(part.contains(child.inner));
                    assert(part[0].is_same_than(child.inner));
                    
                    const Field &source = parent[id];
                    
                    // local scatter
                    source.localScatter<LayoutType>(child.inner,part,target,child.outer);
                    
                    // star-like scatter
                    const size_t psz = part.size();
                    IOBlock     &blk = parent.sendBlock;
                    for(size_t rank=1;rank<psz;++rank)
                    {
                        blk.free();
                        const size_t total = source.save<LayoutType>(blk,part,part[rank]);
                        __Workspace::CheckBlockTotal(blk,total);
                        MPI.vSend(comm_variable_size,blk,rank,tag);
                    }
                }
                else
                {
                    assert(child.rank>0);
                    IOBlock     &blk = child.recvBlock;
                    blk.free();
                    MPI.vRecv(comm_variable_size,blk,0,tag);
                    ios::imstream input(blk);
                    const size_t total = target.load<LayoutType>(input,child.outer,child.inner);
                    __Workspace::CheckBlockTotal(blk,total);
                }
            }
            
            //------------------------------------------------------------------
            //
            // send  some local workspaces to a global workspace
            //
            //------------------------------------------------------------------
            //! send from node 0 to other
            static inline void Gather(mpi                &MPI,
                                      Domain              *parentAddr,
                                      const string        &id,
                                      const WorkspaceType &child )
            {
                
                const Field &target = child[id];
                if(parentAddr)
                {
                    Domain              &parent = *parentAddr; assert(parent.partition.is_valid());
                    const PartitionType &part   =  *parent.partition;
                    assert(child.rank==0);
                    assert(part.contains(child.inner));
                    assert(part[0].is_same_than(child.inner));
                    
                    Field &source = parent[id];
                    
                    // local scatter
                    source.localGather<LayoutType>(child.inner,part,target,child.outer);
                    
                    // will send all data
                    const size_t psz  = part.size();
                    IOBlock     &blk = parent.recvBlock;
                    for(size_t rank=1;rank<psz;++rank)
                    {
                        blk.free();
                        MPI.vRecv(comm_variable_size,blk,rank,tag);
                        ios::imstream input(blk);
                        const size_t  total = source.load<LayoutType>(input,part,part[rank]);
                        __Workspace::CheckBlockTotal(blk,total);
                    }
                }
                else
                {
                    assert(child.rank>0);

                    IOBlock     &blk   = child.sendBlock; blk.free();
                    const size_t total = target.save<LayoutType>(blk,child.outer,child.inner);
                    __Workspace::CheckBlockTotal(blk,total);
                    MPI.vSend(comm_variable_size,blk,0,tag);
                }
                
            }
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Domain);
            
            static inline COORD getZero()   throw()
            {
                COORD ans(0); Coord::LD(ans,0);
                return ans;
            }
            
            static inline COORD getOne()   throw()
            {
                COORD ans(1); Coord::LD(ans,1);
                return ans;
            }
            
            
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
            inline explicit _Domain(mpi              &usrMPI,
                                    const LayoutType &full,
                                    const_coord      &PBC,
                                    const size_t      ng) :
            Parallel<COORD>(usrMPI,full,PBC,false),
            WorkspaceType(usrMPI,full,this->optimal,PBC,ng)
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


