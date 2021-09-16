
//! \file
#ifndef Y_CHEMICAL_FLUX_INCLUDED
#define Y_CHEMICAL_FLUX_INCLUDED 1

#include "y/chemical/sys/strain.hpp"
#include "y/ios/tools/vizible.hpp"
#include "y/core/rnode.hpp"

namespace upsylon
{
    namespace Chemical
    {
        typedef ios::vizible Vizible;

        struct Flux
        {
            class Vertex;

            class Edge
            {
            public:
                typedef ref_dnode<const Edge>   Node;
                typedef core::list_of_cpp<Node> List;
                const Vertex *source;
                const Vertex *target;
                const unit_t  weight;

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Edge);
            };



            class Vertex : public Object, public Vizible
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
                const Edge::List   incoming;
                const Edge::List   outgoing;

                virtual ~Vertex() throw() {}

                explicit Vertex(const Strain &S) throw() :
                genus(IsStrain), incoming(), outgoing()
                {
                    strain = &S;
                }

                explicit Vertex(const Primary &P) throw() :
                genus(IsPrimary),  incoming(), outgoing()
                {
                    primary = &P;
                }


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
                virtual void vizCore(ios::ostream &) const;
            };

            class Graph
            {
            public:

                const Vertex::Array svtx;
                const Vertex::Array pvtx;

                explicit Graph(const Strain::Array  &strain,
                               const Primary::Array &primary) :
                svtx(strain.size(), as_capacity),
                pvtx(primary.size(),as_capacity)
                {
                    Build( aliasing::_(svtx), strain);
                    Build( aliasing::_(pvtx), primary);
                }



                void graphViz(const string &fileName) const;


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
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

                static void Save(ios::ostream &fp, const Vertex::Array &arr)
                {
                    const size_t n = arr.size();
                    for(size_t i=1;i<=n;++i)
                    {
                        arr[i]->vizSave(fp);
                    }
                }

            };

            


        };


    }

}

#endif
