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
                Y_EUCLIDEAN_POINT_ARGS();     //!< aliases
                Y_EUCLIDEAN_SEGMENT_ARGS();   //!< aliases
                Y_EUCLIDEAN_POINTNODE_ARGS(); //!< aliases
                typedef Segments<T,POINT> SegmentsType;
                
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

                //! append one existing point
                inline Arc & operator<<( const SharedPoint &sharedPoint )
                {
                    std::cerr << sharedPoint->key() << std::endl;
                    return *this;
                }

                //! append a new point
                inline Arc & operator<<( const Vertex &v )
                {
                    const SharedPoint P = new SharedPoint(v);
                    return (*this) << P;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
                NodeList      points;
                SegmentsType  segments;


            };

        }
    }
}

#endif

