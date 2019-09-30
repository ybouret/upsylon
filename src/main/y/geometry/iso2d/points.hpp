//! \file
#ifndef Y_GEOM_ISO2D_POINTS_INCLUDED
#define Y_GEOM_ISO2D_POINTS_INCLUDED 1

#include "y/geometry/iso2d/edge.hpp"
#include "y/ptr/intr.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            //__________________________________________________________________
            //
            //! unique point
            //__________________________________________________________________
            class Point_ : public counted_object
            {
            public:
                const Edge   location; //!< logical
                const Vertex position; //!< physical

                explicit     Point_(const Edge &,const Vertex &) throw(); //!< setup
                virtual     ~Point_() throw();                            //!< cleanup
                const Edge & key() const throw();                         //!< for set<...>

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Point_);
            };

            //__________________________________________________________________
            //
            //! alias for dynamic point
            //__________________________________________________________________
            typedef intr_ptr<Edge,Point_>                       Point;


            //__________________________________________________________________
            //
            //! a dynamic Node for an Line
            //__________________________________________________________________
            class Node : public Point
            {
            public:
                Node *next; //!< for isolist
                Node *prev; //!< for isolist

                explicit Node( const Point & ) throw(); //!< setup
                virtual ~Node() throw();                //!< cleanup

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };
            
        }

    }
}

#endif

