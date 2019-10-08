

//! \file
#ifndef Y_EUCLIDEAN_ARC_PERIODIC_INCLUDED
#define Y_EUCLIDEAN_ARC_PERIODIC_INCLUDED 1

#include "y/math/euclidean/arc.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {


            template <typename T, template <class> class VTX>
            class PeriodicArc : public Arc<T,VTX>
            {
            public:
                Y_EUCLIDEAN_ARC_TYPES();
                inline explicit PeriodicArc() throw() {}
                inline virtual ~PeriodicArc() throw() {}

                inline virtual void ensure(size_t n)
                {
                    aliasing::_(this->nodes).ensure(n);
                    aliasing::_(this->segments).ensure(n);
                }


                virtual void motion(const ArcClass C) throw()
                {
                    Nodes       &nds = aliasing::_(this->nodes);
                    const size_t num = nds.size();

                    switch(num)
                    {
                            // no single
                        case 0: break;

                            // single node
                        case 1: nds.front()->reset(); break;

                            // only two nodes
                        case 2: {
                            NodeType &N0 = *nds.front();
                            NodeType &N1 = *nds.back();
                            N0.reset();
                            N1.reset();
                            aliasing::_(N1.V) = -(aliasing::_(N0.V)  = N1.P - N0.P);

                        } break;

                        default: {
                            this->motionBulkFor( *nds[num],  *nds[1],  *nds[2], C );
                            this->motionBulk(C);
                            this->motionBulkFor( *nds[num-1],*nds[num],*nds[1], C );
                        }
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


            };


        }

    }

}

#endif
