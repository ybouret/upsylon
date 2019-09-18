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
            static const size_t                             Dimensions = Coord::Get<COORD>::Dimensions;
            typedef Layout<COORD>                           LayoutType;       //!< alias
            typedef typename LayoutType::coord              coord;            //!< alias
            typedef typename LayoutType::const_coord        const_coord;      //!< alias
            typedef vector<COORD>                           MappingsType;     //!< will store mappings

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const MappingsType mappings;            //!< all possible mappings
            const_coord        optimal;             //!< optimal mappings
            const_coord        favorite;            //!< from preferred

            //! create possible mappings and pick optimal
            inline explicit Parallel(mpi              &MPI,
                                     const LayoutType &fullLayout,
                                     const_coord       boundaries,
                                     coord             preferred) :
            mappings(),
            optimal( Divide::Find(fullLayout,MPI.size,boundaries, (MappingsType *)&mappings  ) ),
            favorite(optimal)
            {
                if( Coord::Product(optimal) <= 0 ) Divide::ErrorNoMappingFor(MPI.size);
                size_t    active=0;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    Coord1D &c = Coord::Of(preferred,dim);
                    if(c!=0)
                    {
                        c=1;
                        ++active;
                    }
                }
#if 0
                MPI.print0(stderr,"{%ld",Coord::Of(preferred,0));
                for(size_t dim=1;dim<Dimensions;++dim)
                {
                    MPI.print0(stderr," %ld",Coord::Of(preferred,dim));
                }
                MPI.print0(stderr,"}");
#endif
                if(active>0&&active<Dimensions)
                {
                    // user requested a preferred layout
                }
                else
                {
                    // keep the default (=optimal?) choice
                }

            }

            //! cleanup
            inline virtual ~Parallel() throw()
            {
                bzset_(optimal);
                bzset_(favorite);
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Parallel);
        };

    }

}

#endif

