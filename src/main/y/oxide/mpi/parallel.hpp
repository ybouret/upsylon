
//! \file

#ifndef Y_OXIDE_MPI_PARALLEL_INCLUDED
#define Y_OXIDE_MPI_PARALLEL_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/field/divide.hpp"

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
                                     const LayoutType &fullLayout,
                                     const COORD      &boundaries,
                                     const bool        computeMappings=true) :
            mappings(), optimal( Divide::Find(fullLayout,MPI.size,boundaries, (computeMappings) ? (MappingsType *)&mappings : 0 ) )
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

#endif

