

#include "y/chemical/sys/stream.hpp"

////////////////////////////////////////////////////////////////////////////////
//
//
// EDGE
//
//
////////////////////////////////////////////////////////////////////////////////
namespace upsylon
{
    namespace Chemical
    {

        namespace Stream
        {
            Edge:: ~Edge() throw() {}

            Edge:: Edge(const Course c, const unit_t n) throw() :
            course(c),
            weight(n)
            {
            }

        }

    }

}

////////////////////////////////////////////////////////////////////////////////
//
//
// GRAPH
//
//
////////////////////////////////////////////////////////////////////////////////

#include "y/ios/ocstream.hpp"
#include "y/ios/tools/graphviz.hpp"

namespace upsylon
{
    namespace Chemical
    {

        namespace Stream
        {

            Graph:: ~Graph() throw()
            {
            }

            template <typename LIST, typename ARRAY> static inline
            void buildList(LIST &L, const ARRAY &A)
            {
                for(size_t i=A.size();i>0;--i)
                {
                    aliasing::_(L).push_front( new typename LIST::node_type( *A[i] ) );
                }
            }


            Graph:: Graph(const Lineage::Array &lineage,
                          const Primary::Array &primary) :
            lineageVertices(),
            primaryVertices()
            {
                // register all species and equilibrium
                buildList(lineageVertices,lineage);
                buildList(primaryVertices,primary);

                // build all connections
                for(const PrimaryVertex *primaryVtx=primaryVertices.head;primaryVtx;primaryVtx=primaryVtx->next)
                {
                    const PrimaryVertex &P = *primaryVtx;
                    const Equilibrium   &E = **P;
                    for(const LineageVertex *lineageVtx=lineageVertices.head;lineageVtx;lineageVtx=lineageVtx->next)
                    {
                        const LineageVertex &L  = *lineageVtx;
                        const Species       &S  = **L;
                        const unit_t         nu = E.stoichiometry(S);
                        if(nu<0)
                        {
                            // reactant  : forward incoming and reverse outgoing
                            aliasing::_(forwardIncomingEdges).push_back( new ForwardIncoming(L,-nu,P) );
                            aliasing::_(reverseOutgoingEdges).push_back( new ReverseOutgoing(P,-nu,L) );
                        }
                        else
                        {
                            if(nu>0)
                            {
                                // product : forward outgoing and reverse incoming
                                aliasing::_(forwardOutgoingEdges).push_back( new ForwardOutgoing(P,nu,L) );
                                aliasing::_(reverseIncomingEdges).push_back( new ReverseIncoming(L,nu,P) );
                            }
                        }
                    }
                }

            }

            template <typename LIST>
            static inline void graphVizList(ios::ostream &fp, const LIST &L)
            {
                for(const typename LIST::node_type *v=L.head;v;v=v->next)
                {
                    (*v)->vizSave(fp);
                }
            }

            void Graph:: graphViz(const string &fileName) const
            {
                {
                    ios::ocstream fp(fileName);

                    Vizible::enterDigraph(fp,"G");
                    graphVizList(fp,lineageVertices);
                    graphVizList(fp,primaryVertices);
                    Vizible::leaveDigraph(fp);

                }

                ios::GraphViz::Render(fileName);

            }

        }

    }

}
