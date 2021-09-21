
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
#include "y/sort/merge-list.hpp"

namespace upsylon
{
    namespace Chemical
    {
        namespace Stream
        {
            const char   Path:: CLID[]     = "Stream::Path";
            const size_t Path:: BaseIndent = 8;

            std::ostream & Path:: indent(std::ostream &os) const
            {
                return Library::Indent(os,BaseIndent+members.size*2);
            }


            Path:: ~Path() throw()
            {
            }

            static inline int compareMembers(const Member *lhs,
                                             const Member *rhs,
                                             void *) throw()
            {
                const Species &L = ***lhs;
                const Species &R = ***rhs;
                return comparison::increasing(L.indx,R.indx);
            }

            void Path:: reshape()
            {
                merge_list_of<Member>::sort(aliasing::_(members),compareMembers,0);
            }

            bool Path:: AreAnalog(const Path &lhs, const Path &rhs) throw()
            {
                if(lhs.members.size==rhs.members.size)
                {
                    for(const Member
                        *l=lhs.members.head,
                        *r=rhs.members.head;l;l=l->next,r=r->next)
                    {
                        const Species &L = ***l;
                        const Species &R = ***r;
                        if(L.indx!=R.indx) return false;
                    }
                    return true;
                }
                else
                {
                    return false;
                }
            }

            
            Path:: Path(const Edge &edge,
                        List       &temp) :
            Oriented(edge),
            dnode<Path>(),
            isValid(false),
            members()
            {
                assert(edge.source.genus==IsLineage);
                assert(edge.target.genus==IsPrimary);

                if(Verbosity)
                {
                    indent(std::cerr) << '[' << edge.source.name() << ']' << "/" << courseText() << std::endl;
                }

                // initialize fist member
                aliasing::_(members).append(*edge.source.lineage);
                assert(owns(edge.source.lineage));


                // try to grow
                conn(edge.target,temp);

            }

            Path:: Path(const Path &other) :
            Oriented(other),
            dnode<Path>(),
            isValid(other.isValid),
            //trigger(other.trigger),
            members(other.members)
            {
            }

            bool Path:: owns(const Lineage *lineage) const throw()
            {
                for(const Member *member=members.head;member;member=member->next)
                {
                    const Lineage &mine = **member;
                    if(&mine==lineage) return true;
                }
                return false;
            }

            void Path:: conn(const Vertex &vhub,
                             List         &temp)
            {
                if(Verbosity)
                {
                    indent(std::cerr) << "|_<" << vhub.name() << ">" << std::endl;
                }

                assert(vhub.genus==IsPrimary);
                const Links *links = 0;
                switch(course)
                {
                    case Forward: links = &vhub.forward; break;
                    case Reverse: links = &vhub.reverse; break;
                }

                if(links->size<=0) throw exception("%s %s detected a corrupted linkage!",courseText(), CLID);

                const Link     *head=links->head;
                for(const Link *link=links->tail;link!=head;link=link->prev)
                {
                    Path *path = temp.push_back( new Path(*this) );
                    path->grow(**link,temp);
                }

                this->grow(**head,temp);


            }

            void Path:: grow(const Edge &edge, List &temp)
            {
                assert(edge.source.genus==IsPrimary);
                assert(edge.target.genus==IsLineage);

                const char * const spName = edge.target.name();
                if(Verbosity)
                {
                    indent(std::cerr) << "|_[" << spName << "]" << std::endl;
                }
                const Vertex  &lvertex = edge.target;
                const Lineage *lineage = lvertex.lineage;

                if(owns(lineage))
                {

                    // detected cycle
                    if(Verbosity) { indent(std::cerr) << "|_[[cycle]]" << std::endl; }
                }
                else
                {
                    const Linkage linkage = lineage->linkage;
                    switch(linkage)
                    {
                        case Single: break;

                        case Intake:
                        {
                            switch(course)
                            {
                                case Reverse:
                                    aliasing::_(isValid)=true;
                                    aliasing::_(members).append(*lineage);
                                    if(Verbosity) { indent(std::cerr) << "|_**" << lineage->stateText() << "**" << std::endl; }
                                    return;           // valid
                                case Forward: break;  // corrupted
                            }
                        } break;

                        case Output:
                        {
                            switch(course)
                            {
                                case Forward:
                                    aliasing::_(isValid)=true;
                                    aliasing::_(members).append(*lineage);
                                    if(Verbosity) { indent(std::cerr) << "|_**" << lineage->stateText() << "**" << std::endl; }
                                    return;           // valid
                                case Reverse: break;  // corrupted
                            }
                        } break;

                        case Source:
                        case Siphon:
                            if(Verbosity) { indent(std::cerr) << "|_[[" << lineage->stateText() << "]]" << std::endl; }
                            return; // stop, not conservative

                        case Inside: // recursive
                            aliasing::_(members).append(*lineage);
                            goto HUB;


                    }
                    throw exception("*** %s %s found %s species [%s] on its way",courseText(),CLID,LinkageText(linkage),spName);

                HUB:;
                    const Links *links = 0;
                    switch(course)
                    {
                        case Forward: links = &lvertex.forward; break;
                        case Reverse: links = &lvertex.reverse; break;
                    }
                    assert(links);
                    assert(links->size>0);

                    const Link     *head=links->head;
                    for(const Link *link=links->tail;link!=head;link=link->prev)
                    {
                        assert((**link).target.genus==IsPrimary);
                        Path *path = temp.push_back( new Path(*this) );
                        path->conn((**link).target,temp);
                    }

                    assert((**head).target.genus==IsPrimary);
                    this->conn( (**head).target,temp);

                }

            }

            void Path:: viz(ios::ostream &fp, const unsigned c) const
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
                    Viz::endl(fp << "edge [colorscheme=spectral11]");
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
                        expandPath(*edge);
                    }
                }

                for(const Edge *edge=reverse.lineageToPrimary.head;edge;edge=edge->next)
                {
                    assert(edge->course==Reverse);
                    assert(edge->family==LineageToPrimary);
                    assert(edge->source.genus==IsLineage);
                    if(Output==edge->source.lineage->linkage)
                    {
                        expandPath(*edge);
                    }
                }

                zapAnalogs();

                Y_CHEMICAL_PRINTLN("      #Path = " << paths.size);
                Y_CHEMICAL_PRINTLN("      <Stream::Paths/>");


            }

            void Graph:: expandPath(const Edge &edge)
            {
                Path::List &ways = aliasing::_(paths);
                Path::List temp;
                ways.push_back( new Path(edge,temp) );
                ways.merge_back(temp);
                assert(0==temp.size);
                while(ways.size)
                {
                    if(ways.head->members.size<=2)
                    {
                        throw exception("%s %s is too small!!!", ways.head->courseText(), Path::CLID);
                    }
                    Path *path = ways.pop_back();
                    if(path->isValid)
                    {
                        temp.push_back(path)->reshape();
                    }
                    else
                    {
                        delete path;
                    }
                }
                ways.swap_with(temp);
            }

            void Graph:: zapAnalogs() throw()
            {

                Path::List &ways = aliasing::_(paths);
                Path::List temp;
                while(ways.size)
                {
                    Path *lhs         = ways.pop_front();
                    Path *rhs         = ways.head;
                    bool  foundAnalog = false;
                    for(;rhs;rhs=rhs->next)
                    {
                        if( Path::AreAnalog(*lhs,*rhs) )
                        {
                            foundAnalog = true;
                            break;
                        }
                    }
                    if(foundAnalog)
                    {
                        assert(lhs);
                        assert(rhs);
                        assert(lhs!=rhs);
                        rhs = ways.unlink(rhs);
                        assert(lhs->course!=rhs->course);
                        if(lhs->course==Forward)
                        {
                            delete rhs;
                            temp.push_back(lhs);
                        }
                        else
                        {
                            assert(rhs->course==Forward);
                            delete lhs;
                            temp.push_back(rhs);
                        }

                    }
                    else
                    {
                        temp.push_back(lhs);
                    }
                }
                temp.swap_with(ways);

            }

        }
    }
}
