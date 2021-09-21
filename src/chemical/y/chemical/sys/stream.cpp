
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
            genus(IsLineage),
            forward(),
            reverse()
            {
                aliasing::_(lineage) = &args;
            }


            Vertex:: Vertex(const Primary &args) throw() :
            Object(),
            dnode<Vertex>(),
            genus(IsPrimary),
            forward(),
            reverse()
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

namespace upsylon
{
    namespace Chemical
    {
        namespace Stream
        {
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

            const char *CourseText(const Course course) throw()
            {
                switch(course)
                {
                    case Forward: return "Forward";
                    case Reverse: return "Reverse";
                }
                return unknown_text;
            }


            Edge:: ~Edge() throw() {}

            Edge:: Edge(const Course  way,
                        const Family  how,
                        const Vertex &ini,
                        const Vertex &end,
                        const unit_t  cof) throw() :
            Oriented(way),
            family(how),
            source(ini),
            target(end),
            weight(cof)
            {

            }

            void Edge:: viz(ios::ostream &fp) const
            {
                fp.viz(&source); Viz::arrow(fp); fp.viz(&target);
                if(weight>1)
                {
                    fp << '[';
                    const string s = vformat("%d",int(weight));
                    Viz::outputLabel(fp,s);
                    fp << ']';
                }
                Viz::endl(fp);
            }


            DualEdges:: DualEdges() throw() : lineageToPrimary(), primaryToLineage()
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
// PATH
//
//
//==============================================================================

namespace upsylon
{
    namespace Chemical
    {
        namespace Stream
        {

            Path:: ~Path() throw()
            {
            }

            Path:: Path(const Edge &edge) :
            Oriented(edge),
            dnode<Path>(),
            isValid(false),
            members()
            {
                assert(edge.source.genus==IsLineage);
                Y_CHEMICAL_PRINTLN("        try " << courseText() << " path from " << edge.source.name() << " towards " << edge.target.name() );

                aliasing::_(members).append(*edge.source.lineage);
            }

            Path:: Path(const Path &other) :
            Oriented(other),
            dnode<Path>(),
            isValid(other.isValid),
            members(other.members)
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



            static inline void displayVertices(const Vertices &vtx, const size_t indent)
            {
                for(const Vertex *v=vtx.head;v;v=v->next)
                {
                    v->display(std::cerr,indent);
                }
            }

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
            pvtx(),
            forward(),
            reverse(),
            paths()
            {

                Y_CHEMICAL_PRINTLN("    <Stream::Graph>");

                //______________________________________________________________
                //
                // register all vertices
                //______________________________________________________________
                registerVertices(aliasing::_(lvtx),lineage);
                registerVertices(aliasing::_(pvtx),primary);

                //______________________________________________________________
                //
                // create all possible edges
                //______________________________________________________________
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
                                //______________________________________________
                                //
                                // species is a product
                                //______________________________________________
                                Edge &fwd = *aliasing::_(forward.primaryToLineage).push_back( new Edge(Forward,PrimaryToLineage,*p,*l,nu) );
                                Edge &rev = *aliasing::_(reverse.lineageToPrimary).push_back( new Edge(Reverse,LineageToPrimary,*l,*p,nu) );

                                aliasing::_(p->forward).append(fwd);
                                aliasing::_(l->reverse).append(rev);

                            }
                            else
                            {
                                assert(nu<0);
                                //______________________________________________
                                //
                                // species is a reactant
                                //______________________________________________
                                Edge &fwd = *aliasing::_(forward.lineageToPrimary).push_back( new Edge(Forward,LineageToPrimary,*l,*p,-nu) );
                                Edge &rev = *aliasing::_(reverse.primaryToLineage).push_back( new Edge(Reverse,PrimaryToLineage,*p,*l,-nu) );

                                aliasing::_(l->forward).append(fwd);
                                aliasing::_(p->reverse).append(rev);
                            }
                        }
                    }
                }

                assert(checkConnectivity());


                if(Verbosity)
                {
                    std::cerr << "      <Stream::Lineage>"  << std::endl;
                    displayVertices(lvtx,8);
                    std::cerr << "      <Stream::Lineage/>" << std::endl;
                    std::cerr << "      <Stream::Primary>"  << std::endl;
                    displayVertices(pvtx,8);
                    std::cerr << "      <Stream::Primary/>" << std::endl;
                }

                //______________________________________________________________
                //
                // probe all paths
                //______________________________________________________________
                buildPaths();

                Y_CHEMICAL_PRINTLN("    <Stream::Graph/>");

            }

            static const char FailureMSG[] = "FAILURE: ";

#define Y_CHECK(CONDITION) do { if(!(CONDITION)) { std::cerr << FailureMSG << #CONDITION << std::endl; return false; } } while(false)

            bool Graph:: checkConnectivity() const throw()
            {
                Y_CHECK(forward.lineageToPrimary.size==reverse.primaryToLineage.size);
                Y_CHECK(reverse.lineageToPrimary.size==forward.primaryToLineage.size);
                return true;
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


            void Graph:: buildPaths()
            {
                Y_CHEMICAL_PRINTLN("      <Stream::Paths>");

                for(const Edge *edge=forward.lineageToPrimary.head;edge;edge=edge->next)
                {
                    assert(edge->course==Forward);
                    assert(edge->family==LineageToPrimary);
                    assert(edge->source.genus==IsLineage);
                    if(Intake==edge->source.lineage->linkage)
                    {
                        tryPathFrom(*edge);
                    }
                }

                for(const Edge *edge=reverse.lineageToPrimary.head;edge;edge=edge->next)
                {
                    assert(edge->course==Reverse);
                    assert(edge->family==LineageToPrimary);
                    assert(edge->source.genus==IsLineage);
                    if(Output==edge->source.lineage->linkage)
                    {
                        tryPathFrom(*edge);
                    }
                }

                Y_CHEMICAL_PRINTLN("      <Stream::Paths/>");


            }

            void Graph:: tryPathFrom(const Edge &edge)
            {
                Paths &ways = aliasing::_(paths);
                Paths temp;
                ways.push_back( new Path(edge) );
                
            }

        }
    }
}
