//! \file
#ifndef Y_GEOM_ISO2D_LINES_INCLUDED
#define Y_GEOM_ISO2D_LINES_INCLUDED 1

#include "y/geometry/iso2d/points.hpp"
#include "y/core/list.hpp"
#include "y/sequence/vector.hpp"
#include "y/ptr/arc.hpp"

namespace upsylon {

    namespace geometry {

        namespace Iso2D {

            //__________________________________________________________________
            //
            //! base class for a line
            //__________________________________________________________________
            typedef core::list_of_cpp<Node> LineType;

            //__________________________________________________________________
            //
            //! a line is a line of nodes
            //__________________________________________________________________
            class Line_ : public LineType, public counted_object
            {
            public:
                explicit Line_() throw(); //!< setup
                virtual ~Line_() throw(); //!< cleanup
                const bool cyclic;        //!< cyclic curve detection

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Line_);
            };

            //__________________________________________________________________
            //
            //! alias for dynamic isoline
            //__________________________________________________________________
            typedef arc_ptr<Line_>                  Line;
            
            //__________________________________________________________________
            //
            //! alias for a few lines
            //__________________________________________________________________
            typedef vector<Line,Contour::Allocator> Lines;

        }

    }

}

#endif

