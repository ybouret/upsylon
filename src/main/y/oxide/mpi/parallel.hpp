
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
            typedef typename LayoutType::coord              coord;
            typedef typename LayoutType::const_coord        const_coord;
            typedef typename Coord::Get<coord>::BooleanType bool_type;   //!< boolean vector
            typedef const bool_type                         const_bool;  //!< const boolean vector
            typedef vector<COORD> MappingsType;     //!< will store mappings

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const MappingsType mappings;            //!< all possible mappings
            const_coord        optimal;             //!< optimal mappings
            const_coord        favorite;            //!< from preferred

            //! create possible mappings and pick optimal
            inline explicit Parallel(const mpi        &MPI,
                                     const LayoutType &fullLayout,
                                     const_coord       boundaries ) :
            mappings(),
            optimal( Divide::Find(fullLayout,MPI.size,boundaries, (MappingsType *)&mappings  ) ),
            favorite(optimal)
            {
                if( Coord::Product(optimal) <= 0 ) throw exception("No available mapping for MPI.size=%d", MPI.size );
                //const_bool choice = Coord::Get<coord>::ToBoolean(preferred);

            }

            //! cleanup
            inline virtual ~Parallel() throw() { bzset_(optimal); }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parallel);
        };

    }

}

#endif

