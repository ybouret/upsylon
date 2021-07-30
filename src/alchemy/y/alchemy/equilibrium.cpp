#include "y/alchemy/equilibrium.hpp"
#include "y/exception.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/ios/osstream.hpp"

#include "y/core/ipower.hpp"

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
                if( prod.search(sp.name) )
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
                    if( reac.search(sp.name) )
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

        string equilibrium:: format(const size_t name_width,
                                    const size_t spec_width,
                                    const size_t reac_width,
                                    const size_t prod_width) const
        {
            string ans;
            {
                ios::osstream fp(ans);
                fp << '<' << name << '>';
                for(size_t i=name.size();i<name_width;++i) fp << ' ';
                fp << '|';
                
                reac.display(fp,spec_width,reac_width<=0 ? reac.cwidth : reac_width);
                fp << " <=> ";
                prod.display(fp,spec_width,prod_width<=0 ? prod.cwidth : prod_width);
            }
            return ans;
        }



        double equilibrium:: compute(const double             K0,
                                     const accessible<double> &C) const throw()
        {
            double lhs = K0;
            {
                size_t n = reac->size();
                for(actors::const_iterator it=reac->begin();n>0;++it,--n)
                {
                    const actor &a = *it;     assert(a.nu>0);
                    const size_t i = a->indx; assert(i>=1); assert(i<=C.size());
                    const double c = C[i];    assert(c>=0);
                    lhs *= ipower<double>(c,a.nu);
                }
            }

            double rhs = 1;
            {
                size_t n = prod->size();
                for(actors::const_iterator it=prod->begin();n>0;++it,--n)
                {
                    const actor &a = *it;     assert(a.nu>0);
                    const size_t i = a->indx; assert(i>=1); assert(i<=C.size());
                    const double c = C[i];    assert(c>=0);
                    rhs *= ipower<double>(c,a.nu);
                }
            }
            
            return lhs-rhs;
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

