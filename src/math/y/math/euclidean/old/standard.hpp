//! \file
#ifndef Y_EUCLIDEAN_STANDARD_ARC_INCLUDED
#define Y_EUCLIDEAN_STANDARD_ARC_INCLUDED 1

#include "y/math/euclidean/arc.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            //==================================================================
            //
            //
            //! Standard Arc
            //
            //
            //==================================================================
            template <typename T, template <class> class POINT>
            class StandardArc : public Arc<T,POINT>
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
                // methods
                //
                //==============================================================

                //! cleanup
                inline virtual ~StandardArc() throw() {}

                //! setup
                inline explicit StandardArc() throw() : ArcType() {}

                //! acquire memory
                inline virtual void ensure(const size_t numNodes)
                {
                    if(numNodes>0)
                    {
                        aliasing::_(this->nodes   ).ensure(numNodes  );
                        aliasing::_(this->segments).ensure(numNodes-1);
                    }
                }

                
                //! compute local standard metrics
                inline virtual void metrics() throw()
                {
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size();
                    
                    // compute tangents
                    switch(num)
                    {
                        case 0: break;
                        case 1: nds.front()->setFixed(); break;
                        case 2: {
                            const Vertex delta = nds.back()->point->position - nds.front()->point->position;
                            nds.front()->setCelerity( delta );
                            nds.back()->setCelerity(  delta );
                        } break;
                        default: {
                            assert(nds.size()>=3);
                            static const_type half(0.5);
                            static const_type four(4);
                            static const_type three(3);
                            {
                                const Vertex &P0 = nds[1]->point->position;
                                const Vertex &P1 = nds[2]->point->position;
                                const Vertex &P2 = nds[3]->point->position;
                                nds.front()->setCelerity( half*( four * P1 - (P2+three*P0 )) );
                            }
                            for(size_t i=num-1;i>1;--i)
                            {
                                nds[i]->setCelerity( half*(nds[i+1]->point->position-nds[i-1]->point->position) );
                            }
                            {
                                const Vertex &P0 = nds[num-0]->point->position;
                                const Vertex &P1 = nds[num-1]->point->position;
                                const Vertex &P2 = nds[num-2]->point->position;
                                nds.back()->setCelerity( half*(  (P2+three*P0 ) - four * P1 ) );
                            }
                        } break;
                    }
                    
                    // compute normals
                    this->computeNormals2D(int2type<Dimensions>());
                    if(num>=3)
                    {
                        this->computeNormals3D(int2type<Dimensions>());
                    }

                }

                virtual void compute( mutable_type u, Vertex *P, Vertex *dP, Vertex *d2P ) const throw()
                {
                    const size_t num = this->nodes.size();
                    switch(num)
                    {
                        case 0:
                            Y_EUCLIDEAN_XZERO(P);
                            Y_EUCLIDEAN_XZERO(dP);
                            Y_EUCLIDEAN_XZERO(d2P);
                            break;
                        case 1:
                            Y_EUCLIDEAN_XCOPY(P,this->nodes.front()->point->position);
                            Y_EUCLIDEAN_XZERO(dP);
                            Y_EUCLIDEAN_XZERO(d2P);
                            break;

                        default:
                            if(u<=1)
                            {
                                const NodeType &node = *(this->nodes.front());
                                Y_EUCLIDEAN_XCOPY(P,node.point->position);
                                Y_EUCLIDEAN_XCOPY(dP,node.V);
                                Y_EUCLIDEAN_XCOPY(d2P,node.A);
                            }
                            else
                            {
                                if(u>=num)
                                {
                                    const NodeType &node = *(this->nodes.back());
                                    Y_EUCLIDEAN_XCOPY(P,node.point->position);
                                    Y_EUCLIDEAN_XCOPY(dP,node.V);
                                    Y_EUCLIDEAN_XCOPY(d2P,node.A);
                                }
                                else
                                {
                                    const size_t    indx = clamp<size_t>(1,floor_of(u),num-1);
                                    const NodeType &node = *(this->nodes[indx]);
                                    //std::cerr << "du=" << u-indx << std::endl;
                                    node.compute(u-indx,P,dP,d2P);
                                }
                            }
                            break;
                    }

                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(StandardArc);
                

                virtual void add( const SharedPoint &p )
                {
                    switch( this->nodes.size() )
                    {
                        case 0: this->pushBack(p); break;
                        default:
                            this->pushBack(p);
                            try {
                                this->pushGrowing();
                            } catch(...) {
                                this->popBack();
                            } break;
                    }

#if !defined(NDEBUG)
                    // sanity checking
                    if( this->nodes.size() > 0 )
                    {
                        assert(this->nodes.size()-1 == this->segments.size());
                        for(size_t i=1;i<this->segments.size();++i)
                        {
                            const SegmentType &s = *(this->segments[i]);
                            assert(s.tail->uuid==this->nodes[i]->uuid);
                            assert(s.head->uuid==this->nodes[i+1]->uuid);

                        }
                    }
#endif
                }
                
                inline void computeNormals3D( int2type<2> ) throw()
                {
                }
                
                inline void computeNormals3D( int2type<3> ) throw()
                {
                    static const_type half(0.5);
                    static const_type four(4);
                    static const_type three(3);
                    
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size(); assert(num>=3);
                    {
                        const Vertex &P0 = nds[1]->basis.t;
                        const Vertex &P1 = nds[2]->basis.t;
                        const Vertex &P2 = nds[3]->basis.t;
                        const Vertex  N  = half*( four * P1 - (P2+three*P0 ));
                        nds[1]->finalize3D(N);
                    }
                    for(size_t i=num-1;i>1;--i)
                    {
                        const Vertex  N  = half*(nds[i+1]->basis.t-nds[i-1]->basis.t);
                        nds[i]->finalize3D(N);
                    }
                    {
                        const Vertex &P0 = nds[num-0]->basis.t;
                        const Vertex &P1 = nds[num-1]->basis.t;
                        const Vertex &P2 = nds[num-2]->basis.t;
                        const Vertex  N  = half*(  (P2+three*P0 ) - four * P1 );
                        nds[num]->finalize3D(N);
                    }
                    
                }
                
                
               
            



            };

        }

    }

}

#endif


