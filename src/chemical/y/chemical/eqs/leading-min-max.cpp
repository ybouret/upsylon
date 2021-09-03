#include "y/chemical/leading.hpp"

namespace upsylon
{
    namespace Chemical
    {


        const Actor & Leading:: maxFromReac(const Accessible &C) const throw()
        {
            assert(reac.size()>0);
            assert(LimitedByReac==kind||LimitedByBoth==kind);

            const Actor *best = &reac[1];
            xmax              = C[best->sp.indx]/best->nu;
            for(size_t i=reac.size();i>1;--i)
            {
                const Actor *temp = &reac[i];
                const double xtmp = C[temp->sp.indx]/temp->nu;
                if(xtmp<xmax)
                {
                    xmax = xtmp;
                    best = temp;
                }
            }
            return *best;
        }

        const Actor & Leading:: minFromProd(const Accessible &C) const throw()
        {
            assert(prod.size()>0);
            assert(LimitedByProd==kind||LimitedByBoth==kind);

            const Actor *best = &prod[1];
            xmin              = -C[best->sp.indx]/best->nu;
            for(size_t i=prod.size();i>1;--i)
            {
                const Actor *temp = &prod[i];
                const double xtmp = -C[temp->sp.indx]/temp->nu;
                if(xtmp>xmin)
                {
                    xmin = xtmp;
                    best = temp;
                }
            }
            return *best;
        }


    }

}
