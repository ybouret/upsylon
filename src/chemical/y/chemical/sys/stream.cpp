
#include "y/chemical/sys/stream.hpp"


//==============================================================================
//
//
// VERTEX
//
//
//==============================================================================

namespace upsylon
{
    namespace Chemical
    {
        namespace Stream
        {
            Vertex:: ~Vertex() throw()
            {
            }

            Vertex:: Vertex(const Lineage &args) throw() :
            Object(),
            dnode<Vertex>(),
            genus(IsLineage)
            {
                aliasing::_(lineage) = &args;
            }


            Vertex:: Vertex(const Primary &args) throw() :
            Object(),
            dnode<Vertex>(),
            genus(IsPrimary)
            {
                aliasing::_(primary) = &args;
            }

            const char * Vertex:: name() const throw()
            {
                switch(genus)
                {
                    case IsLineage: return *( (*lineage)->name );
                    case IsPrimary: return *( (*primary)->name );
                }
                return unknown_text;
            }

            ios::ostream & Vertex:: viz(ios::ostream &fp) const
            {
                fp.viz(this) << '[';
                Viz::outputLabel(fp,name());
                switch(genus)
                {
                    case IsPrimary:
                        Viz::appendShape(fp, "rectangle");
                        switch( (**primary).state )
                        {
                            case Flow::Bounded: Viz::appendStyle(fp, "bold");        break;
                            case Flow::Endless: Viz::appendStyle(fp, "bold,dashed"); break;

                        }
                        break;

                    case IsLineage:
                        switch(lineage->linkage)
                        {
                            case Single:
                                Viz::appendShape(fp, "egg");
                                Viz::appendStyle(fp, "bold,dotted,filled");
                                break;

                            case Intake:
                                Viz::appendShape(fp, "invhouse");
                                Viz::appendStyle(fp, "bold,solid,filled");
                                break;

                            case Output:
                                Viz::appendShape(fp, "house");
                                Viz::appendStyle(fp, "bold,solid,filled");
                                break;

                            case Inside:
                                Viz::appendShape(fp, "oval");
                                Viz::appendStyle(fp, "bold,solid,filled");
                                break;

                            case Source:
                                Viz::appendShape(fp, "invtrapezium");
                                Viz::appendStyle(fp, "bold,dashed,filled");
                                break;

                            case Siphon:
                                Viz::appendShape(fp, "trapezium");
                                Viz::appendStyle(fp, "bold,dashed,filled");
                                break;
                        }
                        break;
                }

                return Viz::endl(fp << ']');
            }


        }
    }
}

//==============================================================================
//
//
// EDGE
//
//
//==============================================================================

namespace upsylon
{
    namespace Chemical
    {
        namespace Stream
        {

            Edge:: ~Edge() throw() {}

            Edge:: Edge(const Course  way,
                        const Family  how,
                        const Vertex &ini,
                        const Vertex &end,
                        const unit_t  cof) throw() :
            course(way),
            family(how),
            source(ini),
            target(end),
            weight(cof)
            {

            }

            void Edge:: viz(ios::ostream &fp) const
            {
                fp.viz(&source); Viz::arrow(fp); fp.viz(&target);

                Viz::endl(fp);
            }


            DualEdges:: DualEdges() throw()
            {
            }

            DualEdges:: ~DualEdges() throw()
            {
            }



        }

    }

}

//==============================================================================
//
//
// GRAPH
//
//
//==============================================================================

#include "y/ios/ocstream.hpp"
#include "y/ios/tools/graphviz.hpp"

namespace upsylon
{
    namespace Chemical
    {
        namespace Stream
        {
            template <typename ARRAY> static inline
            void registerVertices( Vertices &vtx, const ARRAY &arr)
            {
                const size_t n = arr.size();
                for(size_t i=1;i<=n;++i)
                {
                    vtx.push_back( new Vertex( *arr[i]) );
                }
            }

            Graph:: Graph(const Lineage::Array &lineage,
                          const Primary::Array &primary) :
            lvtx(),
            pvtx()
            {
                registerVertices(aliasing::_(lvtx),lineage);
                registerVertices(aliasing::_(pvtx),primary);

                for(const Vertex *p=pvtx.head;p;p=p->next)
                {
                    const Equilibrium &eq = **(p->primary);
                    for(const Vertex *l=lvtx.head;l;l=l->next)
                    {
                        const Species &sp = **(l->lineage);
                        const unit_t   nu = eq.stoichiometry(sp);
                        if(0!=nu)
                        {
                            if(nu>0)
                            {
                                // species is a product
                                aliasing::_(forward.primaryToLineage).push_back( new Edge(Forward,PrimaryToLineage,*p,*l,nu) );

                            }
                            else
                            {
                                assert(nu<0);
                                // species is a reactant
                                aliasing::_(forward.lineageToPrimary).push_back( new Edge(Forward,LineageToPrimary,*l,*p,-nu) );

                            }
                        }
                    }
                }

            }

            Graph:: ~Graph() throw()
            {
            }


            static inline void writeGV(const Vertices &vertices,
                                       ios::ostream   &fp)
            {
                for(const Vertex *v=vertices.head;v;v=v->next)
                {
                    v->viz(fp);
                }
            }

            static inline void writeGV(const Edges    &edges,
                                       ios::ostream   &fp)
            {
                for(const Edge *edge=edges.head;edge;edge=edge->next)
                {
                    edge->viz(fp);
                }
            }

            static inline void writeGV(const DualEdges &dual,
                                       ios::ostream    &fp)
            {
                writeGV(dual.lineageToPrimary,fp);
                writeGV(dual.primaryToLineage,fp);
            }


            void Graph:: graphViz(const string &fileName) const
            {
                {
                    ios::ocstream fp(fileName);
                    Viz::enterDigraph(fp,"G");
                    writeGV(lvtx,fp);
                    writeGV(pvtx,fp);
                    writeGV(forward,fp);
                    Viz::leaveDigraph(fp);
                }
                ios::GraphViz::Render(fileName);

            }

        }
    }
}
