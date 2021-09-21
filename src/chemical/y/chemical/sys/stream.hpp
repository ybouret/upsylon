//! \file

#ifndef Y_CHEMICAL_FLOW_INCLUDED
#define Y_CHEMICAL_FLOW_INCLUDED 1

#include "y/chemical/sys/lineage.hpp"
#include "y/ios/tools/vizible.hpp"

namespace upsylon
{
    namespace Chemical
    {

        namespace Stream
        {
            enum Genus
            {
                IsLineage,
                IsPrimary,
            };

            //! a variant vertex in the graph
            class Vertex : public Object, public dnode<Vertex>
            {
            public:

                const Genus genus;
                union {
                    const Lineage *lineage;
                    const Primary *primary;
                };

                explicit Vertex(const Lineage &) throw();
                explicit Vertex(const Primary &) throw();
                virtual ~Vertex() throw();

            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
            };

            typedef core::list_of_cpp<Vertex> Vertices;

            //! graph
            class Graph
            {
            public:
                explicit Graph(const Lineage::Array &lineage,
                               const Primary::Array &primary);
                virtual ~Graph() throw();

                const Vertices vertices;
                
            private:
                Y_DISABLE_COPY_AND_ASSIGN(Graph);
            };


        }

    }
}

#endif
