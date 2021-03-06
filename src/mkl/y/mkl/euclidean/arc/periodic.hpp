

//! \file
#ifndef Y_MKL_EUCLIDEAN_ARC_PERIODIC_INCLUDED
#define Y_MKL_EUCLIDEAN_ARC_PERIODIC_INCLUDED 1

#include "y/mkl/euclidean/arc.hpp"

namespace upsylon {

    namespace mkl {

        namespace Euclidean {


            //==================================================================
            //
            //
            //! periodic arc
            //
            //
            //==================================================================
            template <typename T, template <class> class VTX>
            class PeriodicArc : public Arc<T,VTX>
            {
            public:
                Y_EUCLIDEAN_ARC_TYPES();                  //!< aliases
                inline explicit PeriodicArc() throw() {}  //!< setup
                inline virtual ~PeriodicArc() throw() {}  //!< cleanup

                //! ensure same number of points and segments
                inline virtual void ensure(size_t n)
                {
                    aliasing::_(this->nodes).ensure(n);
                    aliasing::_(this->segments).ensure(n);
                }

                //! interpolation [1:nodes.size()+1]
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
                            Y_EUCLIDEAN_XZERO(dp);
                            Y_EUCLIDEAN_XZERO(d2p);
                            if(p)
                            {
                                *p = nds[1]->P;
                            }
                            break;

                        default:{
                            mutable_type i = floor_of(u);
                            u-=i;
                            while(i<1)   i+=num;
                            while(i>num) i-=num;
                            const NodeType &node = *(this->nodes[clamp<size_t>(1,size_t(i),num)]);
                            return (node.*(this->onCompute))(u,p,dp,d2p);
                        } break;
                    }
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(PeriodicArc);
                virtual void add(const SharedPoint &p )
                {
                    switch(this->nodes.size())
                    {

                            // first point
                        case 0: this->pushBack(p); break;

                            // first segments
                        case 1: this->pushBack(p);
                            assert(2==this->nodes.size());
                            try {
                                this->pushGrowing();
                                this->pushClosing();
                            } catch(...) {
                                aliasing::_(this->segments).free();
                                this->popBack();
                                throw;
                            }
                            assert(this->segments.size()==this->nodes.size());
                            break;

                        default: assert(this->nodes.size()>=2);
                            this->pushBack(p); {
                                size_t count = 0;
                                try {
                                    this->pushGrowing(); ++count;
                                    this->pushClosing(); ++count;
                                } catch(...) {
                                    this->popSegments(count);
                                    this->popBack();
                                    throw;
                                }
                            }
                            
                            aliasing::_(this->segments).remove_at(this->segments.size()-2);
                            assert(this->segments.size()==this->nodes.size());
                    }
                }
                

                virtual void kinematics() throw()
                {
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size();

                    for(size_t i=num;i>0;--i) nds[i]->reset();

                    switch(num)
                    {
                            // no single
                        case 0: break;

                            // single node
                        case 1:  break;

                            // only two nodes
                        case 2: {
                            NodeType &N0 = *nds.front();
                            NodeType &N1 = *nds.back();
                            aliasing::_(N1.V) = -(aliasing::_(N0.V)  = (N1.P - N0.P) );

                        } break;

                        default: {
                            nds[1]->reset();
                            this->motionBulkFor( *nds[num],  *nds[1],  *nds[2]);
                            this->motionBulk();
                            nds[num]->reset();
                            this->motionBulkFor( *nds[num-1],*nds[num],*nds[1]);
                        }
                    }
                }

                virtual vertex dT(const size_t n) const throw()
                {
                    static const_type half(0.5);
                    const Nodes &nds = this->nodes;
                    const size_t num = nds.size();
                    assert(n>=1);
                    assert(n<=num);
                    assert(num>=3);
                    if(n<=1)
                    {
                        return half*(nds[2]->basis.t-nds[num]->basis.t);
                    }
                    else if(n>=num)
                    {
                        return half*(nds[1]->basis.t-nds[num-1]->basis.t);
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
