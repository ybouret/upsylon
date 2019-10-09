
//! \file
#ifndef Y_EUCLIDEAN_ARC_STANDARD_INCLUDED
#define Y_EUCLIDEAN_ARC_STANDARD_INCLUDED 1

#include "y/math/euclidean/arc.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //
            //! standard arc
            //
            //
            //==================================================================
            template <typename T, template <class> class VTX>
            class StandardArc : public Arc<T,VTX>
            {
            public:
                Y_EUCLIDEAN_ARC_TYPES();                    //!< aliases
                inline explicit StandardArc() throw() {}    //!< setup
                inline virtual ~StandardArc() throw() {}    //!< cleanup


                //! ensure one less segment than nodes
                inline virtual void ensure(size_t n)
                {
                    aliasing::_(this->nodes).ensure(n);
                    if(n>1)
                    {
                        aliasing::_(this->segments).ensure(n-1);
                    }
                }


                //! interpolation function
                virtual void compute( mutable_type u, vertex *p, vertex *dp, vertex *d2p ) const throw()
                {
                    const Nodes &nds = this->nodes;
                    const size_t num = nds.size();
                    switch(num)
                    {
                        case 0:
                            Y_EUCLIDEAN_XZERO(p);
                            Y_EUCLIDEAN_XZERO(dp);
                            Y_EUCLIDEAN_XZERO(d2p);
                            break;
                            
                        case 1:
                            if(p) { *p = nds.front()->P; }
                            Y_EUCLIDEAN_XZERO(dp);
                            Y_EUCLIDEAN_XZERO(d2p);
                            break;

                        default:
                            if(u<=1)
                            {
                                const NodeType &node = *nds.front();
                                Y_EUCLIDEAN_XCOPY(p,  node.P);
                                Y_EUCLIDEAN_XCOPY(dp, node.V);
                                Y_EUCLIDEAN_XCOPY(d2p,node.A);
                            }
                            else
                            {
                                if(u>=num)
                                {
                                    const NodeType &node = *nds.back();
                                    Y_EUCLIDEAN_XCOPY(p,  node.P);
                                    Y_EUCLIDEAN_XCOPY(dp, node.V);
                                    Y_EUCLIDEAN_XCOPY(d2p,node.A);
                                }
                                else
                                {
                                    const size_t    indx = clamp<size_t>(1,floor_of(u),num-1);
                                    const NodeType &node = *nds[indx];
                                    return (node.*(this->onCompute))(u-indx,p,dp,d2p);
                                }
                            }
                            break;
                    }

                }

            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);

                inline virtual void add(const SharedPoint &p )
                {
                    this->pushBack(p);
                    if(this->nodes.size()>1)
                    {
                        try { this->pushGrowing(); }
                        catch(...){
                            this->popBack();
                            throw;
                        }
                    }
                    assert(this->segments.size()==this->nodes.size()-1);
                }
                

                //! even for Arc0, an estimate of V is done
                virtual void kinematics() throw()
                {
                    static const_type half(0.5);
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size();

                    for(size_t i=num;i>0;--i) nds[i]->reset();

                    switch(num)
                    {
                            // no single
                        case 0: break;

                            // single node
                        case 1:  ; break;

                            // only two nodes
                        case 2: {
                            NodeType &N0 = *nds.front();
                            NodeType &N1 = *nds.back();
                            aliasing::_(N1.V) = aliasing::_(N0.V)  = (N1.P - N0.P);
                        } break;

                        default: {
                            assert(num>=3);
                            // head
                            {
                                NodeType     &N1  = *nds[1];
                                const_vertex &P1  = N1.P;
                                const_vertex &P2  = nds[2]->P;
                                const_vertex &P3  = nds[3]->P;

                                aliasing::_(N1.A) = (P3-P2)+(P1-P2);
                                aliasing::_(N1.V) = half*(4*P2-(3*P1+P3));
                            }

                            // bulk
                            this->motionBulk();

                            // tail
                            {
                                NodeType     &Nend  = *nds[num];
                                const_vertex &Pnm0  = Nend.P;
                                const_vertex &Pnm1  = nds[num-1]->P;
                                const_vertex &Pnm2  = nds[num-2]->P;

                                aliasing::_(Nend.A) = (Pnm0-Pnm1)+(Pnm2-Pnm1);
                                aliasing::_(Nend.V) = half*(3*Pnm0+Pnm2-4*Pnm1);
                            }


                        }
                    }

                }

                virtual vertex dT(const size_t n) const throw()
                {
                    static const_type half(0.5);
                    const Nodes      &nds = this->nodes;
                    const size_t      num = nds.size();
                    assert(n>=1);
                    assert(n<=num);
                    assert(num>=3);
                    if(n<=1)
                    {
                        const_vertex &T1 = nds[1]->basis.t;
                        const_vertex &T2 = nds[2]->basis.t;
                        const_vertex &T3 = nds[3]->basis.t;
                        return half*(4*T2 - (3*T1+T3));
                    }
                    else if(n>=num)
                    {
                        const_vertex &T1 = nds[num  ]->basis.t;
                        const_vertex &T2 = nds[num-1]->basis.t;
                        const_vertex &T3 = nds[num-2]->basis.t;
                        return half*((3*T1+T3)-4*T2);
                    }
                    else
                    {
                        return half*(nds[n+1]->basis.t-nds[n-1]->basis.t);
                    }
                }
                
            };

        }

    }

}

#endif
