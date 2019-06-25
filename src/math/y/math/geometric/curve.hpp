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
                    mutable_type speed;     //!< |dr/du|
                    mutable_type curvature; //!< curvature

                    inline  Node(const CorePoint &pos) throw() :
                    r(pos), t(), n(), speed(0), curvature(0) {}     //!< setup
                    inline ~Node() throw() {}                       //!< cleanup
                    inline  Node(const Node &node) throw() :        //|
                    r(node.r), t(node.t), n(node.n),                //|
                    speed(node.speed), curvature(node.curvature)    //|
                    {}                                              //!< copy

                private:
                    Y_DISABLE_ASSIGN(Node);
                };

                typedef vector<Node,ALLOCATOR> Nodes; //!< internal vector of nodes type
                Nodes nodes;                          //!< the nodes

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
                    nodes.free();
                    nodes.ensure(n);

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
                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Curve);

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
                    node.t     = AB/speed;
                    node.speed = speed;
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
                    node.t     = TT/tt;
                    node.speed = tt/2;
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
                    node.t     = AC/ac;
                    node.speed = ac/2;
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
                    node.t     = TT/tt;
                    node.speed = tt/2;
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
                    const_type      dsdu = nA.speed;
                    const CorePoint dTds = (4*TB-3*TA-TC)/(dsdu+dsdu);
                    nA.curvature = dTds * nA.n;
                }

                static inline void update2d_bulk( const Node &prev, Node &node, const Node &next )
                {
                    node.n = node.t.direct_normal();
                    const_type      dsdu = node.speed;
                    const CorePoint dTds = (next.t-prev.t)/(dsdu+dsdu);
                    node.curvature = dTds*node.n;
                }

                static inline void update2d_tail( const Node &nA, const Node &nB, Node &nC )
                {
                    const CorePoint TA = nA.t;
                    const CorePoint TB = nB.t;
                    const CorePoint TC = nC.t;
                    nC.n = TC.direct_normal();
                    const_type      dsdu = nC.speed; assert(dsdu>0);
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

