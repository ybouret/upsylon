#include "y/chemical/sys/flux.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Flux:: Edge::Edge(const Vertex &ini,
                          const Vertex &end,
                          const unit_t  cof) throw() :
        source(ini),
        target(end),
        weight(cof),
        vkey(source,target)
        {
        }

        Flux:: Edge:: ~Edge() throw()
        {
        }

        const Flux::Edge::Key &  Flux::Edge::key() const throw() { return vkey; }



        void Flux::Edge:: viz(ios::ostream &fp) const
        {
            fp.viz(&source);
            Vizible::arrow(fp);
            fp.viz(&target);
            if(weight>1)
            {
                fp << vformat("[label=\"%d\"]",int(weight));
            }
            Vizible::endl(fp);
        }

    }


}


namespace upsylon
{
    namespace Chemical
    {
        Flux:: Vertex:: ~Vertex() throw()
        {
        }

#define Y_CHEM_FLUX_CTOR(FLAG) genus(FLAG), outgoing()

        Flux:: Vertex:: Vertex(const Strain &S) throw() :
        Y_CHEM_FLUX_CTOR(IsStrain)
        {
            strain = &S;
        }

        Flux:: Vertex:: Vertex(const Primary &P) throw() :
        Y_CHEM_FLUX_CTOR(IsPrimary)
        {
            primary = &P;
        }

        void Flux::Vertex:: viz(ios::ostream &fp) const
        {
            fp.viz(this);
            string label;
            string shape;
            string style;
            switch(genus)
            {
                case IsPrimary: {
                    const Primary     &pp = *primary;
                    const Equilibrium &eq = *pp;
                    shape = "rectangle";
                    style = "bold";
                    if(eq.state==Flow::Endless)
                    {
                        style += ",dashed";
                    }

                    label = string_convert::to_printable(eq.name);

                } break;

                case IsStrain: {
                    const Strain  &S = *strain;
                    const Species &s = *S;
                    shape = "oval";
                    style = "bold";

                    switch(S.linkage)
                    {
                        case Single:

                            style += ",dotted";
                            break;

                        case Inside:
                            break;

                        case Intake:
                            shape = "house";
                            style += ",filled";
                            break;

                        case Output:
                            shape = "invhouse";
                            style += ",filled";
                            break;

                        case Siphon:
                            shape  = "trapezium";
                            style += ",dashed,filled";
                            break;

                        case Source:
                            shape = "invtrapezium";
                            style += ",dashed,filled";
                            break;

                        default:
                            break;
                    }

                    label = string_convert::to_printable(s.name);

                } break;
            }

            fp << "[label=\"" << label << "\",shape=\"" << shape << "\",style=\"" << style << "\"]";
            Vizible::endl(fp);
        }
    }

}


namespace upsylon
{
    namespace Chemical
    {

        Flux::Graph:: ~Graph() throw() {}

        template <typename SOURCE> static inline
        void Build(Flux::Vertex::Array &target, const SOURCE &source)
        {
            const size_t n = source.size(); assert(target.capacity()>=n);
            for(size_t i=1;i<=n;++i)
            {
                const Flux::Vertex::Pointer V =   new Flux::Vertex( *source[i] );
                target.push_back_(V);
            }
        }

        Flux::Graph::Graph(const Strain::Array  &strain,
                           const Primary::Array &primary) :
        svtx(strain.size(), as_capacity),
        pvtx(primary.size(),as_capacity),
        edges()
        {
            Build( aliasing::_(svtx), strain);
            Build( aliasing::_(pvtx), primary);
            connect(strain);
        }
        void Flux::Graph:: Save(ios::ostream &fp, const Vertex::Array &arr)
        {
            const size_t n = arr.size();
            for(size_t i=1;i<=n;++i)
            {
                arr[i]->viz(fp);
            }
        }


        const Flux::Edge   & Flux::Graph:: query(const Vertex *ini, const Vertex *end, const unit_t cof)
        {
            assert(ini);
            assert(end);
            assert(cof>0);

            const Edge::Key      key(*ini,*end);
            const Edge::Pointer *ppE = edges.search(key);
            if(ppE)
            {
                if(cof != (**ppE).weight ) throw exception("existing edge weight mismatch");
                return **ppE;
            }
            else
            {
                Edge *pE = new Edge(*ini,*end,cof);
                const Edge::Pointer tmp(pE);
                if( !aliasing::_(edges).insert(tmp)) throw exception("unexpected new edge insertion failure");
                return *pE;
            }
        }

        void Flux::Graph:: connect(const Strain::Array &strain)
        {
            // build outgoing
            {
                const size_t n = strain.size();
                for(size_t i=1;i<=n;++i)
                {
                    const Strain &S = *strain[i];
                    const Vertex *s = & *svtx[i]; assert(s->strain==&S);

                    // producer -> species
                    for(const Appliance *app=S.producers.head;app;app=app->next)
                    {
                        const Primary &P    = **app;
                        const Vertex  *p    = & *pvtx[P->indx]; assert(p->primary==&P);
                        const unit_t   nu   = app->nu; assert(nu!=0);
                        const Edge    &edge = query(p,s,abs_of(nu));
                        aliasing::_(p->outgoing).push_back( new Edge::Node(edge) );
                    }

                    // species -> consumer
                    for(const Appliance *app=S.consumers.head;app;app=app->next)
                    {
                        const Primary &P  = **app;
                        const Vertex  *p  = & *pvtx[P->indx]; assert(p->primary==&P);
                        const unit_t   nu = app->nu; assert(nu!=0);
                        const Edge    &edge = query(s,p,abs_of(nu));
                        aliasing::_(s->outgoing).push_back( new Edge::Node(edge) );
                    }
                }

            }

        }


        

        void     Flux:: Graph::   join(ios::ostream &fp) const
        {

            for(const Edge::Iter *node=edges.head();node;node=node->next)
            {
                const Edge &edge = ***node;
                edge.viz(fp);
            }

        }


        void Flux:: Graph:: graphViz(const string &fileName) const
        {
            {
                ios::ocstream fp(fileName);     // open file
                Vizible::enterDigraph(fp,"G");  // init digraph
                Save(fp,svtx);                  // strain nodes
                Save(fp,pvtx);                  // primary nodes
                join(fp);                       // edges
                Vizible::leaveDigraph(fp);   }  // done digraph
            ios::GraphViz::Render(fileName);    // and render
        }

    }

}
