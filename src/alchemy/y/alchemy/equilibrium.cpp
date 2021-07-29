#include "y/alchemy/equilibrium.hpp"
#include "y/exception.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"

namespace upsylon
{
    namespace alchemy
    {
        equilibrium:: ~equilibrium() throw()
        {
        }
        
        
        
        void equilibrium:: operator()(const long nu, const species &sp)
        {
            if(nu<0)
            {
                if(prod.search(sp.name))
                {
                    throw exception("%s: '%s' is registered as a product!", *name, *sp.name);
                }
                // append to reactants
                const unsigned long mnu =static_cast<unsigned long>(-nu);
                aliasing::_(reac)(sp,mnu);
            }
            else
            {
                if(nu>0)
                {
                    if(reac.search(sp.name))
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
        
        const string & equilibrium:: key() const throw()
        {
            return name;
        }

    }
}

namespace upsylon
{
    namespace alchemy
    {
        constant_equilibrium:: ~constant_equilibrium() throw()
        {
        }
        
        double constant_equilibrium:: K(double) const
        {
            return Kvalue;
        }
    }
}

namespace upsylon
{
    namespace alchemy
    {
        
        std::ostream & equilibrium:: display(std::ostream  &os,
                                             const library &lib,
                                             const double   t,
                                             const size_t   eqw) const
        {
            os << '<' << name << '>';
            for(size_t i=name.size();i<=eqw;++i) os << ' ';
            reac.display(os,lib);
            os << " <=> ";
            prod.display(os,lib);
            os << "    (" << K(t) << ")";
            return os;
        }
        
        
    }
    
}

