//! \file
#ifndef Y_OXIDE_TOPOLOGY_INCLUDED
#define Y_OXIDE_TOPOLOGY_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon
{

    namespace Oxide
    {
        struct Topology
        {

            typedef unsigned      Level;
            static const Level    Level0 = 0x00;
            static const Level    Level1 = 0x01;
            static const Level    Level2 = 0x02;
            static const Level    Level3 = 0x04;

            template <typename COORD> static inline
            Level LevelOf( const COORD &delta ) throw()
            {
                return LevelFor( (const Coord1D *) &delta, Coord::Get<COORD>::Dimensions );
            }



            template <typename COORD>
            class Node
            {
            public:
                const COORD   delta; //!< direction
                const COORD   sizes; //!< mapping
                const COORD   ranks; //!< local ranks
                const Coord1D rank;  //!< global rank
                const Level   level; //!< kind of junction

                inline Node(const COORD &localSizes,
                            const COORD &localRanks,
                            const COORD &localDelta) throw() :
                delta( localDelta ),
                sizes( localSizes ),
                ranks( Coord::RegularizeRanks(localRanks+delta,sizes) ),
                rank( Coord::GlobalRank(sizes,ranks) ),
                level( LevelOf(delta) )
                {
                }

                inline Node(const COORD   &localSizes,
                            const Coord1D  globalRank ) throw() :
                delta(0),
                sizes( localSizes ),
                ranks( Coord::LocalRanks(globalRank,sizes) ),
                rank( globalRank ),
                level( Level0 )
                {
                }

                inline virtual ~Node() throw()
                {
                }

                friend inline
                std::ostream & operator<<( std::ostream &os , const Node &node )
                {
                    os << "<<";
                    os << "@"<< node.delta;
                    os << ":rank(" << node.ranks << ")=" << node.rank;
                    os << ">>";
                    return os;
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

            template <typename COORD>
            class Links
            {
            public:
                typedef Node<COORD> NodeType;

                const NodeType lower;
                const NodeType upper;

                inline Links(const COORD &sizes,
                             const COORD &ranks,
                             const COORD &lo,
                             const COORD &up ) throw() :
                lower(sizes,ranks,lo),
                upper(sizes,ranks,up)
                {
                    assert(lower.level==upper.level);
                }


                inline ~Links() throw()
                {

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Links);
            };

        private:
            static Level LevelFor( const Coord1D *addr, const size_t size ) throw();

        };


    }

}

#endif
