//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/memory/static-slots.hpp"

namespace upsylon
{
    namespace Oxide
    {
        
        template <typename COORD>
        class Node
        {
        public:
            const COORD   localRanks;
            const Coord1D globalRank;
            
            inline Node( const COORD &sizes, const COORD ranks ) throw() :
            localRanks( ranks ),
            globalRank( Coord::GlobalRank(sizes,localRanks) )
            {
            }
            
            
            inline ~Node() throw()
            {
            }
            
            inline Node( const Node &_ ) throw() :
            localRanks(_.localRanks),
            globalRank(_.globalRank)
            {
            }
            
        private:
            Y_DISABLE_ASSIGN(Node);
        };
        
        template <typename COORD>
        class Link
        {
        public:
            typedef Node<COORD> NodeType;
            
            const size_t   dimension;
            const NodeType next;
            const NodeType prev;
            const bool     par;
            const bool     seq;
            
            inline ~Link() throw()
            {
            }
            
            inline Link( const Link &_ ) throw() :
            dimension(_.dimension),
            next(_.next),
            prev(_.prev),
            par(_.par),
            seq(_.seq)
            {}
            
            inline Link( const size_t dim, const COORD &sizes, const COORD &ranks ) throw() :
            dimension( dim ),
            next( sizes, Coord::NextRank(sizes,ranks,dimension) ),
            prev( sizes, Coord::PrevRank(sizes,ranks,dimension) ),
            par(  Coord::Of(sizes,dimension) > 1 ),
            seq( !par )
            {
                
            }
            
        private:
            Y_DISABLE_ASSIGN(Link);
        };
                
        template <typename COORD>
        class Workspace
        {
        public:
            typedef Layout<COORD>                    LayoutType;
            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;
            static  const size_t Dimensions = Coord::Get<COORD>::Dimensions;
            typedef Link<COORD> LinkType;
            typedef memory::static_slots<LinkType,Dimensions> Links;
            
            const_coord      sizes; //!< local sizes
            const size_t     size;  //!< product of sizes
            const size_t     rank;  //!< global rank
            const_coord      ranks; //!< local ranks
            const LayoutType inner; //!< inner layout
            const LayoutType outer; //!< outer layout
            const Links      links; //!< info about local topology
            
            inline virtual ~Workspace() throw() {}
            
            inline explicit Workspace(const LayoutType &full,
                                      const_coord      &globalSizes,
                                      const size_t      globalRank) :
            sizes( globalSizes ),
            size( Coord::Product(sizes) ),
            rank( globalRank ),
            ranks( Coord::LocalRanks(rank,sizes) ),
            inner( full.split(sizes,ranks) ),
            outer( inner )
            {
                Links &l = (Links &)links;
                for(size_t dim=0;dim<Dimensions;++dim)
                {
                    const LinkType dim_link(dim,sizes,ranks);
                    l.push_back( dim_link );
                }
            }
            
            
            
            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(Workspace);
        };
        
    }
}



#endif

