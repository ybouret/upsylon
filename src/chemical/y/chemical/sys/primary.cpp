
#include "y/chemical/sys/primary.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Primary:: Limiting:: Limiting(const size_t n) : Limiting_(n,as_capacity)
        {
        }

        Primary:: Limiting:: ~Limiting() throw()
        {
        }
    }

}

namespace upsylon
{
    namespace Chemical
    {
        Primary:: LimitingReac:: LimitingReac(const size_t n) : Limiting(n)
        {
        }

        Primary:: LimitingReac:: ~LimitingReac() throw()
        {
        }

        const char * Primary:: LimitingReac:: symbol() const throw()
        {
            return " <=  ";
        }

        double Primary::LimitingReac:: rh_val(const double c) const throw()
        {
            return c;
        }

        const Actor & Primary:: LimitingReac:: operator()(double &x, const Accessible &C) const throw()
        {
            assert(size()>0);
            const Limiting &self = *this;
            const Actor    *pOpt = &self[1];
            double          xOpt = C[pOpt->sp.indx]/pOpt->nu;
            for(size_t i=this->size();i>1;--i)
            {
                const Actor *pTmp = &self[i];
                const double xTmp = C[pTmp->sp.indx]/pTmp->nu;
                if(xTmp<xOpt)
                {
                    xOpt = xTmp;
                    pOpt = pTmp;
                }
            }
            x = xOpt;
            return *pOpt;
        }

    }

}



namespace upsylon
{
    namespace Chemical
    {

        Primary:: LimitingProd:: LimitingProd(const size_t n) : Limiting(n)
        {
        }

        Primary:: LimitingProd:: ~LimitingProd() throw()
        {
        }

        const char * Primary:: LimitingProd:: symbol() const throw()
        {
            return " >= -";
        }

        double Primary::LimitingProd:: rh_val(const double c) const throw()
        {
            return -c;
        }

        const Actor & Primary:: LimitingProd:: operator()(double &x, const Accessible &C) const throw()
        {
            assert(size()>0);
            const Limiting &self = *this;
            const Actor    *pOpt = &self[1];
            double          xOpt = -C[pOpt->sp.indx]/pOpt->nu;
            for(size_t i=this->size();i>1;--i)
            {
                const Actor *pTmp = &self[i];
                const double xTmp = -C[pTmp->sp.indx]/pTmp->nu;
                if(xTmp>xOpt)
                {
                    xOpt = xTmp;
                    pOpt = pTmp;
                }
            }
            x = xOpt;
            return *pOpt;
        }
    }

}


namespace upsylon
{
    namespace Chemical
    {
        
        Primary:: ~Primary() throw()
        {
        }


        template <typename SEQ>
        static inline
        void loadPrimary(SEQ &seq, const Actors &actors)
        {
            for(const ANode *node=actors->head();node;node=node->next)
            {
                const Actor &a = **node;
                if(1==a.sp.rating)
                {
                    seq.push_back_(a);
                }
            }

        }




        Primary:: Primary(const Equilibrium &eq, const iMatrix &topo) :
        authority<const Equilibrium>(eq),
        NuT(topo),
        reac((**this).countPrimaryReac()),
        prod((**this).countPrimaryProd()),
        kind(LimitedByNone)
        {

            loadPrimary(aliasing::_(reac),eq.reac);
            loadPrimary(aliasing::_(prod),eq.prod);

            if(reac.size())
            {
                if(prod.size())
                {
                    // some reactant(s)/some product(s)
                    aliasing::_(kind) = LimitedByBoth;
                }
                else
                {
                    // some reactant(s)/no product
                    aliasing::_(kind) = LimitedByReac;
                }
            }
            else
            {

                if(prod.size())
                {
                    // no reactant/some product(s)
                    aliasing::_(kind) = LimitedByProd;
                }
                else
                {
                    // no reactant/no product
                    aliasing::_(kind) = LimitedByNone;
                }
            }

        }

        size_t Primary:: count() const throw()
        {
            return reac.size()+prod.size();
        }

        const char * Primary:: kindText() const throw()
        {
            switch(kind)
            {
                case LimitedByNone: return "None";
                case LimitedByReac: return (reac.size()>1 ? "Reactants" : "Reactant");
                case LimitedByProd: return (prod.size()>1 ? "Products"  : "Product");
                case LimitedByBoth:
                    if(reac.size()>1)
                    {
                        if(prod.size()>1)
                        {
                            return "Reactants and Products";
                        }
                        else
                        {
                            return "Reactants and Product";
                        }
                    }
                    else
                    {
                        if(prod.size()>1)
                        {
                            return "Reactant and Products";
                        }
                        else
                        {
                            return "Reactant and Product";
                        }
                    }
            }
            return "???";
        }

    }

}

