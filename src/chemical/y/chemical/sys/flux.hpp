
//! \file
#ifndef Y_CHEMICAL_FLUX_INCLUDED
#define Y_CHEMICAL_FLUX_INCLUDED 1

#include "y/chemical/sys/strain.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/core/rnode.hpp"
#include "y/container/key-address.hpp"
#include "y/associative/suffix/set.hpp"

namespace upsylon
{
    namespace Chemical
    {
        typedef ios::vizible Vizible;

        struct Flux
        {
            class Vertex;


            class Edge : public Object
            {
            public:
                typedef ref_dnode<const Edge>   Node;
                typedef core::list_of_cpp<Node> List;
                typedef key_address<2>          Key;
                typedef intr_ptr<Key,Edge>      Pointer;
                typedef suffix_set<Key,Pointer> Set;
                typedef Set::data_node          Iter;

                const Vertex *source;
                const Vertex *target;
                const unit_t  weight;
                const Key     vkey;

                virtual ~Edge() throw() {}
                explicit Edge(const Vertex &ini,
                              const Vertex &end,
                              const unit_t  cof) throw() :
                source(&ini),
                target(&end),
                weight(cof),
                vkey(ini,end)
                {
                }

                const Key & key() const throw() { return vkey; }

                void viz(ios::ostream&) const;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Edge);
            };



            class Vertex : public Object
            {
            public:
                typedef arc_ptr<const Vertex>     Pointer;
                typedef vector<Pointer,Allocator> Array;
                
                enum Genus
                {
                    IsStrain,
                    IsPrimary
                };

                const Genus        genus;
                union {
                    const Strain  *strain;
                    const Primary *primary; };
                const Edge::List   edges;

                virtual ~Vertex() throw() {}

                explicit Vertex(const Strain &S) throw() :
                genus(IsStrain), edges()
                {
                    strain = &S;
                }

                explicit Vertex(const Primary &P) throw() :
                genus(IsPrimary),  edges()
                {
                    primary = &P;
                }

                void graphViz(ios::ostream &) const;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
            };

            class Graph
            {
            public:

                const Vertex::Array svtx;
                const Vertex::Array pvtx;
                const Edge::Set     edges;

                explicit Graph(const Strain::Array  &strain,
                               const Primary::Array &primary) :
                svtx(strain.size(), as_capacity),
                pvtx(primary.size(),as_capacity)
                {
                    Build( aliasing::_(svtx), strain);
                    Build( aliasing::_(pvtx), primary);
                    connect(strain);
                }


                void graphViz(const string &fileName) const;

                const Vertex *query(const Primary &) const throw();
                const Edge   &query(const Vertex *ini, const Vertex *end, const unit_t cof);

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
                void connect(const Strain::Array &);

                template <typename SOURCE> static inline
                void Build(Vertex::Array &target, const SOURCE &source)
                {
                    const size_t n = source.size(); assert(target.capacity()>=n);
                    for(size_t i=1;i<=n;++i)
                    {
                        const Vertex::Pointer V =   new Vertex( *source[i] );
                        target.push_back_(V);
                    }
                }

                static void Save(ios::ostream &fp, const Vertex::Array &arr);

            };

            


        };


    }

}

#endif
