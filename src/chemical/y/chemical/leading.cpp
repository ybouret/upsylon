#include "y/chemical/leading.hpp"

namespace upsylon
{
    namespace Chemical
    {


#define Y_CHEM_LEAD(TYPE) case TYPE: return #TYPE

        const char * Leading:: KindText(const Kind k) throw()
        {
            switch(k)
            {
                    Y_CHEM_LEAD(LimitedByNone);
                    Y_CHEM_LEAD(LimitedByReac);
                    Y_CHEM_LEAD(LimitedByProd);
                    Y_CHEM_LEAD(LimitedByBoth);
            }

            return "???";
        }

        const char * Leading:: StatusText(const Status s) throw()
        {
            switch(s)
            {
                    Y_CHEM_LEAD(Accepted);
                    Y_CHEM_LEAD(Modified);
                    Y_CHEM_LEAD(Rejected);
            }

            return "???";
        }


        const char * Leading:: kindText() const throw()
        {
            return KindText(kind);
        }
        
        Leading:: ~Leading() throw()
        {
        }

        Leading:: Leading(const Equilibrium::Pointer &ep) :
        root(ep),
        reac( root->reac.tellLeading(), as_capacity),
        prod( root->prod.tellLeading(), as_capacity),
        kind(LimitedByNone),
        xmax(0),
        xmin(0)
        {
            root->reac.fillLeading( aliasing::_(reac) );
            root->prod.fillLeading( aliasing::_(prod) );

            if(reac.size()>0)
            {
                if(prod.size()>0)
                {
                    aliasing::_(kind) = LimitedByBoth;
                }
                else
                {
                    assert(0==prod.size());
                    aliasing::_(kind) = LimitedByReac;
                }
            }
            else
            {
                assert(0==reac.size());
                if(prod.size()>0)
                {
                    aliasing::_(kind) = LimitedByProd;
                }
                else
                {
                    assert(0==prod.size());
                    aliasing::_(kind) = LimitedByNone;
                }
            }
        }

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


#include "y/mkl/tao.hpp"

namespace upsylon
{
    using namespace mkl;

    namespace Chemical
    {

        Leading::Status Leading:: limitedByReac(Addressable   &C,
                                                const iMatrix &NuT,
                                                Addressable   &xi) const throw()
        {
            const Actor &amax = maxFromReac(C);
            Y_CHEMICAL_PRINTLN( "    |_max=" << xmax << " from " << amax.sp << " = " << C[amax.sp.indx]);
            if(xmax<0)
            {
                tao::ld(xi,0);
                xi[root->indx] = xmax;
                tao::mul_add(C,NuT,xi);
                C[amax.sp.indx] = 0;
                return Modified;
            }
            else
            {
                return Accepted;
            }
        }

        Leading::Status Leading:: limitedByProd(Addressable   &C,
                                                const iMatrix &NuT,
                                                Addressable   &xi) const throw()
        {
            const Actor &amin = minFromProd(C);
            Y_CHEMICAL_PRINTLN("    |_min=" << xmin << " from " << amin.sp << " = " << C[amin.sp.indx]);
            if(xmin>0)
            {
                tao::ld(xi,0);
                xi[root->indx] = xmin;
                tao::mul_add(C,NuT,xi);
                C[amin.sp.indx] = 0;
                return Modified;
            }
            else
            {
                return Accepted;
            }
        }

        Leading::Status Leading:: limitedByBoth(Addressable   &C,
                                                const iMatrix &NuT,
                                                Addressable   &xi) const throw()
        {
            const Actor &amax = maxFromReac(C);
            const Actor &amin = minFromProd(C);
            Y_CHEMICAL_PRINTLN("    |_min=" << xmin << " from " << amin.sp << " = " << C[amin.sp.indx]);
            Y_CHEMICAL_PRINTLN("    |_max=" << xmax << " from " << amax.sp << " = " << C[amax.sp.indx]);

            if(xmin>xmax)
            {
                return Rejected;
            }
            else
            {
                if(xmin>0)
                {
                    tao::ld(xi,0);
                    xi[root->indx] = xmin;
                    tao::mul_add(C,NuT,xi);
                    C[amin.sp.indx] = 0;
                    return Modified;
                }

                if(xmax<0)
                {
                    tao::ld(xi,0);
                    xi[root->indx] = xmax;
                    tao::mul_add(C,NuT,xi);
                    C[amax.sp.indx] = 0;
                    return Modified;
                }

                return Accepted;
            }


        }



        Leading::Status Leading:: solve(Addressable   &C,
                                        const iMatrix &NuT,
                                        Addressable   &xi) const throw()
        {
            Y_CHEMICAL_PRINTLN("    " << root);
            Leading::Status status = Rejected;
            switch(kind)
            {
                case LimitedByNone: status = Accepted; break;
                case LimitedByReac: status = limitedByReac(C,NuT,xi); break;
                case LimitedByProd: status = limitedByProd(C,NuT,xi); break;
                case LimitedByBoth: status = limitedByBoth(C,NuT,xi); break;
            }
            Y_CHEMICAL_PRINTLN("    |_" << StatusText(status));
            return status;
        }

    }
}
