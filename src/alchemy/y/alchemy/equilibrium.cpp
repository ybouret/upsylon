#include "y/alchemy/equilibrium.hpp"
#include "y/exception.hpp"

#include "y/type/aliasing.hpp"
#include "y/type/utils.hpp"
#include "y/ios/osstream.hpp"

#include "y/core/ipower.hpp"
#include "y/mkl/tao.hpp"

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
        
        string equilibrium:: format() const
        {
            string ans;
            {
                ios::osstream fp(ans);
                fp << '<' << name << '>';
                for(size_t i=name.size();i<width;++i) fp << ' ';
                fp << '|' << reac <<   " <=> " << prod;
            }
            return ans;
        }
        
        void equilibrium:: on_compile()
        {
            assert(!compiled);
            assert(!reac.compiled);
            assert(!prod.compiled);
            aliasing::_(reac).compile();
            aliasing::_(prod).compile();
        }
        
        void equilibrium:: fill( addressable<long> &Nu ) const throw()
        {
            assert(compiled);
            mkl::tao::ld(Nu,0);
            for(size_t i=reac->size();i>0;--i)
            {
                Nu[ reac.indx[i] ] = -static_cast<long>( reac.coef[i] );
            }
            
            for(size_t i=prod->size();i>0;--i)
            {
                Nu[ prod.indx[i] ] = static_cast<long>( prod.coef[i] );
            }
        }
        
#if 0
        size_t equilibrium:: forward(double &xi, const accessible<double> &C) const throw()
        {
            assert(compiled);
            double        xm   = 0;
            size_t        im   = 0;
            const size_t *indx = reac.indx;
            const size_t *coef = reac.coef;
            {
                size_t        i = 0;
                // find first
                for(i=reac->size();i>0;--i)
                {
                    const double c = C[ indx[i] ]; assert(c>=0);
                    if(c>0)
                    {
                        xm = c/coef[i];
                        im = i;
                        --i;
                        break;
                    }
                }
                
                // find optimal
                for(;i>0;--i)
                {
                    const double c = C[ indx[i] ]; assert(c>=0);
                    if(c>0)
                    {
                        const double xt = c/coef[i];
                        if(xt<xm)
                        {
                            xm = xt;
                            im = i;
                        }
                    }
                }
            }
            xi  =  xm;
            return im;
        }
#endif
        
        
        double equilibrium:: compute(const double             K0,
                                     const accessible<double> &C) const throw()
        {
            assert(compiled);
            double lhs = K0;
            {
                size_t        n    = reac->size();
                const size_t *indx = reac.indx;
                const size_t *coef = reac.coef;
                while(n>0)
                {
                    lhs *= ipower<double>( C[indx[n]], coef[n] );
                    --n;
                }
            }
            
            double rhs = 1;
            {
                size_t        n    = prod->size();
                const size_t *indx = prod.indx;
                const size_t *coef = prod.coef;
                while(n>0)
                {
                    rhs *= ipower<double>( C[indx[n]], coef[n] );
                    --n;
                }
                
            }
            
            return lhs-rhs;
        }
        
        
        struct eqwrapper
        {
            const equilibrium   &eq;
            const double         K0;
            const accessible<double> &Cini;
            addressable<double> &Ctry;
            
            double operator()(const double xi) const throw()
            {
                mkl::tao::set(Ctry,Cini);
                
                double lhs = K0;
                {
                    size_t        n    = eq.reac->size();
                    const size_t *indx = eq.reac.indx;
                    const size_t *coef = eq.reac.coef;
                    while(n>0)
                    {
                        const size_t nu = coef[n];
                        const double c  = (Ctry[ indx[n] ] -= nu * xi);
                        lhs *= ipower<double>(c,nu);
                        --n;
                    }
                    
                }
                
                double rhs = 1;
                {
                    size_t        n    = eq.prod->size();
                    const size_t *indx = eq.prod.indx;
                    const size_t *coef = eq.prod.coef;
                    while(n>0)
                    {
                        const size_t nu = coef[n];
                        const double c  = (Ctry[ indx[n] ] += nu * xi);
                        rhs *= ipower<double>(c,nu);
                        --n;
                    }
                }
                
                return lhs-rhs;
            }
            
        };
        
        void   equilibrium:: solve(addressable<double> &Cini,
                                   const double         K0,
                                   addressable<double> &Ctry) const throw()
        {
            eqwrapper eqn = { *this, K0, Cini, Ctry };
            
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

