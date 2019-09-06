//! \file
#ifndef Y_OXIDE_TOPOLOGY_INCLUDED
#define Y_OXIDE_TOPOLOGY_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! handle a compute node topology
        /**
         utilities to build sizes, ranks and characteristic of
         a single node
         */
        struct Topology
        {

            //! positional information for a compute node
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
                    BuildHubStatus((const Coord1D *) & (this->sizes),
                                   (const Coord1D *) & (this->ranks),
                                   (bool *) &head,
                                   (bool *) &tail,
                                   (bool *) &seq,
                                   (bool *) &par,
                                   (bool *) &bulk,
                                   Dimensions);

                }

                //! cleanup
                inline virtual ~Hub() throw()
                {
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Hub);
            };

            //! find out where the node is and its properties in the mapping
            static void BuildHubStatus(const Coord1D *size,
                                       const Coord1D *rank,
                                       bool          *head,
                                       bool          *tail,
                                       bool          *seq,
                                       bool          *par,
                                       bool          *bulk,
                                       const unsigned dims) throw();

            //! expand layout coordinates from a Hub data
            /**
             - According to the local status, expand the coordinates
             per dimmension to build an outer layout.
             - the lower, uper and width coordinates of a layout
             are passed as initial arguments
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
