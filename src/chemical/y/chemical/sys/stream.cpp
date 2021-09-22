

#include "y/chemical/sys/stream.hpp"

namespace upsylon
{
    namespace Chemical
    {

        namespace Stream
        {

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
                          const Primary::Array &primary)
            {
                buildList(lineageVertices,lineage);
                buildList(primaryVertices,primary);
            }

            template <typename LIST>
            static inline void graphVizList(ios::ostream &fp, const LIST &L)
            {
                for(const typename LIST::node_type *v=L.head;v;v=v->next)
                {
                    (*v)->vizSave(fp);
                }
            }

            void Graph:: graphViz(const string &fileName) const
            {
                {
                    ios::ocstream fp(fileName);

                    Vizible::enterDigraph(fp,"G");
                    graphVizList(fp,lineageVertices);
                    graphVizList(fp,primaryVertices);
                    Vizible::leaveDigraph(fp);

                }

                ios::GraphViz::Render(fileName);

            }

        }

    }

}
