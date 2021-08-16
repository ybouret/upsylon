#include "y/alchemy/equilibrium.hpp"
#include "y/exceptions.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/ios/osstream.hpp"

#include "y/core/ipower.hpp"
#include "y/mkl/tao.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        Equilibrium:: ~Equilibrium() throw()
        {
        }
        
        bool Equilibrium:: owns(const Species &sp) const throw()
        {
            return reac.owns(sp) || prod.owns(sp);
        }

        size_t Equilibrium:: weight(const Species &sp) const throw()
        {
            return max_of(reac.coeff(sp),prod.coeff(sp));
        }

        
        void Equilibrium:: operator()(const long nu, const Species &sp)
        {
            if(nu<0)
            {
                if( prod->search(sp.name) )
                {
                    throw exception("%s: '%s' is registered as a product!", *name, *sp.name);
                }
                // append to reactants
                const unsigned long mnu = static_cast<unsigned long>(-nu);
                aliasing::_(reac)(sp,mnu);
            }
            else
            {
                if(nu>0)
                {
                    if( reac->search(sp.name) )
                    {
                        throw exception("%s: '%s' is registered as a reactant!", *name, *sp.name);
                    }
                    // append to products
                    aliasing::_(prod)(sp,nu);
                }
                else
                {
                    throw exception("%s: zero coefficient for '%s'", *name, *sp.name);
                }
            }
            aliasing::_(sp.active) = true;
        }
        
        const string & Equilibrium:: key() const throw()
        {
            return name;
        }
        

        
        
        Extents Equilibrium:: findExtents(const Accessible &C) const throw()
        {
            return Extents(reac.findExtent(C),prod.findExtent(C));
        }

        double  Equilibrium:: compute(const double      K0,
                                      const Accessible &C) const throw()
        {
            return reac.massAction(K0,C) - prod.massAction(1,C);
        }

        double  Equilibrium:: compute(const double K0, const Accessible &C, const double xi) const throw()
        {
            return reac.massAction(K0,C,-xi) - prod.massAction(1,C,xi);
        }

        double Equilibrium:: compute(Addressable &phi, const double K0, const Accessible &C) const throw()
        {
            return reac.jacobian(K0,phi,K0,C) - prod.jacobian(1,phi,-1,C);
        }

        void Equilibrium:: guess(addressable<bool> &active) const throw()
        {
            reac.guess(active);
            prod.guess(active);
        }

       

    }
}

namespace upsylon
{
    namespace Alchemy
    {
        ConstEquilibrium:: ~ConstEquilibrium() throw()
        {
        }
        
        double ConstEquilibrium:: K(double) const
        {
            return Kvalue;
        }
    }
}

#include "y/yap/natural.hpp"

namespace upsylon
{
    namespace Alchemy
    {
        void Equilibrium:: verify(const unsigned flags) const
        {
            if(prod->size()<=0 && reac->size()<=0) throw exception("%s is empty",*name);
            
            // verify charge, mandatory
            {
                const long DrZ = prod.z() - reac.z();
                if(DrZ!=0) throw exception("%s has Delta_rZ=%ld", *name, DrZ);
            }
            
            // verifiy co-primary coefficient
            if( 0 != (flags&Minimal) )
            {
                vector<apn,Allocator> coeff(reac->size()+prod->size(),as_capacity);
                for(const Actor::Node *node=reac->head();node;node=node->next)
                {
                    const apn cof = (**node).nu;
                    coeff.push_back_(cof);
                }
                for(const Actor::Node *node=prod->head();node;node=node->next)
                {
                    const apn cof = (**node).nu;
                    coeff.push_back_(cof);
                }
                //std::cerr << "coeff=" << coeff << std::endl;
                apn  gcd = coeff[1];
                bool res = false;
                for(size_t i=coeff.size();i>1;--i)
                {
                    gcd = apn::gcd(gcd,coeff[i]);
                    if(1==gcd)
                    {
                        res = true;
                        break;
                    }
                }
                if(!res)
                {
                    const string tmp = gcd.to_dec();
                    throw exception("%s is not minimal: should be divided by %s",*name,*tmp);
                }
            }
            
        }

    }
    
}

