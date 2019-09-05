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
        private:
            static Level LevelFor( const Coord1D *addr, const size_t size ) throw();

        public:
            //! study level of a vector
            template <typename COORD> static inline
            Level LevelOf( const COORD &delta ) throw()
            {
                return LevelFor( (const Coord1D *) &delta, Coord::Get<COORD>::Dimensions );
            }

            //! check consistency
            static void CheckSameLevels( const Level lhs, const Level rhs );

            //! check
            static void CheckGhosts( const Coord1D ng, const Coord1D width, const unsigned dim);

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


        private:
            static void BuildHubStatus(const Coord1D *size,
                                       const Coord1D *rank,
                                       bool    *head,
                                       bool    *tail,
                                       bool    *seq,
                                       bool    *par,
                                       bool    *bulk,
                                       const unsigned dims) throw();
        public:
            template <typename COORD>
            class Hub : public Node<COORD>
            {
            public:
                //--------------------------------------------------------------
                //
                // connectivity
                //
                //--------------------------------------------------------------
                static  const size_t Dimensions = Coord::Get<COORD>::Dimensions; //!< workspace dimension
                static  const size_t Neighbours = Coord::Get<COORD>::Neighbours; //!< number of touching neighbours
                static  const size_t Directions = Neighbours/2;                  //!< number of directions

                //--------------------------------------------------------------
                //
                // definitions
                //
                //--------------------------------------------------------------
                typedef Node<COORD>                             NodeType;
                typedef typename NodeType::coord                coord;
                typedef typename NodeType::const_coord          const_coord;
                typedef typename Layout<COORD>::Loop            Loop;
                typedef typename Coord::Get<COORD>::BooleanType bool_type;
                typedef const bool_type                         const_bool;

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const_bool pbc;  //!< flags for pbc
                const_bool head; //!< flags for head
                const_bool tail; //!< flags for tail
                const_bool seq;  //!< head && tail => not parallel
                const_bool par;  //!< !both :)
                const_bool bulk; //!< flags for bulk

                //--------------------------------------------------------------
                //
                // implementation
                //
                //--------------------------------------------------------------
                //! setup
                explicit Hub(const COORD   &localSizes,
                             const Coord1D &globalRank,
                             const COORD   &globalPBC) :
                NodeType(localSizes,globalRank),
                pbc( Coord::ToBoolean(globalPBC) ),
                head(false),
                tail(head),
                seq(head),
                par(head),
                bulk(head)
                {
                    buildStatus();
                }

                //! cleanup
                inline virtual ~Hub() throw()
                {
                }

            private:
                inline void buildStatus()
                {
                    // check the local situation
                    BuildHubStatus((const Coord1D *) & (this->sizes),
                                   (const Coord1D *) & (this->ranks),
                                   (bool *) &head,
                                   (bool *) &tail,
                                   (bool *) &seq,
                                   (bool *) &par,
                                   (bool *) &bulk,
                                   Dimensions);

                    std::cerr << "ranks=" << this->ranks << "/" << this->sizes << std::endl;
                    std::cerr << "head=" << head;
                    std::cerr << ", tail=" << tail;
                    std::cerr << ", bulk=" << bulk;
                    std::cerr << ", par=" << par  << "/seq=" << seq << std::endl;
                }

            };

            //! expand layout coordinates from a Hub data
            static void Expand(Coord1D       *lower,
                               Coord1D       *upper,
                               const Coord1D *width,
                               const bool    *head,
                               const bool    *tail,
                               const bool    *bulk,
                               const bool    *par,
                               const bool    *pbc,
                               const Coord1D  ng,
                               const unsigned dims);


        };


    };

}

#endif
