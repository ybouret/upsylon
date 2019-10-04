//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"
#include "y/sequence/vector.hpp"


namespace upsylon {

    namespace math {

        namespace Euclidean {


#define Y_EUCLIDEAN_ARC_ARGS()     \
Y_EUCLIDEAN_POINT_ARGS();           \
Y_EUCLIDEAN_NODE_ARGS();             \
Y_EUCLIDEAN_SEGMENT_ARGS();           \
typedef vector<SharedNode> Nodes;      \
typedef vector<SharedSegment> Segments; \
typedef Arc<T,POINT> ArcType

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
                Y_EUCLIDEAN_ARC_ARGS();

                inline virtual ~Arc() throw() {}

                const Nodes    nodes;
                const Segments segments;

                virtual void   ensure(const size_t numNodes) = 0;
                virtual void   celerities() throw() = 0;


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

                inline void pushBack( const SharedPoint &p )
                {
                    const SharedNode node = new NodeType(p);
                    aliasing::_(nodes).push_back(node);
                }

                inline void popBack() throw()
                {
                    assert(nodes.size()>0);
                    aliasing::_(nodes).pop_back();
                }

                inline void pushSegment( const SharedNode &a, const SharedNode &b )
                {
                    const SharedSegment s = new SegmentType(a,b);
                    aliasing::_(segments).push_back(s);
                }

                inline void pushGrowing()
                {
                    assert(nodes.size()>=2);
                    const size_t n = nodes.size();
                    pushSegment(nodes[n-1],nodes[n]);
                }

                inline void pushClosing()
                {
                    assert(nodes.size()>=2);
                    pushSegment(nodes.back(),nodes.front());
                }



                inline void popSegments(size_t n)
                {
                    assert(n<=segments.size());
                    while(n-- > 0)
                    {
                        aliasing::_(segments).pop_back();
                    }
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
                virtual void add( const SharedPoint &P ) = 0;
            };




        }

    }

}

#if 0
namespace upsylon {
    
    namespace math {
        
        namespace Euclidean {


            //! forwarding types
#define Y_EUCLIDEAN_ARC_ARGS()      \
Y_EUCLIDEAN_POINT_ARGS();           \
Y_EUCLIDEAN_SEGMENT_ARGS();         \
Y_EUCLIDEAN_CURVENODE_ARGS();       \
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
                virtual void celerities() throw()  = 0;
                virtual void compile()             = 0;


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
                inline explicit Arc() throw() : points(), segments() {}
                
                
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

                inline void curveFor(NodeType *Ni, const NodeType *Nj) throw()
                {
                    assert(Ni);
                    assert(Nj);
                    const Vertex Pi = (**Ni).position;
                    const Vertex Pj = (**Nj).position;
                    const Vertex Si = Ni->celerity;
                    const Vertex Sj = Nj->celerity;
                    const Vertex rhs1 = Pj-Pi-Si;
                    const Vertex rhs2 = Sj-Si;
                    aliasing::_(Ni->Q) = 3*rhs1 - rhs2;
                    aliasing::_(Ni->W) = rhs2 - 2*rhs1;
                }

                inline void compileStd() throw()
                {
                    NodeList &pts = aliasing::_(points);
                    for(NodeType *curr=pts.head;curr;)
                    {
                        NodeType *next =curr->next;
                        if(next)
                        {
                            this->curveFor(curr,next);
                        }
                        else
                            break;
                    }
                }

            private:
                Y_DISABLE_ASSIGN(Arc);
                virtual void add( const SharedPoint &sharedPoint ) = 0;

            public:

                class Curve
                {

                };
            };

           


            
            
        }
    }
}
#endif

#endif

