//! \file
#ifndef Y_OXIDE_MPI_REALM_INCLUDED
#define Y_OXIDE_MPI_REALM_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/workspace.hpp"
#include "y/oxide/partition.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //======================================================================
        //
        //! forward declaration from Workspace
#define Y_REALM_DECL(TYPE)  typedef typename     WorkspaceType::TYPE TYPE
        //
        //! forward definition from Workspace
#define Y_REALM_IMPL(VALUE) static  const size_t VALUE = WorkspaceType::VALUE
        //
        //! build a MPI Realm, located at the root node
        /**
         may be used to gather/scatter data using computed partions
         */
        //
        //======================================================================

        template <typename COORD>
        class Realm : public Workspace<COORD>
        {
        public:
            //==================================================================
            //
            // types and definitions
            //
            //==================================================================
            static const int            tag=4;         //!< session tag
            typedef Workspace<COORD>    WorkspaceType; //!< alias
            Y_REALM_DECL(LayoutType);                  //!< alias
            Y_REALM_DECL(Loop);                        //!< alias
            Y_REALM_DECL(coord);                       //!< alias
            Y_REALM_DECL(const_coord);                 //!< alias
            Y_REALM_DECL(AsyncIO);                     //!< alias
            Y_REALM_IMPL(Dimensions);                  //!< alias
            Y_REALM_IMPL(Orientations);                //!< alias
            typedef Partition<COORD>    PartitionType; //!< alias

            mpi                &MPI;         //!< reference to moi singleton
            const PartitionType partition;   //!< the partition(s)

            //! cleanup
            inline virtual ~Realm() throw() {}

            //! create a realm
            explicit Realm(mpi                   & _MPI,
                           const LayoutType      &fullLayout,
                           const_coord            localSizes,
                           const_coord            boundaries,
                           const size_t           ghostsZone) :
            WorkspaceType(fullLayout,Coord::Ones<COORD>(),0,boundaries,ghostsZone),
            MPI(_MPI),
            partition(fullLayout,localSizes,MPI.size)
            {
            }


            //------------------------------------------------------------------
            //
            // send a parent workspace to some local workspaces
            //
            //------------------------------------------------------------------

            //! send from node 0 to other
            static inline void Scatter(mpi           &MPI,
                                       const Realm  *parentAddr,
                                       const string  &id,
                                       WorkspaceType &child )
            {

                Field            &target = child[id];
                if(parentAddr)
                {
                    const Realm         &parent = *parentAddr;
                    const PartitionType &part   =  parent.partition;
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
                                      Realm              *parentAddr,
                                      const string        &id,
                                      const WorkspaceType &child )
            {

                const Field &target = child[id];
                if(parentAddr)
                {
                    Realm               &parent = *parentAddr;
                    const PartitionType &part   = parent.partition;
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
            Y_DISABLE_COPY_AND_ASSIGN(Realm);
        };

    }

}
#endif
