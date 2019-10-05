//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"
#include "y/sequence/vector.hpp"


namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //! forwarding arc types
            //
            //==================================================================
#define Y_EUCLIDEAN_ARC_ARGS()          \
Y_EUCLIDEAN_POINT_ARGS();               \
Y_EUCLIDEAN_NODE_ARGS();                \
Y_EUCLIDEAN_SEGMENT_ARGS();             \
typedef vector<SharedNode> Nodes;       \
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
                //==============================================================
                //
                // types and definition
                //
                //==============================================================
                Y_EUCLIDEAN_ARC_ARGS(); //!< aliases

                //==============================================================
                //
                // members
                //
                //==============================================================
                const Nodes    nodes;     //!< vector of nodes
                const Segments segments;  //!< vector of segments

                //==============================================================
                //
                // virtual interface
                //
                //==============================================================
                virtual void   ensure(const size_t numNodes)    = 0; //!< ensure memory
                virtual void   metrics() throw()                = 0; //!< compute local metrics
                virtual Vertex operator()(mutable_type u) const = 0; //!< approximation
                inline virtual ~Arc() throw() {}                     //!< cleanup


                //==============================================================
                //
                // non-virtual interface
                //
                //==============================================================

                //! add an existing point
                inline Arc & operator<<( const SharedPoint &sp )
                {
                    add(sp);
                    return *this;
                }

                //! create and add a new point
                inline Arc & operator<<( const Vertex &v )
                {
                    const SharedPoint sp = new PointType(v);
                    return (*this) << sp;
                }

                //! compile coefficients for approximation
                void compile()
                {
                    const size_t n = nodes.size();
                    if(n>0)
                    {
                        for(size_t i=segments.size();i>0;--i)
                        {
                            const SegmentType &s    = *segments[i];
                            NodeType          &node = aliasing::_(*(s.tail));
                            const NodeType    &next = *(s.head);
                            const Vertex      &P0   = node.point->position;
                            const Vertex      &P1   = next.point->position;
                            const Vertex      &S0   = node.celerity;
                            const Vertex      &S1   = next.celerity;
                            const Vertex       rhs1 = (P1-P0)-S0;
                            const Vertex       rhs2 = (S1-S0);
                            aliasing::_(node.Q) = 3*rhs1-rhs2;
                            aliasing::_(node.W) = rhs2-2*rhs1;
                        }
                    }
                }

            protected:
                //! setup
                inline explicit Arc() throw()  {}

                //! push back a shared point
                inline void pushBack( const SharedPoint &p )
                {
                    const SharedNode node = new NodeType(p);
                    aliasing::_(nodes).push_back(node);
                }

                //! pop back last point
                inline void popBack() throw()
                {
                    assert(nodes.size()>0);
                    aliasing::_(nodes).pop_back();
                }

                //! create a new segment
                inline void pushSegment( const SharedNode &a, const SharedNode &b )
                {
                    const SharedSegment s = new SegmentType(a,b);
                    aliasing::_(segments).push_back(s);
                }

                //! create a new growing segment
                inline void pushGrowing()
                {
                    assert(nodes.size()>=2);
                    const size_t n = nodes.size();
                    pushSegment(nodes[n-1],nodes[n]);
                }

                //! create a new closing segment
                inline void pushClosing()
                {
                    assert(nodes.size()>=2);
                    pushSegment(nodes.back(),nodes.front());
                }

                //! remove n last segmets
                inline void popSegments(size_t n)
                {
                    assert(n<=segments.size());
                    while(n-- > 0)
                    {
                        aliasing::_(segments).pop_back();
                    }
                }


                //! 2D normal by trigonometric rotation
                inline void computeNormals2D( int2type<2> ) throw()
                {
                    Nodes &nds = aliasing::_(nodes);
                    for(size_t i=nds.size();i>0;--i)
                    {
                        NodeType     &node = *nds[i];
                        VTX          &n = aliasing::cast<VTX,Vertex>( aliasing::_(node.basis.n) );
                        const VTX    &t = aliasing::cast<VTX,Vertex>(node.basis.t);
                        n.x = -t.y;
                        n.y =  t.x;
                    }
                }

                //! no 2D normals in 3D
                inline void computeNormals2D( int2type<3> ) throw()
                {
                }

#if 0
                //! call from computeNormals3D
                inline void finalize3D( NodeType &node, Vertex N )
                {
                    aliasing::_(node.basis).finalize(N);
                }
#endif
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
                virtual void add( const SharedPoint &P ) = 0;
            };




        }

    }

}



#endif

