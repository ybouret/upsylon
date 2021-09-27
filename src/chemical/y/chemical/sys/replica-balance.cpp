
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/code/textual.hpp"
#include <iomanip>

#include "y/counting/comb.hpp"
#include "y/sort/sorted-sum.hpp"


namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        bool System:: balanceReplica(Addressable &C) throw()
        {
            static const size_t from=2;
            static const size_t curr=from+2;

            if(Verbosity)
            {
                Library::Indent(std::cerr,from) << "<Balance Replica>" << std::endl;
                lib.display(std::cerr,C,curr) << std::endl;
                showPrimary(std::cerr,C,curr);
                showReplica(std::cerr,C,curr);
            }

            // assuming balance primary is true
            if(MR>0)
            {
                iMatrix V(MR,N);
                iMatrix U(N,MR);
                for(size_t i=MR;i>0;--i) replica[i]->fill(V[i]);
                U.assign_transpose(V);
                std::cerr << "U=" << U << std::endl;
                std::cerr << "V=" << V << std::endl;

            }

            if(Verbosity)
            {
                Library::Indent(std::cerr,from) << "<Balance Replica>" << std::endl;
            }

            return false;

        }
    }

}

