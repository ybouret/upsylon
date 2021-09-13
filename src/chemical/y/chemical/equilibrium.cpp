
#include "y/chemical/equilibrium.hpp"
#include "y/ios/scribe.hpp"
#include "y/ios/osstream.hpp"

namespace upsylon
{
    namespace Chemical
    {
        Player:: ~Player() throw()
        {
        }
        
        Player:: Player(const Actor &a, const Role r) throw() :
        Object(),
        authority<const Actor>(a),
        role(r)
        {
            
        }
        
        Player:: Player(const Player &other) throw() :
        Object(),
        authority<const Actor>(other),
        role(other.role)
        {
            
        }

        
        const string &Player:: key() const throw() { return (**this).sp.name; }
        
            
    }
    
}

namespace upsylon
{
    namespace Chemical
    {
        
        Equilibrium:: ~Equilibrium() throw() {}


        bool Equilibrium:: isBounded() const throw()
        {
            return (reac->size()>0) && (prod->size()>0);
        }


        string Equilibrium:: Kstr(const double t) const
        {
            static const ios::scribe &S = ios::scribe::query<double>();
            const  double k = K(t);
            return S.write(&k);
        }
        
        string Equilibrium:: toRawString() const
        {
            string        res;
            ios::osstream fp(res);
            display_raw(static_cast<ios::ostream &>(fp));
            return res;
        }
            
        size_t Equilibrium:: countPrimaryReac() const throw()
        {
            return CountPrimary(reac);
        }


        size_t Equilibrium:: countPrimaryProd() const throw()
        {
            return CountPrimary(prod);
        }

        size_t Equilibrium:: CountPrimary(const Actors &actors) throw()
        {
            size_t ans = 0;
            for(const ANode *node=actors->head();node;node=node->next)
            {
                const Actor &a = **node;
                if(1==a.sp.rating) ++ans;
            }
            return ans;
        }

        
            
        ConstEquilibrium:: ~ConstEquilibrium() throw()
        {
        }
        
        double ConstEquilibrium:: K(double) const 
        {
            return K_;
        }

        unit_t Equilibrium:: deltaCharge() const throw()
        {
            return prod.totalCharge() - reac.totalCharge();
        }
        
        const string & Equilibrium:: key() const throw()
        {
            return name;
        }

        
        void Equilibrium:: operator()(const unit_t nu, const Species &sp)
        {
            if(prod.owns(sp)) throw exception("'%s' already in <%s> products",*sp.name,*name);
            if(reac.owns(sp)) throw exception("'%s' already in <%s> reactants",*sp.name,*name);
            
            if(nu<0)
            {
                aliasing::_(reac)( size_t(-nu), sp);
                const Player pl( **(reac->tail()), Player::Reactant );
                if(!aliasing::_(used).insert(pl))
                {
                    throw exception("unexpected multiple reactant '%s' in used species of <%s>",*sp.name,*name);
                }
            }
            else
            {
                if(nu>0)
                {
                    aliasing::_(prod)( size_t(nu), sp) ;
                    const Player pl( **(prod->tail()), Player::Product );
                    if(!aliasing::_(used).insert(pl))
                    {
                        throw exception("unexpected multiple product '%s' in used species of <%s>",*sp.name,*name);
                    }
                }
                else
                {
                    assert(0==nu);
                    throw exception("adding 0*'%s' to <%s>", *sp.name, *name);
                }
            }
        }

    }
    
}

#include "y/yap/gcd.hpp"

namespace upsylon
{
    namespace Chemical
    {
        void Equilibrium:: verify(const unsigned flags) const
        {

            const unit_t dz = deltaCharge();
            if(dz) throw exception("<%s> has deltaCharge=%ld",*name, long(dz));
           
            if(flags&Minimal)
            {
                vector<apn,Allocator> coef(reac->size()+prod->size(),as_capacity);
                reac.save(coef);
                prod.save(coef);
                const apn g = yap::compute_gcd::of(coef);
                if(g!=1) throw exception("<%s> is not minimal",*name);
            }
        }

        
    }
    
}
