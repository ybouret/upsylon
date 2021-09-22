

#include "y/chemical/sys/stream.hpp"

////////////////////////////////////////////////////////////////////////////////
//
//
// ORIENTED
//
//
////////////////////////////////////////////////////////////////////////////////
namespace upsylon
{
    namespace Chemical
    {
        namespace Stream
        {

            const char *CourseText(const Course course) throw()
            {
                switch(course)
                {
                    case Forward: return "Forward";
                    case Reverse: return "Reverse";
                }
                return unknown_text;
            }


            Oriented:: ~Oriented() throw()
            {
            }

            Oriented:: Oriented(const Course c) throw() :
            Object(), course(c) {}

            Oriented:: Oriented(const Oriented &other) throw() :
            Object(), course(other.course)
            {
            }

            const char *Oriented:: courseText() const throw()
            {
                return CourseText(course);
            }
        }

    }

}

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
            Oriented(c),
            weight(n)
            {
            }

        }

    }

}



////////////////////////////////////////////////////////////////////////////////
//
//
// Path
//
//
////////////////////////////////////////////////////////////////////////////////
#include "y/sort/merge-list.hpp"

namespace upsylon
{
    namespace Chemical
    {

        namespace Stream
        {

            std::ostream & Path:: indent(std::ostream&os) const
            {
                return Library::Indent(os,6+visited.size);
            }


            Path:: ~Path() throw()
            {
            }

            Path:: Path(const Course c) throw() :
            Oriented(c),
            visited(),
            isValid(false)
            {

            }

            Path:: Path(const Path &path) :
            Oriented(path),
            visited(path.visited),
            isValid(path.isValid)
            {
                assert(visited.size==path.visited.size);
                assert(false==isValid);
            }

            static int compareMembers(const Member *lhs, const Member *rhs, void *) throw()
            {
                assert(lhs);
                assert(rhs);
                const Species &L = ***lhs;
                const Species &R = ***rhs;
                return comparison::increasing(L.indx,R.indx);
            }

            void Path:: reshape()
            {
                merge_list_of<Member>::sort(aliasing::_(visited),compareMembers,0);
            }

            bool  Path:: owns(const Lineage &lineage) const throw()
            {
                for(const Member *m=visited.head;m;m=m->next)
                {
                    const Lineage &mine = **m;
                    if( &mine == &lineage ) return true;
                }
                return false;
            }

            void Path:: visit(const Lineage &l)
            {
                assert( !owns(l) );
                aliasing::_(visited).append(l);
            }

            void Path:: validate() throw()
            {
                aliasing::_(isValid) = true;
            }

            bool Path:: AreAnalog(const Path &lhs, const Path &rhs) throw()
            {
                if(lhs.visited.size==rhs.visited.size)
                {
                    for(const Member
                        *l=lhs.visited.head,
                        *r=rhs.visited.head;
                        l;l=l->next,r=r->next)
                    {
                        const Lineage &L = **l;
                        const Lineage &R = **r;
                        if(&L != &R) return false;
                    }
                    return true;
                }
                else
                {
                    return false;
                }
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
            primaryVertices(),
            forwardIncomingEdges(),
            forwardOutgoingEdges(),
            reverseIncomingEdges(),
            reverseOutgoingEdges()
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

                // build routes
                buildRoutes();
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


                    {
                        graphVizEdges(fp,forwardOutgoingEdges,"normal",true);
                        graphVizEdges(fp,forwardIncomingEdges,"diamond",true);
                    }

                    if(false)
                    {
                        graphVizEdges(fp,reverseOutgoingEdges,"onormal",false);
                        graphVizEdges(fp,reverseIncomingEdges,"odiamond",false);
                    }
                    

                    Vizible::leaveDigraph(fp);

                }

                ios::GraphViz::Render(fileName);

            }


            template <typename ROUTES, typename EDGE> static inline
            void growRoutes(ROUTES &routes, const EDGE &edge)
            {
                typedef typename ROUTES::node_type ROUTE;
                ROUTES extra;
                routes.push_back( new ROUTE(edge,extra) );
                routes.merge_back(extra);
                while(routes.size)
                {
                    ROUTE *route = routes.pop_front();
                    if(route->isValid)
                    {
                        extra.push_back(route);
                    }
                    else
                    {
                        delete route;
                    }
                }
                routes.swap_with(extra);
            }

            void Graph:: buildRoutes()
            {
                // forward
                Y_CHEMICAL_PRINTLN("    <ForwardPaths>");
                for(const ForwardIncoming *edge=forwardIncomingEdges.head;edge;edge=edge->next)
                {
                    const Lineage &L = *(edge->source);
                    if(L.linkage==Intake)
                    {
                        growRoutes(aliasing::_(forwardRoutes),*edge);
                        //ForwardRoutes extra;
                        //ForwardRoute  route(*edge,extra);
                    }
                }
                Y_CHEMICAL_PRINTLN("    #ForwardPaths = " << forwardRoutes.size);
                Y_CHEMICAL_PRINTLN("    <ForwardPaths/>");

                // reverse
                Y_CHEMICAL_PRINTLN("    <ReversePaths>");
                for(const ReverseIncoming *edge=reverseIncomingEdges.head;edge;edge=edge->next)
                {
                    const Lineage &L = *(edge->source);
                    if(L.linkage==Output)
                    {
                        growRoutes(aliasing::_(reverseRoutes),*edge);
                    }
                }
                Y_CHEMICAL_PRINTLN("    #ReversePaths = " << reverseRoutes.size);
                Y_CHEMICAL_PRINTLN("    <ReversePaths/>");

                Y_CHEMICAL_PRINTLN("    <CompactPaths>");
                cleanRoutes();
                Y_CHEMICAL_PRINTLN("    #ForwardPaths = " << forwardRoutes.size);
                Y_CHEMICAL_PRINTLN("    #ReversePaths = " << reverseRoutes.size);
                Y_CHEMICAL_PRINTLN("    <CompactPaths/>");

            }

            template <typename ROUTES>
            static inline void reshape(ROUTES &routes)
            {
                typedef typename ROUTES::node_type ROUTE;
                for(ROUTE *path=routes.head;path;path=path->next)
                {
                    assert(path->isValid);
                    path->reshape();
                    if(Verbosity)
                    {
                        std::cerr << path->courseText() << " {";
                        for(const Member *m=path->visited.head;m;m=m->next)
                        {
                            std::cerr << ' ' << (***m).name;
                        }
                        std::cerr << " }";
                        std::cerr << " #incoming: " << path->incoming.size;
                        std::cerr << " #outgoing: " << path->outgoing.size << std::endl;
                    }
                }
            }



            static inline
            void compactPaths(const ForwardRoutes &forward,
                              ReverseRoutes       &reverse) throw()
            {
                for(const ForwardRoute *fwd=forward.head;fwd;fwd=fwd->next)
                {
                    for(ReverseRoute *rev=reverse.head;rev;rev=rev->next)
                    {
                        if( Path::AreAnalog(*fwd,*rev) )
                        {
                            delete reverse.unlink(rev);
                            break;
                        }
                    }
                }
            }

            void Graph:: cleanRoutes()
            {
                reshape( aliasing::_(forwardRoutes) );
                reshape( aliasing::_(reverseRoutes) );
                compactPaths(forwardRoutes,aliasing::_(reverseRoutes));
            }

        }

    }

}
