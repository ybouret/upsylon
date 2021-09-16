
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

            typedef ref_dnode<const Strain>  sNode;
            typedef ref_dnode<const Primary> pNode;

            class Node : public Object, public dnode<Node>, public Vizible
            {
            public:
                enum Genus
                {
                    IsStrain,
                    IsPrimary
                };

                virtual ~Node() throw();

                const Genus genus;
                union
                {
                    const Strain  *strain;
                    const Primary *primary;
                };

                explicit Node(const Strain &s);


            private:
                Y_DISABLE_COPY_AND_ASSIGN(Node);
            };

        };


    }

}

#endif
