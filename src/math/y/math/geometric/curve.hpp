//! \file
#ifndef Y_GEOMETRIC_CURVE_INCLUDED
#define Y_GEOMETRIC_CURVE_INCLUDED 1

#include "y/math/geometric/points.hpp"
#include "y/type/bzset.hpp"

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
                //______________________________________________________________
                //
                // aliases
                //______________________________________________________________
                Y_DECL_ARGS(T,type);                                               //!< aliases
                typedef PointInfoFor<T,POINT>    PointInfo;                        //!< alias
                typedef typename PointInfo::Type PointType;                        //!< user point type
                typedef typename PointInfo::Core CorePoint;                        //!< matching point[2|3]d<mutable_type>
                static  const    size_t          Dimension = PointInfo::Dimension; //!< [2|3]

                //______________________________________________________________
                //
                // internal type definitions
                //______________________________________________________________

                //! internal node, one for each point
                class Node
                {
                public:
                    CorePoint    r;         //!< position
                    CorePoint    t;         //!< tangent
                    CorePoint    n;         //!< normal
                    CorePoint    speed;     //!< celerity*n
                    mutable_type celerity;  //!< |dr/du|
                    mutable_type curvature; //!< curvature

                    inline  Node(const CorePoint &pos) throw() :
                    r(pos), t(), n(), speed(),
                    celerity(0), curvature(0) {}                         //!< setup
                    inline ~Node() throw() {}                            //!< cleanup
                    inline  Node(const Node &node) throw() :             //|
                    r(node.r), t(node.t), n(node.n), speed(node.speed),  //|
                    celerity(node.celerity), curvature(node.curvature)   //|
                    {}                                                   //!< copy
                    inline void finalize() throw()                       //!< adjust
                    {
                        speed = celerity * t;
                    }
                private:
                    Y_DISABLE_ASSIGN(Node);
                };

                //! internal segment between pair of nodes
                class Segment
                {
                public:
                    const Node *P; //!< head node
                    const Node *Q; //!< tail node
                    const CorePoint PQ;
                    const CorePoint U; //!< internal value
                    const CorePoint V; //!< internal value

                    //! prepare segment
                    inline Segment(const Node &nodeP, const Node &nodeQ ) throw() :
                    P( &nodeP ), Q( &nodeQ ),
                    PQ(P->r,Q->r), U(), V()
                    {
                        const CorePoint rhs1(P->speed,PQ); assert( P->speed.norm2()>0 );
                        const CorePoint rhs2(PQ,Q->speed); assert( Q->speed.norm2()>0 );
                        (CorePoint &)U = 2 * ( (rhs1+rhs1) - rhs2 );
                        (CorePoint &)V = 2 * ( (rhs2+rhs2) - rhs1 );
                    }

                    //! copy
                    inline  Segment( const Segment &s ) throw() :
                    P(s.P), Q(s.Q), PQ(s.PQ), U(s.U), V(s.V) {}

                    //! destruct
                    inline ~Segment() throw() { }

                    inline void compute(const_type A, const_type B, CorePoint *M, CorePoint *S) const throw()
                    {
                        static const_type one       = T(1);
                        static const_type one_sixth = T(1)/6;
                        const_type        A2 = A*A;
                        const_type        B2 = B*B;
                        if(M)   *M = A * (P->r) + B * (Q->r) + one_sixth * ( A*(A2-one)*U + B*(B2-one)*V );
                        //if(S)   *S = PQ + one_sixth * ( (one-3*A2) * U + (3*B2-one) * V );
                        if(S)   *S = A * (P->speed) + B * (Q->speed);

                    }


                private:
                    Y_DISABLE_ASSIGN(Segment);
                };

                typedef vector<Node,ALLOCATOR>    Nodes;    //!< internal vector of nodes type
                typedef vector<Segment,ALLOCATOR> Segments; //!< internal vector of segments type
                typedef void (Curve::*get_proc)(type,CorePoint*,CorePoint*) const; //!< compute interface

                //______________________________________________________________
                //
                // data
                //______________________________________________________________
                Nodes    nodes;                             //!< the nodes
                Segments segments;                          //!< the segments
                get_proc get_addr;                          //!< address of get function

                //______________________________________________________________
                //
                // methods
                //______________________________________________________________
                inline explicit Curve() :
                nodes(),segments(),get_addr(0) {}          //!< setup
                inline virtual ~Curve() throw() {}          //!< cleanup

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
                    get_addr = 0;



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
                    for(size_t i=n;i>0;--i)
                    {
                        nodes[i].finalize();
                    }
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

                    //__________________________________________________________
                    //
                    // internal linking
                    //__________________________________________________________
                    switch (boundaries)
                    {
                        case Periodic: get_addr = & Curve::get_periodic; break;
                        case Standard: get_addr = & Curve::get_standard; break;
                    }

                }

                //! interpolation
                inline void get(const_type x, PointType *M, PointType *S) const throw()
                {
                    static const int none = 0x00;
                    static const int hasM = 0x01;
                    static const int hasS = 0x02;
                    static const int both = hasM | hasS;
                    const size_t n = nodes.size();

                    int   flag  = none;
                    if(M) flag |= hasM;
                    if(S) flag |= hasS;

                    switch(n)
                    {
                        case 0: {
                            switch(flag)
                            {
                                case hasM : bzset(*M);            break;
                                case hasS : bzset(*S);            break;
                                case both : bzset(*M); bzset(*S); break;
                                default: break;
                            }
                        } break;

                        case 1: {
                            switch(flag)
                            {
                                case hasM: *M = PointInfo::Core2Type(nodes[1].r);           break;
                                case hasS: bzset(*S);                                       break;
                                case both: bzset(*S);*M = PointInfo::Core2Type(nodes[1].r); break;
                            }
                        }  break;

                        default: break;

                    }
                    assert(get_addr);

                    switch(flag)
                    {
                        case hasM: {
                            CorePoint m; ((*this).*get_addr)(x,&m,0); *M = PointInfo::Core2Type(m);
                        } break;

                        case hasS: {
                            CorePoint s; ((*this).*get_addr)(x,0,&s); *S = PointInfo::Core2Type(s);
                        } break;

                        case both: {
                            CorePoint m;
                            CorePoint s;
                            ((*this).*get_addr)(x,&m,&s);
                            *M = PointInfo::Core2Type(m);
                            *S = PointInfo::Core2Type(s);
                        } break;
                    }
                }

                //! get position only
                inline PointType position(const_type x, PointType *S=NULL) const throw()
                {
                    PointType M; get(x,&M,S); return M;
                }

                //! get speed only
                inline PointType speed(const_type x, PointType *M=NULL) const throw()
                {
                    PointType S; get(x,M,&S); return S;
                }



            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curve);

                inline void get_periodic(mutable_type x, CorePoint *M, CorePoint *S) const throw()
                {
                    const size_t n = segments.size();
                    const_type shift(n);
                    while(x>=n) x-=shift;
                    while(x<=1) x+=shift;
                    const size_t j   = clamp<size_t>(1, floor_of(x), n );
                    const_type   B   = x-const_type(j);
                    const_type   A   = const_type(1)-B;
                    segments[j].compute (A,B,M,S);
                }

                inline void get_standard(mutable_type x, CorePoint *M, CorePoint *S) const throw()
                {
                    const size_t n = nodes.size();
                    assert(n-1==segments.size());
                    if(x<=1)
                    {
                        if(M) *M = nodes[1].r;
                        if(S) *S = nodes[1].speed;
                    }
                    else if(x>=n)
                    {
                        if(M) *M = nodes[n].r;
                        if(S) *S = nodes[n].speed;
                    }
                    else
                    {
                        const size_t j   = clamp<size_t>(1, floor_of(x),segments.size());
                        const_type   B   = x-const_type(j);
                        const_type   A   = const_type(1)-B;
                        segments[j].compute(A,B,M,S);
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

