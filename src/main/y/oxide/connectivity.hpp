//! \file
#ifndef Y_OXIDE_CONNECTIVITY_INCLUDED
#define Y_OXIDE_CONNECTIVITY_INCLUDED 1

#include "y/oxide/types.hpp"

namespace upsylon
{
    namespace Oxide
    {

        //! handle connectivity levels
        struct Connectivity
        {
            //! number of variying coords
            enum Level
            {
                Level1, //!< along a main axis
                Level2, //!< along a short diagonal
                Level3  //!< along a long diaogonal
            };

            
            //! make a link from direction and orientation
            /**
             - direction[0..dims-1] must have not zero components, set to +/- 1
             - orientation is set accordingly, with first coordinate positive
             - return a link level according to the D.O.F
             */
            static Level MakeLink(Coord1D       *direction,
                                  Coord1D       *orientation,
                                  const unsigned dims);

            //! store the direction, orientation and connectivity level
            template <typename COORD>
            class Link
            {
            public:
                static const size_t Dimensions = Coord::Get<COORD>::Dimensions; //!< workspace dimension

                const COORD     direction;     //!< regularized direction
                const COORD     orientation;   //!< same for +/- direction
                const Level     level;         //!< number of active coordinates

                //! setup from a vector
                inline explicit Link(const COORD delta) :
                direction( delta ),
                orientation( 0 ),
                level( MakeLink( (Coord1D *)&direction, (Coord1D *)&orientation, Dimensions) )
                {
                }

                //! copy
                inline Link(const Link &link) throw() :
                direction(   link.direction   ),
                orientation( link.orientation ),
                level(       link.level       )
                {
                }

                //! cleanup
                inline ~Link() throw() {}

                //! test same orientation
                static bool SameOrientation( const Link &lhs, const Link &rhs ) throw()
                {
                    return (lhs.level == rhs.level) && (rhs.orientation==lhs.orientation);
                }

            private:
                Y_DISABLE_ASSIGN(Link);
            };


        };

    }

}

#endif


