//! \file
#ifndef Y_EUCLIDEAN_ARC_INCLUDED
#define Y_EUCLIDEAN_ARC_INCLUDED 1

#include "y/math/euclidean/segment.hpp"
#include "y/sequence/vector.hpp"
#include "y/math/kernel/lu.hpp"
#include "y/exception.hpp"

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
                inline type speed(const_type u) const throw()
                {
                    Vertex dP;
                    compute(u,0,&dP,0);
                    return sqrt_of( aliasing::cast<VTX,Vertex>(dP).norm2() );
                }

                //! local acceleration norm
                inline type xlr(const_type u) const throw()
                {
                    Vertex d2P;
                    compute(u,0,0,d2P);
                    return sqrt_of( aliasing::cast<VTX,Vertex>(d2P).norm2() );
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



                void compile()
                {
                    const size_t N = nodes.size(); std::cerr << "compiling for " << N << " nodes" << std::endl;
                    if(N>1)
                    {
                        const size_t         M    = N-1;                  // number of Vectorial Lagrange Multipliers
                        vector<Vertex>       lambda(N,as_capacity);      // Vectorial Lagrange Multipliers

                        //------------------------------------------------------
                        // compute all the lambda vectors
                        //------------------------------------------------------
                        {
                            const size_t         dof = Dimensions * M;    // number of Scalar Lagrange Mutlipliers
                            vector<mutable_type> rhs(dof,0);              // Scalar Lagrange Mutlipliers

                            //--------------------------------------------------
                            // compute the rhs (xxxx..yyyy.zzzz)
                            //--------------------------------------------------
                            {
                                //----------------------------------------------
                                // compute the matrix of constraints
                                //----------------------------------------------
                                matrix<mutable_type> alpha(M,M);
                                for(size_t k=M,kp=N;k>0;--k,--kp)
                                {
                                    array<mutable_type> &alpha_k = alpha[k];
                                    for(size_t j=M,jp=N;j>0;--j,--jp)
                                    {
                                        alpha_k[j] = ( (_delta(k,j)+_delta(kp,jp)) - (_delta(k,jp)+_delta(kp,j) ) );
                                    }
                                }
                                //std::cerr << "alpha=" << alpha << std::endl;
                                if(!LU::build(alpha))
                                {
                                    throw exception("Euclidean::Arc: unexpected singularity!!!");
                                }

                                //----------------------------------------------
                                // gather all consecutive coordinates
                                // of Langrange mutlipliers
                                //----------------------------------------------
                                {
                                    size_t          idof[Dimensions] = {1};
                                    for(size_t dim=1;dim<Dimensions;++dim) { idof[dim] = idof[dim-1]+M; }
                                    for(size_t k=1,kp=2;k<N;++k,++kp)
                                    {
                                        NodeType     &N0 = aliasing::_(*nodes[k] );
                                        NodeType     &N1 = aliasing::_(*nodes[kp]);
                                        const Vertex &P0 = N0.point->position;
                                        const Vertex &P1 = N1.point->position;
                                        const Vertex &V0 = N0.V;
                                        const Vertex &V1 = N1.V;
                                        const Vertex D   = 12*(P1-P0) - 6*(V1+V0);
                                        //std::cerr << "\tD" << k << "=" << D << std::endl;
                                        const_type  *d   = (const_type *)&D;
                                        for(size_t   dim=0; dim<Dimensions;++dim )
                                        {
                                            rhs[ idof[dim]++ ] = d[dim];
                                        }
                                    }
                                }
                                //std::cerr << "rhs=" << rhs << std::endl;

                                //----------------------------------------------
                                // solve per dimensions
                                //----------------------------------------------
                                {
                                    size_t ibase = 1;
                                    for(size_t dim=0;dim<Dimensions;++dim, ibase += M)
                                    {
                                        lightweight_array<mutable_type> arr( &rhs[ibase], M );
                                        LU::solve(alpha,arr);
                                    }
                                }
                                //std::cerr << "lam=" << rhs << std::endl;
                            }

                            //--------------------------------------------------
                            // scatter rhs to lambda
                            //--------------------------------------------------
                            {
                                size_t          idof[Dimensions] = {1};
                                for(size_t dim=1;dim<Dimensions;++dim) { idof[dim] = idof[dim-1]+M; }
                                for(size_t k=1;k<N;++k)
                                {
                                    Vertex        lam;
                                    mutable_type *l = (mutable_type *)&lam;
                                    for(size_t dim=0;dim<Dimensions;++dim)
                                    {
                                        l[dim] = rhs[ idof[dim]++ ];
                                    }
                                    lambda.push_back_(lam);
                                }
                            }
                        }
                        //std::cerr << "lambda=" << lambda << std::endl;

                        //------------------------------------------------------
                        // recomposing accelerations
                        //------------------------------------------------------
                        for(size_t k=1;k<=N;++k)
                        {
                            Vertex Ak;
                            for(size_t j=M;j>0;--j)
                            {
                                if(k==j)
                                {
                                    Ak += lambda[j];
                                }
                                else if(k==j+1)
                                {
                                    Ak -= lambda[j];
                                }
                            }
                            aliasing::_(nodes[k]->A) = Ak;
                            //std::cerr << "\tA[" << k << "]=" << Ak << std::endl;
                        }

                        //------------------------------------------------------
                        // finalizing metrics
                        //------------------------------------------------------
                        for(size_t i=segments.size();i>0;--i)
                        {
                            SegmentType    &segm = aliasing::_(*(segments[i]));
                            NodeType       &curr = aliasing::_(*segm.tail);
                            const NodeType &next = *segm.head;
                            const Vertex   &V0   = curr.V;
                            const Vertex   &V1   = next.V;
                            const Vertex   &A0   = curr.A;
                            const Vertex   &A1   = next.A;

                            aliasing::_(curr.Q) = 6*(V1-V0) - 3*(A1+A0);
                            aliasing::_(curr.B) = A1;
                        }

                        for(size_t k=1;k<=N;++k)
                        {
                            const NodeType &node = *nodes[k];
                            std::cerr << "node[" << k << "] : A=" << node.A << ", B=" << node.B << ", Q=" << node.Q << std::endl;
                        }

                    }

                }


            protected:
                //! setup
                inline explicit Arc() throw() : nodes(), segments() {}


                static inline int _delta(const size_t i, const size_t j) throw()
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

