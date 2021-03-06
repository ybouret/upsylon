
#include "y/aqua/engine.hpp"

namespace upsylon
{

    namespace Aqua
    {

        bool Engine:: sweep(addressable<double> &C, const size_t i) throw()
        {
            assert(C.size()>=M);
            for(size_t j=M;j>0;--j)
            {
                Cswp[j] = C[j];
            }

            if( equilibria[i]->sweep(K[i],Cswp,*Caux,forwardVerbose) )
            {
                for(size_t j=M;j>0;--j)
                {
                    C[j] = Cswp[j];
                }
                return true;
            }
            else
                return false;
        }

        bool Engine:: sweep(addressable<double> &C) throw()
        {
            assert(C.size()>=M);
            for(size_t j=M;j>0;--j)
            {
                Cswp[j] = C[j];
            }

            bool swept = false;
            for(size_t i=N;i>0;--i)
            {
                if( equilibria[i]->sweep(K[i], Cswp, *Caux,forwardVerbose) )
                {
                    swept = true;
                }
            }

            if(swept)
            {
                for(size_t j=M;j>0;--j)
                {
                    C[j] = Cswp[j];
                }
                return true;
            }
            else
            {
                return false;
            }
        }

    }

}
