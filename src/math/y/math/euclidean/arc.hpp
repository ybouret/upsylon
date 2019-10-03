
//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {


            enum Boundaries
            {
                AreStandard,
                ArePeriodic
            };

            template <typename T, template <class> class POINT>
            class Arc : public Object
            {
            public:
                Y_EUCLIDEAN_ARGS();
                const Boundaries boundaries;

                inline explicit Arc(const Boundaries kind) throw() : boundaries(kind)
                {}
                inline virtual ~Arc() throw() {}

                


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);


            };

        }
    }
}

#endif

