//! \file
#ifndef Y_OXIDE_MPI_PARALLEL_INCLUDED
#define Y_OXIDE_MPI_PARALLEL_INCLUDED 1

#include "y/mpi/mpi.hpp"
#include "y/oxide/field/split.hpp"

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
        template <typename COORD> class Parallel : public Split<COORD>
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                           LayoutType;       //!< alias
            typedef typename LayoutType::coord              coord;            //!< alias
            typedef typename LayoutType::const_coord        const_coord;      //!< alias
            
            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------


            //! create possible mappings and pick optimal
            inline explicit Parallel(mpi              &MPI,
                                     const LayoutType &fullLayout,
                                     const_coord       boundaries,
                                     coord             preferred) :
            Split<COORD>( MPI.size, fullLayout, boundaries, preferred)
            {
            }

            //! cleanup
            inline virtual ~Parallel() throw()
            {

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parallel);
        };

    }

}

#endif

