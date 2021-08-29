
#include "y/chemical/equilibrium.hpp"
#include "y/ios/scribe.hpp"
#include "y/ios/osstream.hpp"

namespace upsylon
{
    namespace Chemical
    {
        
        Equilibrium:: ~Equilibrium() throw() {}
        
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
            }
            else
            {
                if(nu>0)
                {
                    aliasing::_(prod)( size_t(nu), sp);
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
