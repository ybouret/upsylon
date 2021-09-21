
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

            }

            Graph:: ~Graph() throw()
            {
            }

        }
    }
}
