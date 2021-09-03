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


