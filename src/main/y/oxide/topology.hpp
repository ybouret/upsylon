//! \file
#ifndef Y_OXIDE_TOPOLOGY_INCLUDED
#define Y_OXIDE_TOPOLOGY_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon
{

    namespace Oxide
    {
        //! handle space topology
        struct Topology
        {

            typedef unsigned      Level;          //!< number of varying coordinates
            static const Level    Level0 = 0x00;  //!< no
            static const Level    Level1 = 0x01;  //!< [x|y|z]
            static const Level    Level2 = 0x02;  //!< [xy|xz|yz]
            static const Level    Level3 = 0x04;  //!< [xyz]
            static const Level    Levels = Level1|Level2|Level3;

            //! study level of a vector
            template <typename COORD> static inline
            Level LevelOf( const COORD &delta ) throw()
            {
                return LevelFor( (const Coord1D *) &delta, Coord::Get<COORD>::Dimensions );
            }

            //! check consistency
            static void CheckSameLevels( const Level lhs, const Level rhs );

            //! a local node to handle connectivity
            template <typename COORD>
            class Node
            {
            public:
                typedef typename Layout<COORD>::coord       coord;
                typedef typename Layout<COORD>::const_coord const_coord;



                const COORD   sizes; //!< local sizes for internal computation
                const Coord1D rank;  //!< global rank
                const COORD   ranks; //!< local  ranks

                inline virtual ~Node() throw() {}

                inline explicit Node(const COORD   &localSizes,
                                     const Coord1D &globalRank) :
                sizes( localSizes ),
                rank(  globalRank ),
                ranks( Coord::LocalRanks(localSizes,globalRank) )
                {
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

            template <typename COORD>
            class Hub : public Node<COORD>
            {
            public:
                static  const size_t Dimensions = Coord::Get<COORD>::Dimensions; //!< workspace dimension
                static  const size_t Neighbours = Coord::Get<COORD>::Neighbours; //!< number of touching neighbours
                static  const size_t Directions = Neighbours/2;                  //!< number of directions
                typedef Node<COORD>                    NodeType;
                typedef typename NodeType::coord       coord;
                typedef typename NodeType::const_coord const_coord;
                typedef typename Layout<COORD>::Loop   Loop;

                const_coord pbc;  //!< flags for pbc
                const_coord head; //!< flags for head
                const_coord tail; //!< flags for tail
                const_coord bulk; //!< flags for bulk

                explicit Hub(const COORD   &localSizes,
                             const Coord1D &globalRank,
                             const COORD   &globalPBC) :
                NodeType(localSizes,globalRank),
                pbc( Coord::ToBoolean(globalPBC) ),
                head(0),
                tail(head),
                bulk(head)
                {
                    build();
                }

                inline virtual ~Hub() throw()
                {
                }

            private:
                inline void build()
                {
                    // check the local situation

                    {
                        const Coord1D *r = (const Coord1D *) & (this->ranks);
                        const Coord1D *s = (const Coord1D *) & (this->sizes);
                        Coord1D       *h = (Coord1D       *) & head;
                        Coord1D       *t = (Coord1D       *) & tail;
                        Coord1D       *b = (Coord1D       *) & bulk;

                        for(size_t dim=0;dim<Dimensions;++dim)
                        {
                            const Coord1D ls = s[dim]; //!< local size
                            const Coord1D lr = r[dim]; //!< local rank
                            assert(ls>0);
                            assert(lr>=0);
                            assert(lr<ls);
                            const bool is_head = ( 1 == (h[dim] = (0==lr)    ? 1 : 0) );
                            const bool is_tail = ( 1 == (t[dim] = (ls-1==lr) ? 1 : 0) );
                            b[dim] = (is_head || is_tail) ? 0 : 1;
                        }
                    }
                    std::cerr << "ranks=" << this->ranks << "/" << this->sizes << std::endl;
                    std::cerr << "head=" << head << std::endl;
                    std::cerr << "tail=" << tail << std::endl;
                    std::cerr << "bulk=" << bulk << std::endl;

                    // the links
                    coord __lo(0); Coord::LD(__lo,-1);
                    coord __up(0); Coord::LD(__up, 1);
                    Loop loop(__lo,__up);
                    loop.start();

                    size_t n[3] = { 0,0,0 };
                    for( size_t j=0; j<Directions; ++j, loop.next() )
                    {
                        const_coord           lower   = loop.value;
                        const_coord           upper   = -lower;
                        const Topology::Level level   = Topology::LevelOf(lower);
                        Topology::CheckSameLevels(level, Topology::LevelOf(upper));
                        switch (level) {
                            case Topology::Level1: ++n[0]; break;
                            case Topology::Level2: ++n[1]; break;
                            case Topology::Level3: ++n[2]; break;
                        }
                    }
                    display_int::to(std::cerr << "n={",n,3) << "}" << std::endl;
                }

            };


        private:
            static Level LevelFor( const Coord1D *addr, const size_t size ) throw();

        };


    }

}

#endif
