#include "y/chemical/leading.hpp"

namespace upsylon
{
    namespace Chemical
    {

        const char Leading:: LeqText[] = " <=  ";
        const char Leading:: GeqText[] = " >= -";

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

        size_t Leading:: count() const throw()
        {
            return reac.size() + prod.size();
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

       

    }

}


#include "y/mkl/tao.hpp"
#include <iomanip>

namespace upsylon
{

    using namespace mkl;
    namespace Chemical
    {

        void Leading:: EnsurePositive(Addressable &C, const Limiting &actors) throw()
        {
            for(size_t i=actors.size();i>0;--i)
            {
                const Actor   &a  = actors[i];
                const Species &sp = a.sp;
                if(1==sp.rating)
                {
                    double &Cj = C[ sp.indx ];
                    std::cerr << "       \\_check leading: " << sp << "@ " << Cj << std::endl;
                    if(Cj<0) Cj=0;
                }

            }
        }




        void Leading::ensurePositive(Addressable &C) const throw()
        {
            EnsurePositive(C,reac);
            EnsurePositive(C,prod);
        }


#if 0
        void Leading:: limitedBy(const Actor &a, const double x, Addressable &C, const iMatrix &NuT, Addressable &xi) const throw()
        {
            xi[root->indx] = x;
            tao::mul_add(C,NuT,xi);
            C[a.sp.indx] = 0;
            ensurePositive(C);
        }

        void Leading:: updateAll(const double x, Addressable &C, const iMatrix &NuT, Addressable &xi) const throw()
        {
            xi[root->indx] = x;
            tao::mul_add(C,NuT,xi);
            ensurePositive(C);
        }


        Y_CHEMICAL_LEADING_MOVE_RET Leading:: moveLimitedByNone(Y_CHEMICAL_LEADING_MOVE_API) const throw()
        {
            Y_CHEMICAL_PRINTLN( "      " << root << " = " << std::setw(xwidth) << x << " is " << kindText() );
            xi[root->indx] = x;
            tao::mul_add(C,NuT,xi);
            return true;
        }




        Y_CHEMICAL_LEADING_MOVE_RET Leading:: moveLimitedByReac(Y_CHEMICAL_LEADING_MOVE_API) const throw()
        {
            const Actor &amax = maxFromReac(C);
            Y_CHEMICAL_PRINTLN( "      " << root << " = " << std::setw(xwidth) << x << " is " << kindText() << " <=  " << amax.sp << "/" << amax.nu <<  " = " << xmax);


            if(x>=xmax)
            {
                Y_CHEMICAL_PRINTLN("       \\_max limited by " << amax.sp);
                limitedBy(amax,xmax,C,NuT,xi);
                return false;
            }
            else
            {
                updateAll(x,C,NuT,xi);
                return true;
            }
        }


        Y_CHEMICAL_LEADING_MOVE_RET Leading:: moveLimitedByProd(Y_CHEMICAL_LEADING_MOVE_API) const throw()
        {
            const Actor &amin = minFromProd(C);
            Y_CHEMICAL_PRINTLN( "      " << root << " = " << std::setw(xwidth) << x << " is " << kindText() << " >= -" << amin.sp << "/" << amin.nu << " = " << xmin);


            if(x<=xmin)
            {
                Y_CHEMICAL_PRINTLN("       \\_min limited by " << amin.sp);
                limitedBy(amin,xmin,C,NuT,xi);
                return false;
            }
            else
            {
                updateAll(x,C,NuT,xi);
                return true;
            }

        }

        Y_CHEMICAL_LEADING_MOVE_RET Leading:: moveLimitedByBoth(Y_CHEMICAL_LEADING_MOVE_API) const throw()
        {
            const Actor &amax = maxFromReac(C);
            const Actor &amin = minFromProd(C);
            Y_CHEMICAL_PRINTLN( "      " << root << " = " << std::setw(xwidth) << x << " is " << kindText()
                               << " in { -" << amin.sp << "/" << amin.nu << " = " << xmin
                               << ":"      << amax.sp << "/" << amax.nu << " = " << xmax
                               << " }");

            if(x<=xmin)
            {
                Y_CHEMICAL_PRINTLN("       \\_min limited by " << amin.sp);
                limitedBy(amin,xmin,C,NuT,xi);
                return false;
            }
            else
            {
                if(x>=xmax)
                {
                    Y_CHEMICAL_PRINTLN("       \\_max limited by " << amax.sp);
                    limitedBy(amax,xmax,C,NuT,xi);
                    return false;
                }
                else
                {
                    updateAll(x,C,NuT,xi);
                    return true;
                }
            }
            

        }


#define Y_CHEMICAL_MOVE_KIND(KIND) case KIND : res = move##KIND(x,C,NuT,xi); break

        Y_CHEMICAL_LEADING_MOVE_RET Leading:: moveAll(Y_CHEMICAL_LEADING_MOVE_API) const throw()
        {
            bool res = true;
            tao::ld(xi,0);
            switch(kind)
            {
                    Y_CHEMICAL_MOVE_KIND(LimitedByNone);
                    Y_CHEMICAL_MOVE_KIND(LimitedByReac);
                    Y_CHEMICAL_MOVE_KIND(LimitedByProd);
                    Y_CHEMICAL_MOVE_KIND(LimitedByBoth);
            }
            Y_CHEMICAL_PRINTLN( "      " << root << " ==> " << (res? "full move" : "limited") << std::endl);
            return res;
        }
#endif
        
    }
}

