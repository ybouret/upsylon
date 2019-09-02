//! \file
#ifndef Y_OXIDE_WORKSPACE_INCLUDED
#define Y_OXIDE_WORKSPACE_INCLUDED 1

#include "y/oxide/layout.hpp"
#include "y/memory/static-slots.hpp"

namespace upsylon
{
    namespace Oxide
    {
        //! MPI Node style information
        template <typename COORD>
        class Node
        {
        public:
            const COORD   localRanks; //!< ranks local grid
            const Coord1D globalRank; //!< global node rank
            
            //! setup
            inline Node( const COORD &sizes, const COORD ranks ) throw() :
            localRanks(ranks), globalRank( Coord::GlobalRank(sizes,localRanks) ) { }
            
            //! cleanup
            inline ~Node() throw()
            {
                Coord::LDZ_(localRanks);
                Coord::LDZ_(globalRank);
            }
            
            //! copy
            inline Node( const Node &_ ) throw() :
            localRanks(_.localRanks), globalRank(_.globalRank) { }
            
        private:
            Y_DISABLE_ASSIGN(Node);
        };
        
        
        //! information on linked node
        template <typename COORD>
        class Link
        {
        public:
            typedef Node<COORD> NodeType; //!< alias
            
            const size_t   dimension;     //!< the used dimension [0|1|2]
            const Coord1D  size;          //!< size in that direction
            const Coord1D  rank;          //!< rank in that direction
            const NodeType next;          //!< next in that dimension
            const NodeType prev;          //!< prev in that dimension
            const bool     parallel;      //!< is parallel in that dimension
            const bool     head;          //!< true if rank=0 in that dimension
            const bool     tail;          //!< true if rank=size-1 in that dimension
            const bool     bulk;          //!< !head and !tail
            //! cleanup
            inline ~Link() throw()
            {
                Coord::LDZ_(dimension);
                Coord::LDZ_(size),
                Coord::LDZ_(rank),
                Coord::LDZ_(parallel);
                Coord::LDZ_(head);
                Coord::LDZ_(tail);
                Coord::LDZ_(bulk);
            }
            
            //! copy
            inline Link( const Link &_ ) throw() :
            dimension(_.dimension),
            size(_.size),
            rank(_.rank),
            next(_.next),
            prev(_.prev),
            parallel(_.parallel),
            head(_.head),
            tail(_.tail),
            bulk(_.bulk)
            {}
            
            //! setup from a local position
            inline Link( const size_t dim, const COORD &sizes, const COORD &ranks ) throw() :
            dimension( dim ),
            size( Coord::Of(sizes,dimension) ),
            rank( Coord::Of(ranks,dimension) ),
            next( sizes, Coord::NextRank(sizes,ranks,dimension) ),
            prev( sizes, Coord::PrevRank(sizes,ranks,dimension) ),
            parallel(  size > 1 ),
            head( 0      == rank ),
            tail( size-1 == rank ),
            bulk( !head && !tail )
            {
                
            }
            
        private:
            Y_DISABLE_ASSIGN(Link);
        };
        
        //! workspace metrics
        template <typename COORD>
        class Workspace
        {
        public:
            //------------------------------------------------------------------
            //
            // types and definitions
            //
            //------------------------------------------------------------------
            typedef Layout<COORD>                             LayoutType; //!< alias
            typedef typename LayoutType::coord                coord;      //!< alias
            typedef typename LayoutType::const_coord          const_coord;//!< alias
            static  const size_t                              Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            typedef Link<COORD>                               LinkType; //!< alias
            typedef memory::static_slots<LinkType,Dimensions> Links;    //!< alias

            //------------------------------------------------------------------
            //
            // members
            //
            //------------------------------------------------------------------
            const_coord      sizes; //!< local sizes
            const size_t     size;  //!< product of sizes
            const size_t     rank;  //!< global rank
            const_coord      ranks; //!< local ranks
            const LayoutType inner; //!< inner layout
            const LayoutType outer; //!< outer layout
            const Links      links; //!< info about local topology

            //------------------------------------------------------------------
            //
            // methods
            //
            //------------------------------------------------------------------

            //! cleanup
            inline virtual ~Workspace() throw()
            {
                Coord::LDZ_(sizes);
                Coord::LDZ_(size);
                Coord::LDZ_(rank);
                Coord::LDZ_(ranks);
            }
            
            //! setup
            inline explicit Workspace(const LayoutType &full,
                                      const_coord      &globalSizes,
                                      const size_t      globalRank) :
            sizes( globalSizes ),
            size( Coord::Product(sizes) ),
            rank( globalRank ),
            ranks( Coord::LocalRanks(rank,sizes) ),
            inner( full.split(sizes,ranks) ),
            outer( inner ),
            links()
            {
                //--------------------------------------------------------------
                // find local topology
                //--------------------------------------------------------------
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

