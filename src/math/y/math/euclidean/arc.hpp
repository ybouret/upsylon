
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
            //
            //! base class for arc
            //
            //
            //==================================================================
            template <typename T, template <class> class VTX>
            class Arc : public Object
            {
            public:
                Y_EUCLIDEAN_SEGMENT_TYPES();             //!< aliases
                typedef vector<SharedNode>    Nodes;     //!< alias
                typedef vector<SharedSegment> Segments;  //!< alias

                const Nodes    nodes;      //!< sequence of nodes
                const Segments segments;   //!< sequence of segments


                //! cleanup
                inline virtual ~Arc() throw() {}


                //! ensure memory for points+segments
                virtual void ensure(const size_t) = 0;

                //! interpolation funcion
                virtual void compute( mutable_type u, vertex *p, vertex *dp, vertex *d2p ) const throw() = 0;


                //! add an existing shared point
                inline Arc & operator<<( const SharedPoint &p)
                {
                    add(p);
                    return *this;
                }

                //! add a new shared point for vertex
                inline Arc & operator<<( const_vertex p)
                {
                    const SharedPoint P = new PointType(p);
                    return (*this) << P;
                }

                //! start computation
                inline void  start() throw()
                {
                    kinematics();
                    for(size_t i=nodes.size();i>0;--i)
                    {
                        aliasing::_(nodes[i])->initializeBasis();
                    }
                }

                //! update after start: build arc support
                inline void update(const ArcClass C) throw()
                {
                    // build interpolating value
                    for(size_t i=segments.size();i>0;--i)
                    {
                        aliasing::_( *segments[i] ).build(C);
                    }

                    // adjust interpolation function
                    switch(C)
                    {
                        case Arc0: onCompute = & NodeType::compute0; break;
                        case Arc1: onCompute = & NodeType::compute1; break;
                        case Arc2: onCompute = & NodeType::compute2; break;
                    }

                    finalize( int2type<Dimensions>() );

                }

                //! full motion start+update
                inline void motion(const ArcClass C) throw()
                {
                    // first pass: local kinematics
                    start();

                    // second pass:
                    update(C);
                }

                //! get vertex with option dp and d2p
                inline vertex operator()(const_type u, vertex *dp=0, vertex *d2p=0) const throw()
                {
                    vertex p;
                    compute(u,&p,dp,d2p);
                    return p;
                }

                //! get |V|
                inline type speed(const_type u) const throw()
                {
                    vertex s;
                    compute(u,0,&s,0);
                    return sqrt_of( s.norm2() );
                }

                //! sum individual segment lengths
                inline type length() throw()
                {
                    mutable_type l = 0;
                    for(size_t i=segments.size();i>0;--i)
                    {
                        const SegmentType &segment = *segments[i];
                        const_vertex       v(segment.head->P,segment.tail->P);
                        l += sqrt_of( v.norm2() );
                    }
                    return l;
                }



            protected:
                typename NodeType::Compute onCompute; //!< pointer to method

                //! setup
                inline explicit Arc() throw() : Object(), nodes(), segments(), onCompute(0) {}


                //! push a new node
                void pushBack(const SharedPoint &p)
                {
                    const SharedNode node = new NodeType(p);
                    aliasing::_(nodes).push_back(node);
                }

                //! remove last node
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

                //! pop n last segments
                void popSegments(size_t n) throw()
                {
                    assert(n<=segments.size());
                    while(n-- > 0)
                    {
                        aliasing::_(segments).pop_back();
                    }
                }

                //! compute bulk kinematic, V approx even for Arc0
                inline static
                void motionBulkFor(const NodeType &prev,
                                   const NodeType &curr,
                                   const NodeType &next) throw()
                {
                    static const_type half(0.5);
                    
                    const_vertex   &Pm  = prev.P;
                    const_vertex   &P0  = curr.P;
                    const_vertex   &Pp  = next.P;

                    aliasing::_(curr.A) = (Pp-P0)+(Pm-P0);
                    aliasing::_(curr.V) = half*(Pp-Pm);

                }

                //! compute motion data for bulk point
                void motionBulk() throw()
                {
                    const size_t      num = nodes.size();
                    for(size_t im=1,i=2,ip=3;i<num;++im,++i,++ip)
                    {
                        const NodeType &node = *nodes[i];
                        motionBulkFor(*nodes[im], node, *nodes[ip]);
                    }
                }

                //! finalize Basis in 2D
                inline void finalize( int2type<2> ) throw()
                {
                    for(size_t i=nodes.size();i>0;--i)
                    {
                        const NodeType &node = *nodes[i];
                        const Basis    &basis = node.basis;
                        aliasing::_(basis.n) = basis.t.direct_normal();
                    }
                }

                //! finalize Basis in 3D
                inline void finalize( int2type<3> ) throw()
                {
                    for(size_t i=nodes.size();i>0;--i)
                    {
                        const NodeType &node = *nodes[i];
                        const Basis    &basis = node.basis;
                        const_vertex   &t     = basis.t;
                        vertex         &n     = aliasing::_(basis.n);
                        n  = dT(i);
                        n -= (n*t)*t;
                        const_type n2 = n.norm2();
                        if(n2>0)
                        {
                            n /= sqrt_of( n2 );
                        }
                    }
                }



                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Arc);
                virtual void   add(const SharedPoint &p)        = 0;
                virtual void   kinematics()             throw() = 0;
                virtual vertex dT(const size_t n) const throw() = 0;

            };

            //! forward nested types
#define Y_EUCLIDEAN_ARC_TYPES()                \
Y_EUCLIDEAN_SEGMENT_TYPES();                   \
typedef Arc<T,VTX> ArcType;                    \
typedef typename   ArcType::Nodes    Nodes;    \
typedef typename   ArcType::Segments Segments

        }
    }
}

#endif

