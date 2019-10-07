//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"
#include "y/sequence/vector.hpp"
#include "y/math/kernel/lu.hpp"

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
                //! interpolation of values
                virtual void   compute( mutable_type u, Vertex *P, Vertex *dP, Vertex *d2P ) const throw() = 0;

                inline virtual ~Arc() throw() {}                     //!< cleanup



                //==============================================================
                //
                // non-virtual interface
                //
                //==============================================================
                //! return position with optional speed and acceleration
                inline Vertex operator()(const_type u, Vertex *dP=0, Vertex *d2P=0) const throw()
                {
                    Vertex P;
                    compute(u,&P,dP,d2P);
                    return P;
                }

                //! local speed norm
                inline type getSpeed(const_type u) const throw()
                {
                    Vertex dP;
                    compute(u,0,&dP,0);
                    return sqrt_of( aliasing::cast<VTX,Vertex>(dP).norm2() );
                }

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
                void compileOld()
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
                            const Vertex      &V0   = node.V;
                            const Vertex      &V1   = next.V;
                            const Vertex       rhs1 = (P1-P0)-V0;
                            const Vertex       rhs2 = (V1-V0);
                            aliasing::_(node.A) = 3*rhs1-rhs2;
                            aliasing::_(node.B) = rhs2-2*rhs1;
                        }
                    }
                }

                void compile()
                {
                    std::cerr << "compiling..." << std::endl;
                    const size_t M = segments.size();
                    const size_t N = nodes.size();
                    if(M>0)
                    {
                        matrix<mutable_type> alpha(M,M);
                        for(size_t k=M;k>0;--k)
                        {
                            const size_t kp = _next(k,N);
                            for(size_t j=M;j>0;--j)
                            {
                                const size_t jp = _next(j,N);
                                alpha[k][j] = _delta(k,j)+_delta(kp,jp)-( _delta(k,jp) + _delta(kp,j) );
                            }

                        }
                        std::cerr << "alpha=" << alpha << std::endl;
                    }
                    //exit(0);
                }


            protected:
                //! setup
                inline explicit Arc() throw() : nodes(), segments() {}

                static inline size_t _next(const size_t j, const size_t N) throw()
                {
                    const size_t jp1 = j+1;
                    return (jp1>N) ? 1 : jp1;
                }

                static inline mutable_type _delta(const size_t i, const size_t j) throw()
                {
                    return (i!=j) ? 0 : 1;
                }

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

