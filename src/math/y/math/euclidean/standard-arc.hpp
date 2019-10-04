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
                Y_EUCLIDEAN_ARC_ARGS();

                inline virtual ~StandardArc() throw() {}
                inline explicit StandardArc() throw() : ArcType() {}

                inline virtual void ensure(const size_t numNodes)
                {
                    if(numNodes>0)
                    {
                        aliasing::_(this->nodes   ).ensure(numNodes  );
                        aliasing::_(this->segments).ensure(numNodes-1);
                    }
                }

                inline virtual void celerities() throw()
                {
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size();
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



            };

        }

    }

}

#endif


