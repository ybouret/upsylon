//! \file
#ifndef Y_GEOM_ISO2D_COORD_INCLUDED
#define Y_GEOM_ISO2D_COORD_INCLUDED 1

#include "y/geometry/contour.hpp"
#include "y/type/point2d.hpp"
#include "y/hashing/function.hpp"

namespace upsylon {

    namespace geometry {

        //======================================================================
        //
        //! finding iso levels in 2d
        //
        //======================================================================
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
         on an edge, which defines a set of unique Coordinates
         for any point of the isoline
         */
        namespace Iso2D
        {
            //__________________________________________________________________
            //
            //! physical position
            //__________________________________________________________________
            typedef point2d<double> Vertex;

            //__________________________________________________________________
            //
            //! logical location, with lexicographic ordering
            //__________________________________________________________________
            class Coordinate
            {
            public:
                static const short Full = 0; //!< mark for complete coordinate
                static const short Half = 1; //!< mark for center coordinate

                const unit_t i; //!< logical index
                const unit_t j; //!< logical index
                const short  q; //!< modifier

                Coordinate(const unit_t,const unit_t,const short) throw();           //!< setup
                ~Coordinate() throw();                                               //!< cleanup
                Coordinate(const Coordinate &) throw();                              //!< copy
                friend bool operator==(const Coordinate&,const Coordinate&) throw(); //!< component-wise
                friend bool operator!=(const Coordinate&,const Coordinate&) throw(); //!< component-wise
                friend bool operator< (const Coordinate&,const Coordinate&) throw(); //!< lexicographic
                static int  Compare(const Coordinate&,const Coordinate&) throw();    //!< lexicographic
                void        __run(hashing::function &) const throw();                //!< run hash function
                void        __sto(unit_t *) const throw();                           //!< store components
                friend std::ostream & operator<<(std::ostream &,const Coordinate&);  //!< display

            private:
                Y_DISABLE_ASSIGN(Coordinate);
            };

        }

    }

}
#endif
