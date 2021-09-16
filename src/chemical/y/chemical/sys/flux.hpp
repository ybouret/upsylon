
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
            typedef core::list_of_cpp<sNode> sList;
            typedef core::list_of_cpp<pNode> pList;

            

        };


    }

}

#endif
