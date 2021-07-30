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
            for(size_t i=reac.size;i>0;--i)
            {
                Nu[ reac.indx[i] ] = -static_cast<long>( reac.coef[i] );
            }
            
            for(size_t i=prod.size;i>0;--i)
            {
                Nu[ prod.indx[i] ] = static_cast<long>( prod.coef[i] );
            }
        }
        
        extents equilibrium:: find_extents(const accessible<double> &C) const throw()
        {
            assert(compiled);
            return extents(reac.find_extent(C),prod.find_extent(C));
        }
        
        
        double equilibrium:: compute(const double             K0,
                                     const accessible<double> &C) const throw()
        {
            assert(compiled);
            double lhs = K0;
            {
                size_t        n    = reac.size;
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
                size_t        n    = prod.size;
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
                                   addressable<double> &Ctry) const  
        {
            //eqwrapper eqn = { *this, K0, Cini, Ctry };
            
            static const int invalid = 0x00;
            static const int fwd_lim = 0x01;
            static const int rev_lim = 0x02;
            
            const extents xtn   = find_extents(Cini);
            int           flags = xtn.forward.index ? fwd_lim : invalid;
            if(xtn.reverse.index) flags |= rev_lim;
            
            switch(flags)
            {
                case invalid:
                    throw exception("<%s> is invalid", *name);
                    
                case fwd_lim: std::cerr << "forward limited @" << xtn.forward.index << std::endl;
                    break;
                    
                case rev_lim: std::cerr << "reverse limited @" << xtn.reverse.index << std::endl;
                    break;
                    
                case fwd_lim|rev_lim:
                    std::cerr << "forward limited @" << xtn.forward.index << std::endl;
                    std::cerr << "reverse limited @" << xtn.reverse.index << std::endl;
                    break;
            }
            
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

