//! \file
#ifndef Y_OXIDE_TOPOLOGY_INCLUDED
#define Y_OXIDE_TOPOLOGY_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! handle a compute node topology
        struct Topology
        {

            typedef unsigned   Level;          //!< number of varying coordinates
            static const Level Level0 = 0x00;  //!< no
            static const Level Level1 = 0x01;  //!< [x|y|z]
            static const Level Level2 = 0x02;  //!< [xy|xz|yz]
            static const Level Level3 = 0x04;  //!< [xyz]
            static const Level Levels = Level1|Level2|Level3; //!< acceptable Level mask

        private:
            static Level LevelFor( const Coord1D *addr, const size_t size ) throw();

        public:
            //! count varying coordinates of a vector
            template <typename COORD> static inline
            Level LevelOf( const COORD &delta ) throw()
            {
                return LevelFor( (const Coord1D *) &delta, Coord::Get<COORD>::Dimensions );
            }
            

            //! positional informations for a compute node
            template <typename COORD>
            class Node
            {
            public:
                typedef typename Layout<COORD>::coord       coord;        //!< alias
                typedef typename Layout<COORD>::const_coord const_coord;  //!< alias

                const COORD   sizes; //!< local sizes for internal computation
                const Coord1D rank;  //!< global rank
                const COORD   ranks; //!< local  ranks

                //! cleanup
                inline virtual ~Node() throw() {}

                //! setup
                inline explicit Node(const_coord   &localSizes,
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

            //! a Hub is a local compute node, with its information per dimension
            /**
             All those local information are mandatory to build connectivity
             */
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

                //--------------------------------------------------------------
                //
                // definitions
                //
                //--------------------------------------------------------------
                typedef Node<COORD>                             NodeType;    //!< base type
                typedef typename NodeType::coord                coord;       //!< alias
                typedef typename NodeType::const_coord          const_coord; //!< alias
                //typedef typename Layout<COORD>::Loop            Loop;        //!< multi loop alias
                typedef typename Coord::Get<COORD>::BooleanType bool_type;   //!< boolean vector
                typedef const bool_type                         const_bool;  //!< const boolean vector

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const_bool pbc;  //!< flags for periodic boundary conditions
                const_bool head; //!< flags for head position
                const_bool tail; //!< flags for tail position
                const_bool seq;  //!< head && tail => not parallel
                const_bool par;  //!< !seq :)
                const_bool bulk; //!< flags for bulk position

                //--------------------------------------------------------------
                //
                // implementation
                //
                //--------------------------------------------------------------
                //! setup all information
                explicit Hub(const_coord   &localSizes,
                             const Coord1D &globalRank,
                             const_coord   &globalPBC) :
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
#if 0
                    std::cerr << "ranks=" << this->ranks << "/" << this->sizes << std::endl;
                    std::cerr << "head=" << head;
                    std::cerr << ", tail=" << tail;
                    std::cerr << ", bulk=" << bulk;
                    std::cerr << ", par=" << par  << "/seq=" << seq << std::endl;
#endif
                }

            };

            //! expand layout coordinates from a Hub data
            /**
             According to the local status, expand the coordinates
             per dimmension to build an outer layout
             */
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
