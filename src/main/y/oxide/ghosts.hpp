
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

        //! common data for Ghosts
        struct GhostsInfo
        {
            static const char *Kind(const bool async) throw(); //!< display async/local
            static const unsigned None = 0x00;    //!< No Ghosts in a given orientation
            static const unsigned Fwd  = 0x01;    //!< Only Forward Ghosts in a given orientation
            static const unsigned Rev  = 0x02;    //!< Only Reverse Ghosts in ag iven orientation
            static const unsigned Both = Fwd|Rev; //!< Both Ghosts in ag iven orientation
        };

        typedef vector<Coord1D> GhostIndices; //!< alias

        //! a ghost is a sub layout with indices
        template <typename COORD>
        class Ghost : public Layout<COORD>
        {
        public:
            typedef Layout<COORD> LayoutType; //!< alias
            GhostIndices          indices;    //!< indices of sub layout in outer layout

            //! setup build collecting indices
            inline Ghost(const LayoutType &sub,
                         const LayoutType &out ) :
            LayoutType(sub), indices( this->items, as_capacity )
            {
                assert(out.contains(*this));
                out.collect(indices,sub);
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
            typedef Ghost<COORD>                     GhostType;   //!< alias
            typedef Topology::Node<COORD>            NodeType;    //!< alias
            typedef Connectivity::Link<COORD>        LinkType;    //!< alias
            typedef Layout<COORD>                    LayoutType;  //!< alias
            typedef typename LayoutType::coord       coord;       //!< alias
            typedef typename LayoutType::const_coord const_coord; //!< alias

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

            inline const char *kind() const throw() { return GhostsInfo::Kind(async); } //!< return async|local

            //! display information
            friend inline std::ostream & operator<<( std::ostream &os, const _Ghosts &G )
            {
                Coord::Disp(os << G.kind(),G.host,2) << "-->";
                Coord::Disp(os,G.rank) << "|inner=" << G.inner << " outer=" << G.outer;
                return os;
            }

        private:
            Y_DISABLE_COPY_AND_ASSIGN(_Ghosts);
        };
        

    }
}

#endif

