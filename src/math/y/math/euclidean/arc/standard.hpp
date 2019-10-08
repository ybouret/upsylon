
//! \file
#ifndef Y_EUCLIDEAN_ARC_STANDARD_INCLUDED
#define Y_EUCLIDEAN_ARC_STANDARD_INCLUDED 1

#include "y/math/euclidean/arc.hpp"

namespace upsylon {

    namespace math {

        namespace Euclidean {

            template <typename T, template <class> class VTX>
            class StandardArc : public Arc<T,VTX>
            {
            public:
                Y_EUCLIDEAN_ARC_TYPES();
                inline explicit StandardArc() throw() {}
                inline virtual ~StandardArc() throw() {}

                inline virtual void ensure(size_t n)
                {
                    aliasing::_(this->nodes).ensure(n);
                    if(n>1)
                    {
                        aliasing::_(this->segments).ensure(n-1);
                    }
                }

                virtual void motion(const ArcClass C) throw()
                {
                    static const_type half(0.5);
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
                            aliasing::_(N1.V) = aliasing::_(N0.V)  = N1.P - N0.P;

                        } break;

                        default: {
                            assert(num>=3);
                            // head
                            {
                                NodeType     &N1 = *nds[1]; N1.reset();
                                const_vertex &P1 = N1.P;
                                const_vertex &P2 = nds[2]->P;
                                const_vertex &P3 = nds[3]->P;
                                switch(C)
                                {
                                    case Arc2: aliasing::_(N1.A) = (P3-P2)+(P1-P2);
                                    case Arc1: aliasing::_(N1.V) = half*(4*P2-(3*P1+P3));
                                    case Arc0: break;
                                }
                            }

                            // bulk
                            this->motionBulk(C);

                            // tail
                            {
                                NodeType     &Nend = *nds[num]; Nend.reset();
                                const_vertex &Pnm0 = Nend.P;
                                const_vertex &Pnm1 = nds[num-1]->P;
                                const_vertex &Pnm2 = nds[num-2]->P;
                                switch(C)
                                {
                                    case Arc2: aliasing::_(Nend.A) = (Pnm0-Pnm1)+(Pnm2-Pnm1);
                                    case Arc1: aliasing::_(Nend.V) = half*(3*Pnm0+Pnm2-4*Pnm1);
                                    case Arc0: break;
                                }
                            }


                        }
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



            };

        }

    }

}

#endif
