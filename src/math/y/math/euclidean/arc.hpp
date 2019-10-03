//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //
            //! Boundaries for Arc
            //
            //
            //==================================================================
            enum Boundaries
            {
                AreStandard,
                ArePeriodic
            };


            //==================================================================
            //
            //
            //! Arc with
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class Arc : public Object
            {
            public:
                //==============================================================
                //
                // types and declarations
                //
                //==============================================================
                Y_EUCLIDEAN_ARGS(); //!< alias

                //==============================================================
                //
                // public members
                //
                //==============================================================
                const Boundaries boundaries; //!< kind of boundaries

                //==============================================================
                //
                // methods
                //
                //==============================================================

                //! setup
                inline explicit Arc(const Boundaries kind) throw() : boundaries(kind)
                {}

                //! cleanup
                inline virtual ~Arc() throw() {}

                //! append one point
                inline Arc & operator<<( const SharedPoint &sharedPoint )
                {

                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);


            };

        }
    }
}

#endif

