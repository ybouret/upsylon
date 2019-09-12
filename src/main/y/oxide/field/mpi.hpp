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

            const mpi            &MPI;
            const LayoutType      full;     //!< full layout
            const vector<COORD>   mappings; //!< possible mappings
            const COORD           optimal;  //!< optimal mapping

            explicit ParallelContext(const mpi        &userMPI,
                                     const LayoutType &userFull ) :
            MPI(userMPI),
            full(userFull),
            mappings(),
            optimal( OptimalMapping::Find(full,MPI.size) )
            {
                full.buildMappings((vector<COORD> &)mappings,MPI.size);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParallelContext);
        };

        template <typename COORD>
        class ParallelWorkspace : public Workspace<COORD>
        {
        public:
            typedef Workspace<COORD>                     WorkspaceType;
            typedef typename WorkspaceType::LayoutType   LayoutType;
            typedef typename WorkspaceType::coord        coord;
            typedef typename WorkspaceType::const_coord  const_coord;




        private:
            Y_DISABLE_COPY_AND_ASSIGN(ParallelWorkspace);
        };

    }
}

#endif


