
//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"
#include "y/sequence/vector.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            template <typename T, template <class> class VTX>
            class Arc : public Object
            {
            public:
                Y_EUCLIDEAN_SEGMENT_TYPES();
                typedef vector<SharedNode>    Nodes;
                typedef vector<SharedSegment> Segments;

                inline virtual ~Arc() throw() {}


                virtual void ensure(const size_t) = 0;

                const Nodes    nodes;
                const Segments segments;

                inline Arc & operator<<( const SharedPoint &p)
                {
                    add(p);
                    return *this;
                }

                inline Arc & operator<<( const_vertex p)
                {
                    const SharedPoint P = new PointType(p);
                    return (*this) << P;
                }


            protected:
                inline explicit Arc() throw() : Object(), nodes(), segments() {}


                void pushBack(const SharedPoint &p)
                {
                    const SharedNode node = new NodeType(p);
                    aliasing::_(nodes).push_back(node);
                }

                void popBack() throw()
                {
                    assert(nodes.size()>0);
                    aliasing::_(nodes).pop_back();
                }

                //! push a growing segment
                void pushGrowing()
                {
                    assert(nodes.size()>=2);
                    const size_t        n = nodes.size();
                    const SharedSegment s = new SegmentType( nodes[n-1], nodes[n] );
                    aliasing::_(segments).push_back(s);
                }

                //! push a closing segment
                void pushClosing()
                {
                    assert(nodes.size()>=2);
                    const SharedSegment s = new SegmentType( nodes.back(), nodes.front() );
                    aliasing::_(segments).push_back(s);
                }


                void popSegments(size_t n) throw()
                {
                    assert(n<=segments.size());
                    while(n-- > 0)
                    {
                        aliasing::_(segments).pop_back();
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
                virtual void add(const SharedPoint &p) = 0;
            };

#define Y_EUCLIDEAN_ARC_TYPES()                \
Y_EUCLIDEAN_SEGMENT_TYPES();                   \
typedef Arc<T,VTX> ArcType;                    \
typedef typename   ArcType::Nodes    Nodes;    \
typedef typename   ArcType::Segments Segments

        }
    }
}

#endif

