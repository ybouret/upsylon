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


            class Vertex : public Object
            {
            public:
                enum Genus
                {
                    IsStrain,
                    IsPrimary,
                };

                const Genus genus;
                


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Vertex);
            };

        }

    }
}

#endif
