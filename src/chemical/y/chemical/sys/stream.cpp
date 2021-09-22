

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

/////////////////////////////////////
// GRAPH

namespace upsylon
{
    namespace Chemical
    {

        namespace Stream
        {

            Graph:: ~Graph() throw()
            {
            }

            Graph:: Graph(const Lineage::Array &lineage,
                          const Primary::Array &primary)
            {
                for(size_t i=lineage.size();i>0;--i)
                {
                    aliasing::_(lineageVertices).push_front( new LineageVertex( *lineage[i]) );
                }

                for(size_t i=primary.size();i>0;--i)
                {
                    aliasing::_(primaryVertices).push_front( new PrimaryVertex( *primary[i]) );
                }


            }

        }

    }

}
