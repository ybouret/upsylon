
#include "y/alchemy/equilibria.hpp"
#include "y/exception.hpp"
#include "y/type/utils.hpp"
#include "y/type/aliasing.hpp"

namespace upsylon
{
    namespace Alchemy
    {

        Equilibria::const_type & Equilibria:: bulk() const throw()
        {
            return edb;
        }

        Equilibria:: ~Equilibria() throw()
        {
            
        }
        
        Equilibria:: Equilibria() : edb(), enw(), tdisp(0)
        {
        }

        const char Equilibria::CLID[] = "Equilibria";

        Equilibrium & Equilibria:: operator()(Equilibrium *eq)
        {
            assert(eq);
            // take care of eq
            const Equilibrium::Pointer p(eq); assert(0==eq->indx);
            if(frozen)         throw exception("%s: frozen while adding '%s'",CLID,*(eq->name));
            if(!edb.insert(p)) throw exception("%s: multipliple equilibrium '%s'", CLID, *(eq->name) );

            // update status
            aliasing::_(enw)      = max_of(enw,eq->name.size());
            aliasing::_(eq->indx) = edb.size();
            return *eq;
        }


        const Equilibrium & Equilibria:: operator()(const size_t indx) const throw()
        {
            assert(indx>=1);assert(indx<=edb.size());
            const Equilibrium &eq = *edb.fetch(indx-1);
            assert(indx==eq.indx);
            return eq;
        }


        void Equilibria:: verify(const unsigned flags) const
        {
            size_t indx = 0;
            for(const Equilibrium::Node *node=edb.head();node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                if(++indx!=eq.indx) throw exception("%s wrong index for '%s'",CLID, *eq.name);
                eq.verify(flags);
            }
        }


        void Equilibria:: compute(Addressable &K, const double t) const throw()
        {
            assert(K.size()>=edb.size());
            size_t i=0;
            for(const Equilibrium::Node *node=edb.head();node;node=node->next)
            {
                K[++i] = (**node)->K(t);
            }

        }

        void Equilibria:: compute(Addressable      &K,
                                  Addressable      &Gam,
                                  const Accessible &C,
                                  const double      t) const throw()
        {
            size_t i=0;
            for(const Equilibrium::Node *node=edb.head();node;node=node->next)
            {
                ++i;
                const Equilibrium &eq = ***node;
                const double       K0 = (K[i] = eq.K(t));
                Gam[i] = eq.compute(K0,C);
            }
        }

        void Equilibria::upgrade(const Accessible &K,
                                 Addressable      &Gam,
                                 const Accessible &C) const throw()
        {
            size_t i=0;
            for(const Equilibrium::Node *node=edb.head();node;node=node->next)
            {
                ++i;
                const Equilibrium &eq = ***node;
                Gam[i] = eq.compute(K[i],C);
            }
        }


        void Equilibria:: compute(Addressable      &K,
                                  Addressable      &Gam,
                                  Matrix           &Phi,
                                  const Accessible &C,
                                  const double      t) const throw()
        {
            assert(K.size()  >=edb.size());
            assert(Gam.size()>=edb.size());
            assert(Phi.rows  >=edb.size());

            size_t i=0;
            for(const Equilibrium::Node *node=edb.head();node;node=node->next)
            {
                ++i;
                const Equilibrium &eq = ***node;
                const double       K0 = (K[i] = eq.K(t));
                Gam[i] = eq.compute(Phi[i],K0,C);
            }
            
        }

        void Equilibria:: upgrade(const Accessible &K,
                                  Addressable      &Gam,
                                  Matrix           &Phi,
                                  const Accessible &C) const throw()
        {
            assert(K.size()  >=edb.size());
            assert(Gam.size()>=edb.size());
            assert(Phi.rows  >=edb.size());

            size_t i=0;
            for(const Equilibrium::Node *node=edb.head();node;node=node->next)
            {
                ++i;
                const Equilibrium &eq = ***node;
                Gam[i] = eq.compute(Phi[i],K[i],C);
            }
        }

        size_t Equilibria:: guess(addressable<bool> &active) const throw()
        {
            size_t na = active.size();
            for(size_t i=na;i>0;--i) active[i] = false;

            for(const Equilibrium::Node *node=edb.head();node;node=node->next)
            {
                (**node)->guess(active);
            }
            size_t ans = 0;
            for(size_t i=na;i>0;--i)
            {
                if(active[i]) ++ans;
            }
            return ans;
        }

        
    }
}

