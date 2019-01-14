#ifndef Y_GEOMETRY_ISO2D_INCLUDED
#define Y_GEOMETRY_ISO2D_INCLUDED 1


#include "y/type/point2d.hpp"
#include "y/associative/set.hpp"

namespace upsylon
{
    namespace geometry
    {

        //! findind iso levels in 2d
        /**
         Each square is divided in four triangles
         4@(i  ,j+1)------3@(i+1,j+1)
         | \           /  |
         |   \       /    |
         |     \   /      |
         |       0        |
         |     /   \      |
         |   /       \    |
         | /           \  |
         1@(i ,j   )-----2@(i+1,j  )

         For a given level, a matching value can be found on a vertex or
         on within an edge, which defines a set of unique coordinates
         for any point of the isoline
         */
        struct iso2d
        {
            //__________________________________________________________________
            //
            // iso level is on a vertex
            //__________________________________________________________________
            static const unsigned vertex0 = 0x01<<0; //!< bit 1
            static const unsigned vertex1 = 0x01<<1; //!< bit 2
            static const unsigned vertex2 = 0x01<<2; //!< bit 3
            static const unsigned vertex3 = 0x01<<3; //!< bit 4
            static const unsigned vertex4 = 0x01<<4; //!< bit 5

            //__________________________________________________________________
            //
            // iso level is on an edge
            //__________________________________________________________________
            static const unsigned edge01  = (vertex0|vertex1);
            static const unsigned edge02  = (vertex0|vertex2);
            static const unsigned edge03  = (vertex0|vertex3);
            static const unsigned edge04  = (vertex0|vertex4);

            static const unsigned edge12  = (vertex1|vertex2);
            static const unsigned edge14  = (vertex1|vertex4);

            static const unsigned edge23  = (vertex2|vertex3);
            static const unsigned edge34  = (vertex3|vertex4);


            class identifier
            {
            public:
                const unit_t   i; //!< index of the square
                const unit_t   j; //!< index of the square
                const unsigned p; //!< flag of

            };



        };

    }
}

#endif

