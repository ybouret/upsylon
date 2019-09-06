
//! \file
#ifndef Y_OXIDE_GHOSTS_INCLUDED
#define Y_OXIDE_GHOSTS_INCLUDED 1

#include "y/oxide/connectivity.hpp"
#include "y/oxide/topology.hpp"
#include "y/ptr/arc.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon
{
    namespace Oxide
    {

        struct GhostsInfo
        {
            static const char *Kind(const bool async) throw();
        };

        //! a ghost is a sub layout with indices
        template <typename COORD>
        class Ghost : public Layout<COORD>
        {
        public:
            typedef Layout<COORD> LayoutType;

            vector<Coord1D>  indices; //!< indices to pick up items later

            //! setup build collecting indices
            inline Ghost(const LayoutType &sub,
                         const LayoutType &out ) :
            LayoutType(sub), indices( this->items, as_capacity )
            {
                assert(out.contains(*this));
                out.collect(indices, sub);
                assert(this->items==indices.size());
            }

            //! cleanup, do nothing
            inline ~Ghost() throw()  {}

        private:
            Y_DISABLE_COPY_AND_ASSIGN(Ghost);
        };

        //! base class for ghosts inner and outer sublayouts
        /**
         The positional NodeType will be used as peer information for I/O
         */
        template <typename COORD>
        class _Ghosts : public Topology::Node<COORD>, public counted_object
        {
        public:
            typedef Ghost<COORD>                     GhostType;
            typedef Topology::Node<COORD>            NodeType;
            typedef Connectivity::Link<COORD>        LinkType;
            typedef Layout<COORD>                    LayoutType;
            typedef typename LayoutType::coord       coord;
            typedef typename LayoutType::const_coord const_coord;

            const LinkType          link;  //!< directionality of ghosts
            const Coord1D           host;  //!< global host rank
            const bool              local; //!< is local
            const bool              async; //!< is async

            //! setup
            inline explicit _Ghosts(const_coord           &localSizes,
                                    const Coord1D         &globalRank,
                                    const Coord1D         &globalHost,
                                    const LinkType        &localLink,
                                    const LayoutType      &innerArea,
                                    const LayoutType      &outerArea,
                                    const LayoutType      &outerLayout) throw() :
            NodeType(localSizes,globalRank), link(localLink),
            host(globalHost),
            local( (host==this->rank)  ),
            async( !local ),
            inner(innerArea,outerLayout),
            outer(outerArea,outerLayout)
            {
                assert(inner.items==outer.items);
            }

            //! cleanup
            inline virtual ~_Ghosts() throw()
            {
            }

            const GhostType inner; //!< inner, to send
            const GhostType outer; //!< outer, to recv

            inline const char *kind() const throw() { return GhostsInfo::Kind(async); }

            friend inline std::ostream & operator<<( std::ostream &os, const _Ghosts &G )
            {
                Coord::Disp(os << G.kind(),G.host,2) << "-->";
                Coord::Disp(os,G.rank) << "|inner=" << G.inner << " G.outer=" << G.outer;
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(_Ghosts);
        };

        //! a pair of ghosts with same orientation
        template <typename COORD>
        class _GhostsPair : public counted_object
        {
        public:
            typedef arc_ptr< _Ghosts<COORD> > Ghosts;
            const Ghosts forward;  //!< forward ghosts
            const Ghosts reverse;  //!< reverse ghosts

            //! setup
            inline _GhostsPair( const Ghosts &g1, const Ghosts &g2) throw() :
            forward(g1),
            reverse(g2)
            {
                assert( Connectivity::Link<COORD>::ArePaired(g1->link,g2->link) );
                if( Connectivity::Reverse == forward->link.way )
                {
                    cswap(forward,reverse);
                }
            }

            //! cleanup
            inline ~_GhostsPair() throw()
            {

            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(_GhostsPair);
        };

        enum GhostsMultiplicity
        {
            SingleGhosts,
            PairedGhosts
        };

        template <typename COORD>
        class GhostsIO
        {
        public:
            const GhostsMultiplicity multiplicity;
            union
            {
                const _GhostsPair<COORD> *paired;
                const _Ghosts<COORD>     *single;
            };

            
        private:
            Y_DISABLE_COPY_AND_ASSIGN(GhostsIO);
        };

    }
}

#endif

