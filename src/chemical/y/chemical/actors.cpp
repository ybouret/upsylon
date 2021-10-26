#include "y/chemical/actors.hpp"
#include "y/core/ipower.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Actors:: ~Actors() throw()
        {
        }
        
        Actors:: Actors() : adb()
        {
        }
        
        void Actors:: operator()(const size_t nu, const Species &sp)
        {
            if(nu<=0) throw exception("cannot use 0*%s",*sp.name);
            
            const be_key key(sp);
            const Actor  act(nu,sp);
            if(!adb.insert(key,act))
            {
                throw exception("multiple actor '%s'",*sp.name);
            }
            
            aliasing::incr(sp.rating);
        }

        Actors::const_type & Actors:: bulk() const throw()
        {
            return adb;
        }
        
        bool Actors:: owns(const Species &sp) const throw()
        {
            const be_key key(sp);
            return adb.search(key) != NULL;
        }

        unit_t Actors:: totalCharge() const throw()
        {
            unit_t sum = 0;
            for(const ANode *node=adb.head();node;node=node->next)
            {
                const Actor &a = **node;
                sum += unit_t(a.nu) * a.sp.charge;
            }
            return sum;
        }

        size_t Actors:: tellLeading() const throw()
        {
            size_t res = 0;
            for(const ANode *node=adb.head();node;node=node->next)
            {
                if( 1 == (**node).sp.rating) ++res;
            }
            return res;
        }
        
        void Actors:: fillLeading(sequence<const Actor> &leading) const
        {
            for(const ANode *node=adb.head();node;node=node->next)
            {
                const Actor &a = **node;
                if( 1 == a.sp.rating) leading.push_back(a);
            }
        }

        double Actors:: massAction(double res, const Accessible &C) const throw()
        {
            for(const ANode *node=adb.head();node;node=node->next)
            {
                const Actor &a = **node;
                res *= a.massAction(C);
            }
            return res;
        }

        void Actors:: massActionJ(const double ini, Addressable &J, const Accessible &C) const throw()
        {

            for(const ANode *node=adb.head();node;node=node->next)
            {
                const Actor &a    = **node;
                const size_t j    = a.sp.indx;
                double       res  = ini * a.nu * ipower<double>(C[j],a.nu1);
                for(const ANode *other=node->prev;other;other=other->prev)
                {
                    res *= (**other).massAction(C);
                }
                for(const ANode *other=node->next;other;other=other->next)
                {
                    res *= (**other).massAction(C);
                }
                J[j] = res;
            }


        }


    }
    
}


