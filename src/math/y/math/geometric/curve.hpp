//! \file
#ifndef Y_GEOMETRIC_CURVE_INCLUDED
#define Y_GEOMETRIC_CURVE_INCLUDED 1

#include "y/math/geometric/points.hpp"

namespace upsylon
{
    
    namespace math
    {

        namespace Geometric
        {

            //! a generic curve of POINT<T>
            template <typename T, template <class> class POINT, class ALLOCATOR=memory::global>
            class Curve
            {
            public:
                Y_DECL_ARGS(T,type);                                               //!< aliases
                typedef PointInfoFor<T,POINT>    PointInfo;                        //!< alias
                typedef typename PointInfo::Type PointType;                        //!< user point type
                typedef typename PointInfo::Core CorePoint;                        //!< matching point[2|3]d<mutable_type>
                static  const    size_t          Dimension = PointInfo::Dimension; //!< [2|3]

                //! internal node, one for each point
                class Node
                {
                public:
                    CorePoint    r;         //!< position
                    CorePoint    t;         //!< tangent
                    CorePoint    n;         //!< normal
                    mutable_type celerity;  //!< |dr/du|
                    mutable_type curvature; //!< curvature

                    inline  Node(const CorePoint &pos) throw() :
                    r(pos), t(), n(), celerity(0), curvature(0) {}     //!< setup
                    inline ~Node() throw() {}                           //!< cleanup
                    inline  Node(const Node &node) throw() :            //|
                    r(node.r), t(node.t), n(node.n),                    //|
                    celerity(node.celerity), curvature(node.curvature)  //|
                    {}                                                  //!< copy

                private:
                    Y_DISABLE_ASSIGN(Node);
                };

                class Segment
                {
                public:
                    const Node *P;
                    const Node *Q;

                    inline Segment(const Node &nodeP, const Node &nodeQ ) throw() :
                    P( &nodeP ), Q( &nodeQ )
                    {
                    }

                    inline  Segment( const Segment &s ) throw() :
                    P(s.P), Q(s.Q) {}
                    inline ~Segment() throw() { }

                    inline CorePoint compute( const_type A, const_type B ) const throw()
                    {
                        const CorePoint & p = P->r;
                        const CorePoint & q = Q->r;
                        return A*p + B*q;
                    }

                private:
                    Y_DISABLE_ASSIGN(Segment);
                };

                typedef vector<Node,ALLOCATOR>    Nodes;    //!< internal vector of nodes type
                typedef vector<Segment,ALLOCATOR> Segments; //!< internal vector of segments type

                Nodes    nodes;                       //!< the nodes
                Segments segments;                    //!< the segments
                inline explicit Curve() : nodes() {}  //!< setup
                inline virtual ~Curve() throw() {}    //!< cleanup

                //! compute tangents, normals and curvatures
                template <typename SEQUENCE>
                inline void compute( const SEQUENCE &points, const Boundaries boundaries )
                {
                    //__________________________________________________________
                    //
                    // acquire resources
                    //__________________________________________________________
                    const size_t n = points.size();
                    segments.free();
                    nodes.free();
                    nodes.ensure(n);
                    segments.ensure(n);

                    //__________________________________________________________
                    //
                    // create nodes with tangents
                    //__________________________________________________________
                    switch(n)
                    {
                        case 0:
                        case 1:  break;
                        case 2:  compute_two(points,boundaries); break;
                        default: compute_all(points,boundaries); break;
                    }
                    assert(nodes.size()==n);

                    //__________________________________________________________
                    //
                    // create normals and curvatures
                    //__________________________________________________________
                    switch(n)
                    {
                        case 0:
                        case 1:
                            break;
                        case 2:  update_two(int2type<Dimension>(),boundaries); break;
                        default: update_all(int2type<Dimension>(),boundaries); break;
                    }

                    //__________________________________________________________
                    //
                    // create segments
                    //__________________________________________________________
                    for(size_t i=1;i<n;++i)
                    {
                        create_segment( nodes[i], nodes[i+1] );
                    }
                    switch (boundaries)
                    {
                        case Periodic: create_segment(nodes[n],nodes[1]); break;
                        default: break;
                    }
                }

                inline PointType get(const_type x) const throw()
                {
                    const size_t n = nodes.size();
                    const size_t s = segments.size();
                    switch(n)
                    {
                        case 0:
                        {
                            const CorePoint origin = PointInfo::Origin();
                            return PointInfo::Core2Type(origin);
                        }
                        case 1: return PointInfo::Core2Type(nodes[1].r);
                        default: break;

                    }

                    const CorePoint c = ( (n==s) ? get_periodic(x,n) : get_standard(x,n));
                    return PointInfo::Core2Type(c);
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curve);

                inline CorePoint get_periodic(mutable_type x, const size_t n) const throw()
                {
                    const_type shift(n);
                    while(x>=n) x-=shift;
                    while(x<=1) x+=shift;
                    const size_t j   = clamp<size_t>(1, floor_of(x), n );
                    const_type   B   = x-const_type(j);
                    const_type   A   = const_type(1)-B;
                    return segments[j].compute(A,B);
                }

                inline CorePoint get_standard(mutable_type x, const size_t n) const throw()
                {
                    if(x<=1)
                    {
                        return nodes[1].r;
                    }
                    else if(x>=n)
                    {
                        return nodes[n].r;
                    }
                    else
                    {
                        const size_t j   = clamp<size_t>(1, floor_of(x), n-1 );
                        const_type   B   = x-const_type(j);
                        const_type   A   = const_type(1)-B;
                        return segments[j].compute(A,B);
                    }
                }

                inline void create_segment(const Node &P, const Node &Q)
                {
                    const Segment segment(P,Q);
                    segments.push_back_(segment);
                }

                //--------------------------------------------------------------
                //
                // computing tangents
                //
                //--------------------------------------------------------------
                template <typename SEQUENCE>
                void compute_two( const SEQUENCE &points, const Boundaries boundaries )
                {
                    const CorePoint A = PointInfo::Type2Core( points.front() );
                    const CorePoint B = PointInfo::Type2Core( points.back()  );
                    const CorePoint AB(A,B);
                    const_type      speed = sqrt_of( CheckNorm2(AB.norm2(),"AB") );
                    Node node(A);
                    node.t        = AB/speed;
                    node.celerity = speed;
                    nodes.push_back_(node);
                    node.r = B;
                    switch(boundaries)
                    {
                        case Standard: nodes.push_back_(node);                   break;
                        case Periodic: node.t = -node.t; nodes.push_back_(node); break;
                    }
                }

                inline void compute_head(const PointType &PA, const PointType &PB, const PointType &PC)
                {
                    const CorePoint A   = PointInfo::Type2Core(PA);
                    const CorePoint B   = PointInfo::Type2Core(PB);
                    const CorePoint C   = PointInfo::Type2Core(PC);
                    const CorePoint TT  = 4*B-(3*A+C);
                    const_type      tt  = sqrt_of(CheckNorm2(TT.norm2(),"head"));
                    Node node(A);
                    node.t        = TT/tt;
                    node.celerity = tt/2;
                    nodes.push_back_(node);
                }

                inline void compute_bulk( const PointType &PA, const PointType &PB, const PointType &PC )
                {
                    const CorePoint A = PointInfo::Type2Core(PA);
                    const CorePoint B  = PointInfo::Type2Core(PB);
                    const CorePoint C = PointInfo::Type2Core(PC);
                    const CorePoint AC(A,C);
                    const_type      ac  = sqrt_of(CheckNorm2(AC.norm2(),"AC"));
                    Node            node(B);
                    node.t        = AC/ac;
                    node.celerity = ac/2;
                    nodes.push_back_(node);
                }

                inline void compute_tail(const PointType &PA, const PointType &PB, const PointType &PC)
                {
                    const CorePoint A   = PointInfo::Type2Core(PA);
                    const CorePoint B   = PointInfo::Type2Core(PB);
                    const CorePoint C   = PointInfo::Type2Core(PC);
                    const CorePoint TT  = (3*C+A)-4*B;
                    const_type      tt  = sqrt_of( CheckNorm2(TT.norm2(),"tail") );
                    Node node(C);
                    node.t        = TT/tt;
                    node.celerity = tt/2;
                    nodes.push_back_(node);
                }

                template <typename SEQUENCE>
                void compute_all( const SEQUENCE &points, const Boundaries boundaries )
                {
                    //__________________________________________________________
                    //
                    // prepare for scanning
                    //__________________________________________________________
                    const size_t   n = points.size(); assert(n>=3);
                    const typename SEQUENCE::const_iterator         head = points.begin();
                    const typename SEQUENCE::const_reverse_iterator tail = points.rbegin();
                    typename       SEQUENCE::const_iterator         prev = head;
                    typename       SEQUENCE::const_iterator         curr = head; ++curr;
                    typename       SEQUENCE::const_iterator         next = curr; ++next;

                    //__________________________________________________________
                    //
                    // head
                    //__________________________________________________________
                    switch(boundaries)
                    {
                        case Periodic: compute_bulk(*tail,*prev,*curr); break;
                        case Standard: compute_head(*prev,*curr,*next); break;
                    }

                    //__________________________________________________________
                    //
                    // bulk
                    //__________________________________________________________
                    for(size_t i=2;i<n;++i,++prev,++curr,++next)
                    {
                        compute_bulk(*prev,*curr,*next);
                    }

                    //__________________________________________________________
                    //
                    // tail
                    //__________________________________________________________
                    switch(boundaries)
                    {
                        case Periodic: compute_bulk(*prev,*curr,*head); break;
                        case Standard: next=prev; --next; compute_tail(*next,*prev,*curr); break;
                    }
                }

                //--------------------------------------------------------------
                //
                // computing normals and curvature
                //
                //--------------------------------------------------------------
                inline void update_two( int2type<2>,  const Boundaries)
                {
                    for(size_t i=2;i>0;--i)
                    {
                        Node &node = nodes[i];
                        node.n = node.t.direct_normal();
                    }
                }

                inline void update_two( int2type<3>, const Boundaries)
                {
                    // nothing to do
                }


                static inline void update2d_head( Node &nA, const Node &nB, const Node &nC )
                {
                    const CorePoint TA = nA.t;
                    const CorePoint TB = nB.t;
                    const CorePoint TC = nC.t;
                    nA.n = TA.direct_normal();
                    const_type      dsdu = nA.celerity;
                    const CorePoint dTds = (4*TB-3*TA-TC)/(dsdu+dsdu);
                    nA.curvature = dTds * nA.n;
                }

                static inline void update2d_bulk( const Node &prev, Node &node, const Node &next )
                {
                    node.n = node.t.direct_normal();
                    const_type      dsdu = node.celerity;
                    const CorePoint dTds = (next.t-prev.t)/(dsdu+dsdu);
                    node.curvature = dTds*node.n;
                }

                static inline void update2d_tail( const Node &nA, const Node &nB, Node &nC )
                {
                    const CorePoint TA = nA.t;
                    const CorePoint TB = nB.t;
                    const CorePoint TC = nC.t;
                    nC.n = TC.direct_normal();
                    const_type      dsdu = nC.celerity; assert(dsdu>0);
                    const CorePoint dTds = (3*TC+TA-4*TB)/(dsdu+dsdu);
                    nC.curvature = dTds * nC.n;
                }

                inline void update_all( int2type<2>, const Boundaries boundaries)
                {
                    const size_t n   = nodes.size();
                    const size_t nm1 = n-1;
                    // head
                    {
                        switch( boundaries )
                        {
                            case Standard: update2d_head(nodes[1],nodes[2],nodes[3]); break;
                            case Periodic: update2d_bulk(nodes[n],nodes[1],nodes[2]); break;
                        }
                    }

                    // bulk
                    for(size_t i=nm1;i>1;--i)
                    {
                        update2d_bulk(nodes[i-1],nodes[i],nodes[i+1]);
                    }

                    // tail
                    {
                        switch( boundaries )
                        {
                            case Standard: update2d_tail(nodes[nm1-1],nodes[nm1],nodes[n]); break;
                            case Periodic: update2d_bulk(nodes[nm1],nodes[n],nodes[1]);   break;
                        }
                    }
                }

                inline void update_all( int2type<3>, const Boundaries boundaries)
                {
                    const size_t n = nodes.size();
                    // head
                    {
                        //Node &node = nodes[1];
                        switch( boundaries )
                        {
                            case Standard: break;
                            case Periodic: break;
                        }
                    }

                    // bulk
                    for(size_t i=n-1;i>1;--i)
                    {
                        //const Node &prev = nodes[i-1];
                        //Node       &node = nodes[i];
                        //const Node &next = nodes[i+1];
                    }

                    // tail
                    {
                        //Node &node = nodes[n];
                        // node.n = node.t.direct_normal();
                        switch( boundaries )
                        {
                            case Standard: break;
                            case Periodic: break;
                        }
                    }
                }

            };

            

        }
        
    }
    
}

#endif

