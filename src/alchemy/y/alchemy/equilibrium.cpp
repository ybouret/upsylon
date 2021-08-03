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
        }
        
        const string & Equilibrium:: key() const throw()
        {
            return name;
        }
        

        
        
        Extents Equilibrium:: findExtents(const accessible<double> &C) const throw()
        {
            return Extents(reac.findExtent(C),prod.findExtent(C));
        }

        double  Equilibrium:: compute(const double              K0,
                                      const accessible<double> &C) const throw()
        {
            return reac.massAction(K0,C) - prod.massAction(1,C);
        }

        double  Equilibrium:: compute(const double K0, const accessible<double> &C, const double xi) const throw()
        {
            return reac.massAction(K0,C,-xi) - prod.massAction(1,C,xi);
        }

        double Equilibrium:: compute(addressable<double> &phi, const double K0, const accessible<double> &C) const throw()
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

