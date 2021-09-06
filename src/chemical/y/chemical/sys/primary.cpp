
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

        double Primary::LimitingReac:: by(const double c) const throw()
        {
            return c;
        }

        const Actor & Primary:: LimitingReac:: operator()(double &x, const Accessible &C) const throw()
        {

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

        double Primary::LimitingProd:: by(const double c) const throw()
        {
            return -c;
        }

        const Actor & Primary:: LimitingProd:: operator()(double &x, const Accessible &C) const throw()
        {

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




        Primary:: Primary(const Equilibrium &eq) :
        root(eq),
        reac(root.countPrimaryReac()),
        prod(root.countPrimaryProd())
        {

            loadPrimary(aliasing::_(reac),eq.reac);
            loadPrimary(aliasing::_(prod),eq.prod);
            
        }

        size_t Primary:: count() const throw()
        {
            return reac.size()+prod.size();
        }

    }

}
