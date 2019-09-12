//! \file
#ifndef Y_OXIDE_TOPOLOGY_INCLUDED
#define Y_OXIDE_TOPOLOGY_INCLUDED 1

#include "y/oxide/layout.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! Handle information about a compute node
        /**
         utilities to build sizes, ranks and characteristic of
         a single Node (head,bulk,tail)
         */
        struct Topology
        {

            //! a Node is some positional information
            /**
             common local sizes (=mapping), global rank, and local ranks
             */
            template <typename COORD>
            class Node
            {
            public:
                //--------------------------------------------------------------
                //
                // types and definitions
                //
                //--------------------------------------------------------------
                typedef typename Layout<COORD>::coord       coord;        //!< alias
                typedef typename Layout<COORD>::const_coord const_coord;  //!< alias

                //--------------------------------------------------------------
                //
                // members
                //
                //--------------------------------------------------------------
                const_coord   sizes; //!< local sizes for internal computation
                const Coord1D rank;  //!< global rank
                const_coord   ranks; //!< local  ranks in local sizes

                //--------------------------------------------------------------
                //
                // methods
                //
                //--------------------------------------------------------------
               
                //! setup from localSizes (=mapping) and a globalRank
                inline explicit Node(const_coord   &localSizes,
                                     const Coord1D  globalRank) :
                sizes( localSizes ),
                rank(  globalRank ),
                ranks( Coord::LocalRanks(localSizes,globalRank) )
                {
                }

                //! cleanup
                inline virtual ~Node() throw()
                {
                    bzset_(sizes);
                    bzset_(rank);
                    bzset_(ranks);
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
            public://--------------------------------------------------------------
                //
                // definitions
                //
                //--------------------------------------------------------------
                typedef Node<COORD>                             NodeType;    //!< base type
                typedef typename NodeType::coord                coord;       //!< alias
                typedef typename NodeType::const_coord          const_coord; //!< alias
                typedef typename Coord::Get<COORD>::BooleanType bool_type;   //!< boolean vector
                typedef const bool_type                         const_bool;  //!< const boolean vector
                static  const size_t Dimensions = Coord::Get<COORD>::Dimensions; //!< workspace dimension

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
                             const Coord1D  globalRank,
                             const_coord   &boundaryConditions) :
                NodeType(localSizes,globalRank),
                pbc( Coord::ToBoolean(boundaryConditions) ),
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
                    bzset_(head);
                    bzset_(tail);
                    bzset_(seq);
                    bzset_(par);
                    bzset_(bulk);
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
