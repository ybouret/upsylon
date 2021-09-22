

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
                            ForwardIncoming &fwd = *aliasing::_(forwardIncomingEdges).push_back( new ForwardIncoming(L,-nu,P) );
                            ReverseOutgoing &rev = *aliasing::_(reverseOutgoingEdges).push_back( new ReverseOutgoing(P,-nu,L) );
                            aliasing::_(L.forward).append(fwd);
                            aliasing::_(P.reverse).append(rev);
                        }
                        else
                        {
                            if(nu>0)
                            {
                                // product : forward outgoing and reverse incoming
                                ForwardOutgoing &fwd = *aliasing::_(forwardOutgoingEdges).push_back( new ForwardOutgoing(P,nu,L) );
                                ReverseIncoming &rev = *aliasing::_(reverseIncomingEdges).push_back( new ReverseIncoming(L,nu,P) );
                                aliasing::_(P.forward).append(fwd);
                                aliasing::_(L.reverse).append(rev);
                            }
                        }
                    }
                }

            }

            template <typename LIST>
            static inline void graphVizVertices(ios::ostream &fp, const LIST &L)
            {
                for(const typename LIST::node_type *v=L.head;v;v=v->next)
                {
                    (*v)->vizSave(fp);
                }
            }

            template <typename EDGES>
            static inline void graphVizEdges(ios::ostream &fp,
                                             const EDGES  &edges,
                                             const char   *arrowhead,
                                             const bool    showWeight)
            {
                assert(arrowhead);
                for(const typename EDGES::node_type *edge=edges.head;edge;edge=edge->next)
                {
                    const Vizible &src = *(edge->source);
                    const Vizible &tgt = *(edge->target);
                    src.vizName(fp); Vizible::arrow(fp); tgt.vizName(fp);
                    fp << '[';
                    fp << "arrowhead=" << arrowhead;
                    if(showWeight && (edge->weight>1) )
                    {
                        fp(",label=%u",unsigned(edge->weight));
                    }
                    fp << ']';
                    Vizible::endl(fp);
                }
            }


            void Graph:: graphViz(const string &fileName) const
            {
                {
                    ios::ocstream fp(fileName);

                    Vizible::enterDigraph(fp,"G");
                    Vizible::endl(fp << "edge [colorscheme=spectral11]");

                    graphVizVertices(fp,lineageVertices);
                    graphVizVertices(fp,primaryVertices);


                    graphVizEdges(fp,forwardOutgoingEdges,"normal",true);
                    graphVizEdges(fp,forwardIncomingEdges,"vee",true);

                    if(false)
                    {
                        graphVizEdges(fp,reverseOutgoingEdges,"onormal",false);
                        graphVizEdges(fp,reverseIncomingEdges,"ovee",false);
                    }
                    

                    Vizible::leaveDigraph(fp);

                }

                ios::GraphViz::Render(fileName);

            }

        }

    }

}
