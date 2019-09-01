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
            localRanks( ranks ),
            globalRank( Coord::GlobalRank(sizes,localRanks) )
            {
            }
            
            //! cleanup
            inline ~Node() throw()
            {
            }
            
            //! copy
            inline Node( const Node &_ ) throw() :
            localRanks(_.localRanks),
            globalRank(_.globalRank)
            {
            }
            
        private:
            Y_DISABLE_ASSIGN(Node);
        };
        
        
        //! information on linked node
        template <typename COORD>
        class Link
        {
        public:
            typedef Node<COORD> NodeType; //!< alias
            
            const size_t   dimension; //!< the used dimension [0|1|2]
            const NodeType next;      //!< next in that dimension
            const NodeType prev;      //!< prev in that dimension
            const bool     par;       //!< is parallel
            const bool     seq;       //!< is sequential
            
            //! cleanup
            inline ~Link() throw()
            {
            }
            
            //! copy
            inline Link( const Link &_ ) throw() :
            dimension(_.dimension),
            next(_.next),
            prev(_.prev),
            par(_.par),
            seq(_.seq)
            {}
            
            //! setup from a local position
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
        
        //! workspace metrics
        template <typename COORD>
        class Workspace
        {
        public:
            typedef Layout<COORD>                             LayoutType; //!< alias
            typedef typename LayoutType::coord                coord;      //!< alias
            typedef typename LayoutType::const_coord          const_coord;//!< alias
            static  const size_t                              Dimensions = Coord::Get<COORD>::Dimensions; //!< alias
            typedef Link<COORD>                               LinkType; //!< alias
            typedef memory::static_slots<LinkType,Dimensions> Links;    //!< alias
            
            const_coord      sizes; //!< local sizes
            const size_t     size;  //!< product of sizes
            const size_t     rank;  //!< global rank
            const_coord      ranks; //!< local ranks
            const LayoutType inner; //!< inner layout
            const LayoutType outer; //!< outer layout
            const Links      links; //!< info about local topology
            
            //! cleanup
            inline virtual ~Workspace() throw() {}
            
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

