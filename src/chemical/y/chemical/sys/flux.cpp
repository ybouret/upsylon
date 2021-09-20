#include "y/chemical/sys/flux.hpp"
#include "y/ios/ocstream.hpp"
#include "y/string/convert.hpp"
#include "y/ios/tools/graphviz.hpp"
#include "y/type/utils.hpp"

//! EDGE
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

// VERTEX

namespace upsylon
{
    namespace Chemical
    {
        Flux:: Vertex:: ~Vertex() throw()
        {
        }
        
#define Y_CHEM_FLUX_CTOR(FLAG) genus(FLAG), outgoing(), incoming()
        
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
        
        const char * Flux::Vertex:: name() const throw()
        {
            switch(genus)
            {
                case IsStrain:  return *((**strain) .name);
                case IsPrimary: return *((**primary).name);
            }
            return unknown_text;
        }
        
        
        bool Flux::Vertex:: hasOutgoing(const Edge &edge) const throw()
        {
            for(const Edge::Node *node=outgoing.head;node;node=node->next)
            {
                const Edge &self = **node;
                if( &self == &edge ) return true;
            }
            return false;
        }
        
        void Flux::Vertex:: Display(const Edge::List &edges, const bool flag)
        {
            std::cerr << edges.size;
            if(edges.size) {
                std::cerr<< " {";
                for(const Edge::Node *node=edges.head;node;node=node->next)
                {
                    const Edge &E = **node;
                    std::cerr << ' ' << (flag?E.target.name():E.source.name());
                }
                std::cerr <<" }";
            }
        }
        
        void Flux:: Vertex:: display() const
        {
            std::cerr << "      " << name();
            if(outgoing.size)
            {
                std::cerr << " | #out=";  Display(outgoing,true);
            }
            if(incoming.size)
            {
                std::cerr << " | #in ="; Display(incoming,false);
            }
            std::cerr << std::endl;
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
                    style = "bold,filled";
                    
                    switch(S.linkage)
                    {
                        case Single:
                            style += ",dotted";
                            break;
                            
                        case Inside:
                            break;
                            
                        case Intake:
                            shape = "house";
                            break;
                            
                        case Output:
                            shape = "invhouse";
                            break;
                            
                        case Siphon:
                            shape  = "trapezium";
                            style += ",dashed";
                            break;
                            
                        case Source:
                            shape = "invtrapezium";
                            style += ",dashed";
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
        
        void Flux::Path:: push(const Edge &edge)
        {
            aliasing::_(edges).append(edge);
        }
        
        void Flux::Path:: push(const Strain *strain)
        {
            assert(strain);
            aliasing::_(slist).append(*strain);
            assert(owns(strain));
        }
        
        void Flux::Path:: setup(const Edge &edge)
        {
            push(edge);
            try
            {
                switch(route)
                {
                    case Forward: push(edge.source.strain); break;
                    case Reverse: push(edge.target.strain); break;
                }
            }
            catch(...)
            {
                delete aliasing::_(edges).pop_back();
                throw;
            }

        }
        
    }
}

#include "y/sort/merge-list.hpp"

// PATH
namespace upsylon
{
    namespace Chemical
    {

        static inline Flux::Route Edge2Route(const Flux::Edge &edge)
        {
            if(edge.source.genus==Flux::Vertex::IsStrain)
            {
                assert(edge.source.strain->linkage==Intake);
                return Flux::Forward;
            }
            else
            {
                assert(edge.target.genus==Flux::Vertex::IsStrain);
                assert(edge.target.strain->linkage==Output);
                return Flux::Reverse;
            }
        }

        const char * Flux:: RouteText(const Route route) throw()
        {
            switch(route)
            {
                case Forward: return "Forward";
                case Reverse: return "Reverse";
            }
            return unknown_text;
        }

        Flux:: Path:: Path(const Edge &edge) throw() :
        Object(), dnode<Path>(),
        route( Edge2Route(edge) ),
        edges(),
        slist(),
        valid(true)
        {
            setup(edge);
        }
        
        Flux:: Path:: Path(const Path &path) :
        Object(), dnode<Path>(),
        route(path.route),
        edges(path.edges),
        slist(path.slist),
        valid(path.valid)
        {

        }


        Flux:: Path:: ~Path() throw()
        {
        }


        const char  * Flux::Path:: routeText() const throw()
        {
            return RouteText(route);
        }

        static inline int sNodeCompare(const Flux::sNode *lhs,
                                       const Flux::sNode *rhs,
                                       void *) throw()
        {
            const Species &L = ***lhs;
            const Species &R = ***rhs;
            return comparison::increasing_addresses(&L,&R);
        }

        static inline void sListUpdate(Flux::sList &slist)
        {
            merge_list_of<Flux::sNode>::sort(slist,sNodeCompare,0);
        }

        bool Flux:: Path:: owns(const Strain *strain) const throw()
        {
            assert(strain);
            const Species   &spec   = **strain;
            for(const sNode *node = slist.head;node;node=node->next)
            {
                const Species &temp = ***node;
                if(&temp == &spec) return true;
            }
            return false;
        }

        Flux::Path::List & Flux::Path:: grow(List &stack)
        {
            Y_CHEMICAL_PRINTLN("      Try " << routeText() << " Path from " << ***slist.head);

            {
                switch(route)
                {
                    case Forward: runForward(stack); break;
                    case Reverse: runReverse(stack); break;
                }
            }


            return stack;
        }

        void Flux::Path:: runForward(List &stack)
        {
            Path             &self = *this;
            const Edge       &curr = **edges.tail;   assert(curr.target.genus==Vertex::IsPrimary);
            const Vertex     &vhub = curr.target;
            const Edge::List &dest = vhub.outgoing;
            assert(dest.size>0);
            for(const Edge::Node *node=dest.tail;node!=dest.head;node=node->prev)
            {
                stack.push_back( new Path(self) )->tryForward(**node,stack);
            }
            self.tryForward(**dest.head,stack);
        }

        void Flux::Path:: tryForward(const Edge &edge, List &stack)
        {
            assert(edge.target.genus==Vertex::IsStrain);
        }


        void Flux::Path:: runReverse(List &stack)
        {
            Path             &self = *this;
            const Edge       &curr = **edges.tail;   assert(curr.source.genus==Vertex::IsPrimary);
            const Vertex     &vhub = curr.source;
            const Edge::List &from = vhub.incoming;
            assert(from.size>0);
            for(const Edge::Node *node=from.tail;node!=from.head;node=node->prev)
            {
                stack.push_back( new Path(self) )->tryReverse(**node,stack);
            }
            self.tryReverse(**from.head,stack);
        }


        void Flux::Path:: tryReverse(const Edge &edge, List &stack)
        {
            assert(edge.source.genus==Vertex::IsStrain);

        }


    }

}


// GRAPH
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
            run();
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
            
            
            
            // build incoming
            for(const Edge::Iter *node=edges.head();node;node=node->next)
            {
                const Edge   &edge   = ***node;
                assert(edge.source.hasOutgoing(edge));
                aliasing::_(edge.target.incoming).push_back( new Edge::Node(edge) );
            }
            
            
            // result
            if(Verbosity)
            {
                std::cerr << "  <Graph>" << std::endl;
                
                std::cerr << "    <Species>" << std::endl;
                for(size_t i=1;i<=svtx.size();++i)
                {
                    svtx[i]->display();
                }
                std::cerr << "    <Species/>" << std::endl;
                
                
                std::cerr << "    <Equilibria>" << std::endl;
                for(size_t i=1;i<=pvtx.size();++i)
                {
                    pvtx[i]->display();
                }
                std::cerr << "    <Equilibria/>" << std::endl;
                
                std::cerr << "  <Graph/>" << std::endl;
                
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



namespace upsylon
{
    namespace Chemical
    {


        void    Flux::Graph::   buildPathFrom(const Edge &edge)
        {
            Path::List  temp;
            aliasing::_(paths).push_back( new Path(edge) )->grow(temp);

        }

        void Flux::Graph:: run( )
        {


            Y_CHEMICAL_PRINTLN("    <Paths>");
            for(const Edge::Iter *node=edges.head();node;node=node->next)
            {
                const Edge &edge = ***node;
                if(Vertex::IsStrain==edge.source.genus)
                {
                    if(Intake == edge.source.strain->linkage)
                        buildPathFrom(edge);

                }
                else
                {
                    assert(edge.target.genus==Vertex::IsStrain);
                    if(Output == edge.target.strain->linkage)
                        buildPathFrom(edge);
                }
            }
            Y_CHEMICAL_PRINTLN("    <Paths/>");

        }

    }

}
