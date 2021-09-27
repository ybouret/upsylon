
#include "y/chemical/system.hpp"
#include "y/mkl/tao.hpp"
#include "y/mkl/kernel/lu.hpp"
#include "y/mkl/kernel/apk.hpp"
#include "y/code/textual.hpp"
#include <iomanip>




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
                Flags   go(N,false);
                V.ld(0);
                for(size_t j=MR;j>0;--j) replica[j]->fill(V[j]);
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

