//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"

namespace upsylon {
    
    namespace math {
        
        namespace Euclidean {
            
            //! forwarding types
#define Y_EUCLIDEAN_ARC_ARGS()      \
Y_EUCLIDEAN_POINT_ARGS();           \
Y_EUCLIDEAN_SEGMENT_ARGS();         \
Y_EUCLIDEAN_POINTNODE_ARGS();       \
typedef Segments<T,POINT> SegmList; \
typedef Arc<T,POINT>      ArcType;

            //==================================================================
            //
            //
            //! base class for Arc operations
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class Arc : public Object
            {
            public:

                Y_EUCLIDEAN_ARC_ARGS(); //!< aliases
                
                
                inline virtual ~Arc() throw() {}
                

                
                const NodeList points;
                const SegmList segments;
                
                virtual bool check() const throw() = 0;
                virtual void celerities() throw() = 0;


                inline Arc & operator<<( const SharedPoint &sp )
                {
                    add(sp);
                    return *this;
                }

                inline Arc & operator<<( const Vertex &v )
                {
                    const SharedPoint sp = new PointType(v);
                    return (*this) << sp;
                }

            protected:
                inline explicit Arc() throw() {}
                
                
                inline void pushBack( const SharedPoint &sp )
                {
                    aliasing::_(points).push_back( new NodeType(sp) );
                }
                
                inline void popBack() throw()
                {
                    assert(points.size>0);
                    delete aliasing::_(points).pop_back();
                }
                
                inline void celerityOf(const NodeType *prev,
                                       NodeType       *curr,
                                       const NodeType *next) throw()
                {
                    static const_type half(0.5);
                    assert(prev); assert(curr); assert(next);
                    curr->setCelerity( half*((*next)->position - (*prev)->position) );
                }
                
            private:
                Y_DISABLE_ASSIGN(Arc);
                virtual void add( const SharedPoint &sharedPoint ) = 0;
                
            };

           


            
            
        }
    }
}

#endif

