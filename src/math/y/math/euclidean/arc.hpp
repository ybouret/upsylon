
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
                virtual void kinematics(const ArcClass) throw() = 0;
                virtual void compute( mutable_type u, vertex *p, vertex *dp, vertex *d2p ) const throw() = 0;



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

                //! upadte after kinematics is computed/tunes
                inline void update(const ArcClass C) throw()
                {
                    for(size_t i=segments.size();i>0;--i)
                    {
                        aliasing::_( *segments[i] ).build(C);
                    }

                    switch(C)
                    {
                        case Arc0: onCompute = & NodeType::compute0; break;
                        case Arc1: onCompute = & NodeType::compute1; break;
                        case Arc2: onCompute = & NodeType::compute2; break;
                    }

                    // third pass: local tangents
                    for(size_t i=nodes.size();i>0;--i)
                    {
                        aliasing::_( *nodes[i] ).setTangent();
                    }
                }

                //! full motion
                inline void motion(const ArcClass C) throw()
                {
                    // first pass: local kinematics
                    kinematics(C);

                    // second pass:
                    update(C);
                }

                inline vertex operator()(const_type u, vertex *dp=0, vertex *d2p=0) const throw()
                {
                    vertex p;
                    compute(u,&p,dp,d2p);
                    return p;
                }

                inline type speed(const_type u) const throw()
                {
                    vertex s;
                    compute(u,0,&s,0);
                    return sqrt_of( s.norm2() );
                }



            protected:
                typename NodeType::Compute onCompute;

                inline explicit Arc() throw() : Object(), nodes(), segments(), onCompute(0) {}


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

                inline static
                void motionBulkFor(const NodeType &prev,
                                   const NodeType &curr,
                                   const NodeType &next,
                                   const ArcClass C) throw()
                {
                    static const_type half(0.5);
                    const_vertex   &Pm = prev.P;
                    const_vertex   &P0 = curr.P;
                    const_vertex   &Pp = next.P;
                    switch(C)
                    {
                        case Arc2: aliasing::_(curr.A) = (Pp-P0)+(Pm-P0);
                        case Arc1: aliasing::_(curr.V) = half*(Pp-Pm);
                        case Arc0: break;
                    }
                }

                void motionBulk( const ArcClass C ) throw()
                {
                    const size_t      num = nodes.size();
                    for(size_t im=1,i=2,ip=3;i<num;++im,++i,++ip)
                    {
                        const NodeType &node = *nodes[i];
                        aliasing::_(node).reset();
                        motionBulkFor(*nodes[im], node, *nodes[ip], C);
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

