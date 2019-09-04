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
            static const Level    Levels = Level1|Level2|Level3;

            template <typename COORD> static inline
            Level LevelOf( const COORD &delta ) throw()
            {
                return LevelFor( (const Coord1D *) &delta, Coord::Get<COORD>::Dimensions );
            }

            static void CheckSameLevels( const Level lhs, const Level rhs );

            template <typename COORD>
            class Node
            {
            public:
                const COORD   sizes; //!< local sizes for internal computation
                const Coord1D rank;  //!< global rank
                const COORD   ranks;

                inline virtual ~Node() throw() {}

                inline explicit Node(const COORD   &localSizes,
                                     const Coord1D &globalRank) throw() :
                sizes( localSizes ),
                rank(  globalRank ),
                ranks( Coord::LocalRanks(globalRank,localSizes) )
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };


        private:
            static Level LevelFor( const Coord1D *addr, const size_t size ) throw();

        };


    }

}

#endif
