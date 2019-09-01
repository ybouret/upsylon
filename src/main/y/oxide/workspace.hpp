//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon
{
    namespace Oxide
    {

        template <typename COORD>
        class Contact;
        
        
        
        template <typename COORD>
        class Workspace
        {
        public:
            typedef Layout<COORD>                    LayoutType;
            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;
            static  const size_t Dimensions = Coord::Get<COORD>::Dimensions;

            const_coord      sizes; //!< local sizes
            const size_t     size;  //!< product of sizes
            const size_t     rank;  //!< global rank
            const_coord      ranks; //!< local ranks
            const LayoutType inner; //!< inner layout
            const LayoutType outer; //!< outer layout
            coord            next_ranks[Dimensions];
            coord            prev_ranks[Dimensions];

            inline virtual ~Workspace() throw() {}

            inline explicit Workspace(const LayoutType &full,
                               const_coord      &globalSizes,
                               const size_t      globalRank) :
            sizes( globalSizes ),
            size( Coord::Product(sizes) ),
            rank( globalRank ),
            ranks( Coord::LocalRanks( rank, sizes ) ),
            inner( full.split(sizes,ranks) ),
            outer( inner )
            {
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const Coord1D sz = Coord::Of(sizes,dim);
                    next_ranks[dim]  = ranks;
                    prev_ranks[dim]  = ranks;
                    parops::set_next_rank( sz, Coord::Of(next_ranks[dim],dim) );
                    parops::set_next_rank( sz, Coord::Of(prev_ranks[dim],dim) );
                }
            }




        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);
        };

    }
}



#endif

